/*! \file
 * \brief FUNcube dongle interface implementation
 */
#ifdef HAVE_CONFIG
# include <config.h>
#endif
#include <stdlib.h> /* NULL, malloc, free */
#include "hidapi.h"
#include "fcd.h"


/*! \brief Implementation of \ref FCD */
struct FCD_impl
{
	/*! \brief HID device handle */
	hid_device *hid_dev;
};


FCD * fcd_open(void)
{
	FCD *handle;

	handle = malloc(sizeof(FCD));
	if (NULL != handle)
	{
		/* get first available FUNcube donge HID device */
		handle->hid_dev = hid_open(FCD_USB_VID, FCD_USB_PID, NULL);
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
