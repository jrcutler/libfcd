/*! \file
 * \brief FUNcube dongle interface implementation
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif

#include <stdlib.h> /* NULL, malloc, free */
#include "hidapi.h" /* hid_dev, hid_open, hid_close */
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
 * Functions
 */


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
