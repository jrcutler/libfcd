/*! \file
 * \brief FUNcube dongle common interface implementation
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h> /* E*, errno */
#include <stdlib.h> /* NULL, malloc, free */
#include <string.h> /* memset, memcpy */
#ifdef HAVE_USLEEP
# ifdef HAVE_UNISTD_H
#  include <unistd.h> /* usleep */
# endif
#endif
#include "fcd.h" /* FCD */
#include "fcd_cmd.h" /* FCD_CMD_* */
#include "fcd_common.h"


void ms_sleep(unsigned int ms)
{
#ifdef HAVE_USLEEP
	unsigned int t;
	for (t = ms; t >= 1000; t -= 1000)
	{
		usleep(500*SHORT_SLEEP_FACTOR);
		usleep(500*SHORT_SLEEP_FACTOR);
	}
	usleep(t*SHORT_SLEEP_FACTOR);
#elif defined(HAVE_NAP)
	nap(ms*SHORT_SLEEP_FACTOR);
#endif
}


int fcd_io(FCD *dev, unsigned char cmd, unsigned char iskip, const void *idata,
	unsigned char ilen, void *odata, unsigned char olen)
{
	fcd_buffer buffer;
	int result = -1;

	/*! \todo validate cmd */
	/* do not allow NULL pointer for non-trivial I/O */
	if ((ilen && (NULL == idata)) || (olen && (NULL == odata)))
	{
		errno = EFAULT;
		return -1;
	}
	/* trim request lengths as needed */
	if (ilen > sizeof(buffer.command.data) - iskip)
	{
		ilen = sizeof(buffer.command.data) - iskip;
	}
	if (olen > sizeof(buffer.response.data))
	{
		olen = sizeof(buffer.response.data);
	}

	/* send request */
	buffer.command.report_id = 0;
	buffer.command.command = cmd;
	/* pad skipped input byte(s) */
	memset(&buffer.command.data, 0, iskip);
	/* copy in data */
	memcpy(&(buffer.command.data[iskip]), idata, ilen);
	/*! \bug Windows: hid_write() always returns 65 */
	if (hid_write(dev->hid_dev, (unsigned char *)&buffer, ilen+2+iskip) >=
		ilen+2+iskip)
	{
		/* receive response */
		/*! \bug Windows: hid_read() always returns 64 */
		if (hid_read(dev->hid_dev, (unsigned char *)&buffer, olen+2) >= olen+2)
		{
			/* validate response */
			if ((buffer.response.command == cmd) &&
				(buffer.response.status == 1))
			{
				memcpy(odata, &buffer.response.data, olen);
				result = 0;
			}
		}
	}

	if (result)
	{
		errno = EIO;
	}
	return result;
}


int fcd_get(FCD *dev, unsigned char cmd, void *data, unsigned char len)
{
	if (!fcd_io(dev, cmd, 0, NULL, 0, data, len))
	{
		return len;
	}
	return -1;
}


int fcd_set_skip(FCD *dev, unsigned char cmd, const void *data,
	unsigned char len, unsigned char skip)
{
	if (!fcd_io(dev, cmd, skip, data, len, NULL, 0))
	{
		return len;
	}
	return -1;
}


int fcd_set(FCD *dev, unsigned char cmd, const void *data, unsigned char len)
{
	return fcd_set_skip(dev, cmd, data, len, 0);
}


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


API int fcd_for_each(fcd_path_callback *fn, void *context)
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


API FCD * fcd_open(const char *path)
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
			errno = EINVAL;
		}
	}

	return dev;
}


API void fcd_close(FCD *dev)
{
	if (NULL != dev)
	{
		hid_close(dev->hid_dev);
		free(dev);
	}
}


API char * fcd_query(FCD *dev, char *str, int len)
{
	/* query device */
	len = fcd_get(dev, FCD_CMD_QUERY, str, len);
	if (len <= 0)
	{
		/* query failed */
		return NULL;
	}
	/* ensure NULL-terminated string */
	str[len-1] = 0;
	return str;
}


API void fcd_reset_bootloader(unsigned int delay_ms)
{
	unsigned char cmd = FCD_CMD_RESET_BOOTLOADER;
	fcd_for_each(fcd_reset, &cmd);
	ms_sleep(delay_ms);
}


API void fcd_reset_application(unsigned int delay_ms)
{
	unsigned char cmd = FCD_CMD_RESET_APPLICATION;
	fcd_for_each(fcd_reset, &cmd);
	ms_sleep(delay_ms);
}
