/*! \file
 * \brief FUNcube dongle interface implementation
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif

#include <stdlib.h> /* NULL, malloc, free */
#include <string.h> /* memset, memcpy */
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


/*! \brief Perform a set command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[in]     data input data pointer
 * \param         len  input data length
 * \returns length of sent data or -1 on error
 */
int fcd_set(FCD *dev, unsigned char cmd, const void *data, unsigned char len)
{
	fcd_buffer buffer;
	int result = -1;

	/*! \todo validate cmd */
	/* do not allow NULL pointer for non-trivial set */
	if (len && (NULL == data)) return -1;
	/* trim request length as needed */
	if (len > sizeof(buffer.command.data))
	{
		len = sizeof(buffer.command.data);
	}

	/* send set request */
	buffer.command.report_id = 0;
	buffer.command.command = cmd;
	memcpy(&buffer.command.data, data, len);
	if (hid_write(dev->hid_dev, (unsigned char *)&buffer, len+2) == len+2)
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
