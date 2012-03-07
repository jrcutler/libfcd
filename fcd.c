/*! \file
 * \brief FUNcube dongle interface implementation
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h> /* NULL, malloc, free */
#include <string.h> /* memset, memcpy */
#ifdef HAVE_STDINT_H
# include <stdint.h> /* [u]int*_t */
#endif
#ifdef HAVE_USLEEP
# ifdef HAVE_UNISTD_H
#  include <unistd.h> /* usleep */
# endif
# define ms_sleep(ms) usleep((ms)*SHORT_SLEEP_FACTOR)
#elif defined(HAVE_NAP)
# define ms_sleep(ms) nap((ms)*SHORT_SLEEP_FACTOR)
#endif
#include "hidapi/hidapi.h" /* hid_* */
#include "fcd.h" /* FCD */
#include "fcd_cmd.h" /* FCD_CMD_* */


/*
 * Types
 */


/*! \brief Implementation of \ref FCD */
struct FCD_impl
{
	/*! \brief HID device */
	hid_device *hid_dev;
};

/*! \brief FUNcube dongle command data length */
#define FCD_COMMAND_DATA_LEN 63

/*! \brief Common FUNcube dongle command structure */
typedef struct
{
	/*! \brief Report ID (always 0) */
	unsigned char report_id;
	/*! \brief Command byte */
	unsigned char command;
	/*! \brief Payload data (varies by \p command) */
	unsigned char data[FCD_COMMAND_DATA_LEN];
} fcd_command;

/*! \brief FUNcube dongle response data length */
#define FCD_RESPONSE_DATA_LEN 62

/*! \brief Common FUNcube dongle response structure */
typedef struct
{
	/*! \brief Command byte (should match previous \ref fcd_command) */
	unsigned char command;
	/*! \brief Status byte (0 for failure, 1 for success) */
	unsigned char status;
	/*! \brief Payload data (varies by \p command) */
	unsigned char data[FCD_RESPONSE_DATA_LEN];
} fcd_response;

/*! \brief Shared command/response buffer type */
typedef union
{
	/*! \brief Command */
	fcd_command  command;
	/*! \brief Response */
	fcd_response response;
} fcd_buffer;


/*
 * Private Functions
 */


/*!
 * \brief Convert 32-bit integer to/from little-endian
 * \param v value
 * \returns \p v on little-endian systems or byte-swapped \p v on big-endian
 * systems
 */
inline uint32_t convert_le_u32(uint32_t v)
{
#ifdef WORDS_BIGENDIAN
	/* big endian */
	return ((v & 0xff000000) >> 24) |
	       ((v & 0x00ff0000) >>  8) |
	       ((v & 0x0000ff00) <<  8) |
	       ((v & 0x000000ff) << 24));
#else
	/* little endian */
	return v;
#endif
}


/*! \brief Perform a get command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[out]    data output data pointer
 * \param         len  output data length
 * \returns length of received data or -1 on error
 */
int fcd_get(FCD *dev, unsigned char cmd, void *data, unsigned char len)
{
	fcd_buffer buffer;
	int result = -1;

	/*! \todo validate cmd */
	/* do not allow NULL pointer for non-trivial get */
	if (len && (NULL == data)) return -1;
	/* trim request length as needed */
	if (len > sizeof(buffer.response.data))
	{
		len = sizeof(buffer.response.data);
	}

	/* send get request */
	buffer.command.report_id = 0;
	buffer.command.command = cmd;
	if (hid_write(dev->hid_dev, (unsigned char *)&buffer, 2) == 2)
	{
		/* receive get response */
		if (hid_read(dev->hid_dev, (unsigned char *)&buffer, len+2) == len+2)
		{
			/* validate response */
			if ((buffer.response.command == cmd) &&
				(buffer.response.status == 1))
			{
				memcpy(data, &buffer.response.data, len);
				result = len;
			}
		}
	}

	return result;
}


/*! \brief Perform a set command with optional skipped data bytes
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[in]     data input data pointer
 * \param         len  input data length
 * \param         skip number of data bytes to skip (normally 0)
 * \returns length of sent data or -1 on error
 * \note For now, this only appears to be necessary for the write block command.
 */
