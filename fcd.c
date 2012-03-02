/*! \file
 * \brief FUNcube dongle interface implementation
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif

#include <stdlib.h> /* NULL, malloc, free */
#include <string.h> /* memset, memcpy */
#include "hidapi.h" /* hid_* */
#include "fcd.h" /* FCD */


/*
 * Types
 */


/*! \brief Implementation of \ref FCD */
struct FCD_impl
{
	/*! \brief HID device */
	hid_device *hid_dev;
};


/*
 * Private Functions
 */


int fcd_get(FCD *dev, unsigned char cmd, void *data, unsigned char len)
{
	unsigned char buffer[65];
	int result = -1;

	/*! \TODO validate cmd */
	if (len && (NULL == data)) return -1;
	if (len > 62) return -1;

	memset(buffer, 0, sizeof(buffer));
	buffer[1] = cmd;
	if (hid_write(dev->hid_dev, buffer, 2) == 2)
	{
		buffer[1] = 0;
		if (hid_read(dev->hid_dev, buffer, len+2) == len+2)
		{
			if ((buffer[0] == cmd) && (buffer[1] == 1))
			{
				memcpy(data, buffer+2, len);
				result = len;
			}
		}
	}

	return result;
}


int fcd_set(FCD *dev, unsigned char cmd, const void *data, unsigned char len)
{
	unsigned char buffer[65];
	int result = -1;

	/*! \TODO validate cmd */
	if (len && (NULL == data)) return -1;
	if (len > 62) return -1;

	buffer[0] = 0;
	buffer[1] = cmd;
	memcpy(buffer+2, data, len);
	if (hid_write(dev->hid_dev, buffer, len+2) == len+2)
	{
		if (hid_read(dev->hid_dev, buffer, 2) == 2)
		{
			if ((buffer[0] == cmd) && (buffer[1] == 1))
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
