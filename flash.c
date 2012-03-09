/*! \file
 * \brief FUNcube dongle flash utility
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h> /* fprintf, stderr, perror, fopen, fclose, fseek, ftell, SEEK_END, SEEK_SET */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, NULL */
#include "fcd.h" /* FCD, fcd_* */


/*! \brief Flash context */
typedef struct
{
	/*! \brief Flash data */
	unsigned char *data;
	/*! \brief Flash data size */
	unsigned long int size;
	
} flash_context;


/*!
 * \brief Get the length of a stream
 * \param[in,out] stream open stream
 * \note \p stream must be seekable
 * \returns Stream length (or -1 on error)
 * \post \p stream is at beginning
 */
long int fsize(FILE *stream)
{
	long int size;

	/* move to end of file */
	if (fseek(stream, 0, SEEK_END) == -1)
	{
		return -1;
	}

	/* get position at end of file (size) */
	size = ftell(stream);
	if (size == -1)
	{
		return -1;
	}

	/* move to beginning of file */
	if (fseek(stream, 0, SEEK_SET) == -1)
	{
		return -1;
	}

	return size;
}


/*!
 * \brief Read entire contents of file into a new buffer
 * \param[in,out] stream input stream
 * \param[out]    len    length of buffer
 * \retval non-NULL pointer to contents of stream
 * \retval NULL     error
 * \post If a non-NULL value is returned, it must be freed.
 */
void *fread_all(FILE *stream, unsigned long *len)
{
	void *buffer = NULL;
	long int size = fsize(stream);
	if (size != -1)
	{
		buffer = malloc(size);
	}
	if (NULL != buffer)
	{
		if (fread(buffer, size, 1, stream) == 1)
		{
			*len = (unsigned long)size;
		}
		else
		{
			free(buffer);
			buffer = NULL;
		}
	}
	return buffer;
}


/*! \copydetails fcd_path_callback
 * \brief Upgrade each device's firmware
 */
int funcube_flash(const char *path, void *context)
{
	flash_context *ctx = context;
	int result = -1;
	FCD *fcd;

	fcd = fcd_open(path);

	if (NULL != fcd)
	{
		/* flash device */
		result = fcd_bl_flash_write(fcd, ctx->data, ctx->size);
		/* display error (if any) */
		switch (result)
		{
			case 0:
			break;
			case -1:
				fprintf(stderr, "[%s] get_address_range failed\n", path);
			break;
			case -2:
				fprintf(stderr, "[%s] invalid address range\n", path);
			break;
			case -3:
				fprintf(stderr, "[%s] invalid firmware image\n", path);
			break;
			case -4:
				fprintf(stderr, "[%s] erase failed\n", path);
			break;
			case -5:
				fprintf(stderr, "[%s] set address failed\n", path);
			break;
			case -6:
				fprintf(stderr, "[%s] write failed\n", path);
			break;
			default:
				fprintf(stderr, "[%s] unknown error\n", path);
			break;
		}
	}

	fcd_close(fcd);

	return result;
}



/*!
 * \brief FUNcube dongle utility entry point
 * \param argc number of command line arguments
 * \param argv command line arguments
 * \retval EXIT_SUCCESS success
 * \retval EXIT_FAILURE failure
 */
int main(int argc, char **argv)
{
	int result = EXIT_SUCCESS;
	FILE *f;
	flash_context context = {NULL, 0};

	/* check argument count */
	if (argc != 2)
	{
		/* display help and exit */
		fprintf(stderr, "usage:\t%s <file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	f = fopen(argv[1], "rb");
	if (NULL == f)
	{
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	/* get file contents */
	context.data = fread_all(f, &context.size);
	if (NULL != context.data)
	{
		/* flash all FUNcube dongles present */
		fcd_reset_bootloader();
		if (!fcd_for_each(funcube_flash, &context))
		{
			/* restart all FUNcube dongles */
			fcd_reset_application();
		}
		else
		{
			result = EXIT_FAILURE;
		}

		/* release file contents */
		free(context.data);
	}

	fclose(f);

	return result;
}
