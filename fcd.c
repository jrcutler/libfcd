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
	/*! \brief HID device handle */
	hid_device *hid_dev;
};


/*
 * Functions
 */


FCD * fcd_open(const char *path)
{
	FCD *handle;

	handle = malloc(sizeof(FCD));
	if (NULL != handle)
	{
		if (NULL == path)
		{
			/* get first available FUNcube dongle HID device */
			handle->hid_dev = hid_open(FCD_USB_VID, FCD_USB_PID, NULL);
		}
		else
		{
			/* open FUNcube dongle HID device by path */
			handle->hid_dev = hid_open_path(path);
		}
		if (NULL == handle->hid_dev)
		{
			/* could not open HID device */
			fcd_close(handle);
			handle = NULL;
		}
	}

	return handle;
}


void fcd_close(FCD *handle)
{
	if (NULL != handle)
	{
		hid_close(handle->hid_dev);
		free(handle);
	}
}
