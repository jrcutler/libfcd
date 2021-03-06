/*! \file
 * \brief FUNcube dongle flash utility
 * \author Justin R. Cutler
 */
/*
 * Copyright (C) 2012 Justin R. Cutler
 *
 * libfcd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libfcd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfcd.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h> /* fprintf, stderr, perror, fopen, fclose, fseek, ftell, SEEK_END, SEEK_SET */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, NULL */
#ifdef HAVE_GETOPT_H
# include <getopt.h> /* getopt_long */
#endif
#include <limits.h> /* CHAR_MAX */
#include "fcd.h" /* FCD, fcd_* */


/*
 * Enumerations
 */


/*! \brief Command line options */
enum
{
	/*! \brief Display help and exit */
	OPTION_HELP = CHAR_MAX + 1,
	/*! \brief Display version and exit */
	OPTION_VERSION,
	/*! \brief Perform full flash update */
	OPTION_FLASH
};

/*! \brief Action flags */
enum
{
	/*! \brief Reset device */
	ACTION_RESET=1<<0,
	/*! \brief Erase flash */
	ACTION_ERASE=1<<1,
	/*! \brief Write flash */
	ACTION_WRITE=1<<2,
	/*! \brief Verify flash */
	ACTION_VERIFY=1<<3
};


/*
 * Types
 */


/*! \brief Flash context */
typedef struct
{
	/*! \brief action flags set by command line */
	unsigned int actions;
	/*! \brief reset delay (in ms) */
	unsigned int delay;
	/*! \brief Flash data */
	unsigned char *data;
	/*! \brief Flash data size */
	unsigned long int size;
} flash_context;


/*
 * Variables
 */


/*! \brief Long command line options */
static const struct option long_options[] =
{
	/* options */
	{"input",     required_argument, NULL, 'i'},
	/* actions */
	{"reset",     optional_argument, NULL, 'r'},
	{"no-reset",  no_argument,       NULL, 'R'},
	{"erase",     no_argument,       NULL, 'e'},
	{"no-erase",  no_argument,       NULL, 'E'},
	{"write",     no_argument,       NULL, 'w'},
	{"no-write",  no_argument,       NULL, 'W'},
	{"verify",    no_argument,       NULL, 'v'},
	{"no-verify", no_argument,       NULL, 'V'},
	{"help",      no_argument,       NULL, OPTION_HELP},
	{"version",   no_argument,       NULL, OPTION_VERSION},
	/* meta-actions */
	{"flash",     required_argument, NULL, OPTION_FLASH},
	/* end of list */
	{NULL, 0, NULL, 0}
};

/*! \brief Flash error messages */
static const char *flash_error_message[] =
{
	"unknown error",
	"get_address_range failed",
	"invalid address range",
	"invalid firmware image",
	"set address failed",
	"write failed",
	"verify failed",
};


/*
 * Functions
 */


/*!
 * \brief Get the length of a stream
 * \param[in,out] stream open stream
 * \note \p stream must be seekable
 * \returns Stream length (or -1 on error)
 * \post \p stream is at beginning
 */
static long int fsize(FILE *stream)
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
static void *fread_all(FILE *stream, unsigned long *len)
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


/*! \brief Look up an error message by \p result
 * \param result previous operation's result
 * \returns String containing error message
 */
static const char *error_msg(int result)
{
	switch (result)
	{
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
			return flash_error_message[-result];
			break;
		default:
			break;
	}
	return flash_error_message[0];
}


/*! \copydetails fcd_path_callback
 * \brief Upgrade/verify each device's firmware
 */
static int funcube_flash(const char *path, void *context)
{
	flash_context *ctx = context;
	int result = 0;
	FCD *fcd;

	fcd = fcd_open(path);

	if (NULL != fcd)
	{
		if (ctx->actions & ACTION_ERASE)
		{
			/* erase flash */
			result = fcd_bl_erase_application(fcd);
			if (result)
			{
				fprintf(stderr, "[%s] erase failed\n", path);
			}
		}
		if (!result && ctx->actions & ACTION_WRITE)
		{
			/* flash device */
			result = fcd_bl_flash_write(fcd, ctx->data, ctx->size);
			if (result)
			{
				fprintf(stderr, "[%s] write: %s\n", path, error_msg(result));
			}
		}
		if (!result && ctx->actions & ACTION_VERIFY)
		{
			/* verify flash */
			result = fcd_bl_flash_verify(fcd, ctx->data, ctx->size);
			if (result)
			{
				fprintf(stderr, "[%s] verify: %s\n", path, error_msg(result));
			}
		}
	}
	else
	{
		result = -1;
	}

	fcd_close(fcd);

	return result;
}


/*! \brief Print an error and exit */
static void die(void)
{
	fputs("Try `fcd-flash --help' for more information.\n", stderr);
	exit(EXIT_FAILURE);
}