int fcd_set_skip(FCD *dev, unsigned char cmd, const void *data,
	unsigned char len, unsigned char skip)
{
	fcd_buffer buffer;
	int result = -1;

	/*! \todo validate cmd */
	/* do not allow NULL pointer for non-trivial set */
	if (len && (NULL == data)) return -1;
	/* trim request length as needed */
	if (len > sizeof(buffer.command.data) - skip)
	{
		len = sizeof(buffer.command.data) - skip;
	}

	/* send set request */
	buffer.command.report_id = 0;
	buffer.command.command = cmd;
	/* pad skipped byte(s) */
	memset(&buffer.command.data, 0, skip);
	/* copy in data */
	memcpy(&(buffer.command.data[skip]), data, len);
	if (hid_write(dev->hid_dev, (unsigned char *)&buffer, len+2+skip) ==
		len+2+skip)
	{
		/* receive set response */
		if (hid_read(dev->hid_dev, (unsigned char *)&buffer, 2) == 2)
		{
			/* validate response */
			if ((buffer.response.command == cmd) &&
				(buffer.response.status == 1))
			{
				result = len;
			}
		}
	}

	return result;
}


/*! \brief Perform a set command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[in]     data input data pointer
 * \param         len  input data length
 * \returns length of sent data or -1 on error
 */
int fcd_set(FCD *dev, unsigned char cmd, const void *data, unsigned char len)
{
	return fcd_set_skip(dev, cmd, data, len, 0);
}


/*! \copydetails fcd_path_fn
 * \brief Reset FUNcube dongle
 * \note \p context points to specified reset command
 */
int fcd_reset(const char *path, void *context)
{
	FCD *dev;
	unsigned char cmd = *(unsigned char*)context;

	/* try to open device */
	dev = fcd_open(path);
	if (NULL != dev)
	{
		/* reset and close */
		fcd_set(dev, cmd, NULL, 0);
		fcd_close(dev);
	}

	/* always return success */
	return 0;
}


/*
 * Functions
 */


int fcd_for_each(fcd_path_fn *fn, void *context)
{
	struct hid_device_info *devs, *current;
	int result = 0;

	/* enumerate FUNcube dongles */
	devs = hid_enumerate(FCD_USB_VID, FCD_USB_PID);
	current = devs;
	/* for each FUNcube dongle */
	while (NULL != current)
	{
		if (NULL != current->path)
		{
			/* call user function */
			result = fn(current->path, context);
			if (result)
			{
				/* abort on first error */
				break;
			}
		}
		/* proceed to next device */
		current = current->next;
	}
	hid_free_enumeration(devs);

	return result;
}


FCD * fcd_open(const char *path)
{
	FCD *dev;

	dev = malloc(sizeof(FCD));
	if (NULL != dev)
	{
		if (NULL == path)
		{
			/* get first available FUNcube dongle HID device */
			dev->hid_dev = hid_open(FCD_USB_VID, FCD_USB_PID, NULL);
		}
		else
		{
			/* open FUNcube dongle HID device by path */
			dev->hid_dev = hid_open_path(path);
		}
		if (NULL == dev->hid_dev)
		{
			/* could not open HID device */
			fcd_close(dev);
			dev = NULL;
		}
	}

	return dev;
}


void fcd_close(FCD *dev)
{
	if (NULL != dev)
	{
		hid_close(dev->hid_dev);
		free(dev);
	}
}


char * fcd_query(FCD *dev, char *str, int len)
{
	/* query device */
	len = fcd_get(dev, FCD_CMD_QUERY, str, len);
	if (!len)
	{
		/* query failed */
		return NULL;
	}
	/* ensure NULL-terminated string */
	str[len-1] = 0;
	return str;
}


int fcd_bl_set_address(FCD *dev, unsigned int addr)
{
	int result;
	uint32_t address;

	/* convert from native format */
	address = convert_le_u32(addr);

	/* set address */
	result = fcd_set(dev, FCD_CMD_SET_BYTE_ADDR, &address, sizeof(address));
	if (result != sizeof(address))
	{
		return -1;
	}

	return 0;
}


int fcd_bl_get_address_range(FCD *dev, unsigned int *start, unsigned int *end)
{
	int result;
	uint32_t range[2];

	/* get raw address range */
	result = fcd_get(dev, FCD_CMD_GET_BYTE_ADDR_RANGE, range, sizeof(range));
	if (result != sizeof(range))
	{
		return -1;
	}

	/* output in native format */
	if (NULL != start)
	{
		*start = convert_le_u32(range[0]);
	}
	if (NULL != end)
	{
		*end = convert_le_u32(range[1]);
	}

	return 0;
}


void fcd_reset_bootloader(void)
{
	unsigned char cmd = FCD_CMD_RESET_BOOTLOADER;
	fcd_for_each(fcd_reset, &cmd);
	ms_sleep(1000);
}


void fcd_reset_application(void)
{
	unsigned char cmd = FCD_CMD_RESET_APPLICATION;
	fcd_for_each(fcd_reset, &cmd);
	ms_sleep(1000);
}