/*! \brief Print the version and exit */
static void version(void)
{
	printf("fcd-flash (%s) %s\n", PACKAGE_NAME, PACKAGE_VERSION);
	puts("Copyright (C) 2012 Justin R. Cutler");
	puts("This is free software; see the source for copying conditions.  There is NO");
	puts("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
	exit(EXIT_SUCCESS);
}


/*! \brief Print usage and exit */
static void usage(void)
{
	puts("Usage: fcd-flash [OPTION]...");
	puts("Perform firmware operations on all present FUNcube dongles.");
	puts("");
	puts("Mandatory arguments to long options are mandatory for short options too.");
	puts("      --flash=FILE  perform a full flash upgrade from firmware image");
	puts("                    (equivalent to `-r -ewv -iFILE`)");
	puts("  -i, --input=FILE  read image from FILE");
	puts("  -r, --reset[=MS]  reset to/from bootloader with a MS millisecond");
	puts("                    delay (default is 2000 ms)");
	puts("  -R, --no-reset    do not reset");
	puts("  -e, --erase       erase flash");
	puts("  -E, --no-erase    do not erase flash");
	puts("  -w, --write       write image to flash");
	puts("  -W, --no-write    do not write to flash");
	puts("  -v, --verify      verify flash against image");
	puts("  -V, --no-verify   do not verify flash");
	puts("      --help        display this help and exit");
	puts("      --version     output version information and exit");
	puts("");
	puts("Examples:");
	puts("fcd-flash --flash=export18b.bin");
	puts("  Write `export18b.bin` to FUNcube dongle");
	puts("fcd-flash --flash=export18b.bin --no-erase --no-write");
	puts("  Reset and verify flash matches `export18b.bin`");
	puts("fcd-flash -r1000 -vi export18b.bin");
	puts("  Reset with 1 second delay and verify flash matches `export18b.bin`");

	exit(EXIT_SUCCESS);
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
	int c, index;
	char *filename = NULL;
	flash_context context = {0, 2000, NULL, 0};

	/* parse command line */
	while ((c = getopt_long(argc, argv, "i:r::ReEwWvV", long_options, &index)) != -1)
	{
		switch (c)
		{
			case 'i':
				filename = optarg;
				break;
			case 'r':
				context.actions |= ACTION_RESET;
				if (NULL != optarg)
				{
					char *end;
					context.delay = strtoul(optarg, &end, 0);
					if (end == optarg)
					{
						fprintf(stderr, "Invalid reset delay\n");
						die();
					}
				}
				break;
			case 'R':
				context.actions &= ~ACTION_RESET;
				break;
			case 'e':
				context.actions |= ACTION_ERASE;
				break;
			case 'E':
				context.actions &= ~ACTION_ERASE;
				break;
			case 'w':
				context.actions |= ACTION_WRITE;
				break;
			case 'W':
				context.actions &= ~ACTION_WRITE;
				break;
			case 'v':
				context.actions |= ACTION_VERIFY;
				break;
			case 'V':
				context.actions &= ~ACTION_VERIFY;
				break;

			case OPTION_FLASH:
				context.actions = ACTION_RESET|ACTION_ERASE|ACTION_WRITE|ACTION_VERIFY;
				filename = optarg;
				break;

			case OPTION_HELP:
				usage();
				break;

			case OPTION_VERSION:
				version();
				break;

			default:
				die();
				break;
		}
	}

	/* check argument count */
	if (optind != argc)
	{
		fprintf(stderr, "extra operand: %s\n", argv[optind]);
		die();
	}

	/* verify an action was requested */
	if (!context.actions)
	{
		fputs("Nothing to do\n", stderr);
		die();
	}

	/* verify image filename is provided for write & verify */
	if ((context.actions & (ACTION_WRITE|ACTION_VERIFY)) && (NULL == filename))
	{
		fputs("No image file specfied\n", stderr);
		die();
	}

	/* read image */
	if (context.actions & (ACTION_WRITE|ACTION_VERIFY))
	{
		/* get file contents */
		FILE *f = fopen(filename, "rb");
		if (NULL == f)
		{
			perror("open");
			return EXIT_FAILURE;
		}
		context.data = fread_all(f, &context.size);
		fclose(f);
		if (NULL == context.data)
		{
			perror("read");
			return EXIT_FAILURE;
		}
	}

	/* enter bootloader */
	if (context.actions & ACTION_RESET)
	{
		fcd_reset_bootloader(context.delay);
	}

	/* erase/flash/verify all FUNcube dongles present */
	if (fcd_for_each(funcube_flash, &context))
	{
		result = EXIT_FAILURE;
	}

	/* restart all FUNcube dongles */
	if (context.actions & ACTION_RESET)
	{
		fcd_reset_application(context.delay);
	}

	if (NULL != context.data)
	{
		/* release file contents */
		free(context.data);
	}

	return result;
}
