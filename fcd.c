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


/*!
 * \brief Convert 16-bit integer to/from little-endian
 * \param v value
 * \returns \p v on little-endian systems or byte-swapped \p v on big-endian
 * systems
 */
inline uint16_t convert_le_u16(uint16_t v)
{
#ifdef WORDS_BIGENDIAN
	/* big endian */
	return ((v & 0xff00) >>  8) |
	       ((v & 0x00ff) <<  8);
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


/*! \copydetails fcd_path_callback
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


int fcd_for_each(fcd_path_callback *fn, void *context)
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
	if (len <= 0)
	{
		/* query failed */
		return NULL;
	}
	/* ensure NULL-terminated string */
	str[len-1] = 0;
	return str;
}


int fcd_bl_erase_application(FCD *dev)
{
	return fcd_set(dev, FCD_CMD_ERASE_APPLICATION, NULL, 0);
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


int fcd_bl_read_block(FCD *dev, unsigned char *block)
{
	if (fcd_get(dev, FCD_CMD_READ_BLOCK, block, 48) != 48)
	{
		return -1;
	}
	return 0;
}


int fcd_bl_write_block(FCD *dev, const unsigned char *block)
{
	/* use 1 byte skip, as write block data starts at 3 for unknown reason */
	if (fcd_set_skip(dev, FCD_CMD_WRITE_BLOCK, block, 48, 1) != 48)
	{
		return -1;
	}
	return 0;
}


int fcd_bl_flash_write(FCD *dev, const unsigned char *data, unsigned int size)
{
	unsigned int start, end, addr;
	/* get flash range */
	if (fcd_bl_get_address_range(dev, &start, &end))
	{
		return -1;
	}
	/* sanity check range */
	if (start >= end || (end - start) % 48)
	{
		return -2;
	}
	/* ensure firmware image is large enough */
	if (end > size)
	{
		return -3;
	}
	/* set address to start of flash */
	if (fcd_bl_set_address(dev, start))
	{
		return -4;
	}
	/* write flash (in 48-byte blocks) */
	for (addr = start; addr < end; addr += 48)
	{
		if (fcd_bl_write_block(dev, data+addr))
		{
			return -5;
		}
	}
	return 0;
}


int fcd_bl_flash_verify(FCD *dev, const unsigned char *data, unsigned int size)
{
	unsigned int start, end, addr;
	/* get flash range */
	if (fcd_bl_get_address_range(dev, &start, &end))
	{
		return -1;
	}
	/* sanity check range */
	if (start >= end || (end - start) % 48)
	{
		return -2;
	}
	/* ensure firmware image is large enough */
	if (end > size)
	{
		return -3;
	}
	/* set address to start of flash */
	if (fcd_bl_set_address(dev, start))
	{
		return -4;
	}
	/* verify flash (in 48-byte blocks) */
	for (addr = start; addr < end; addr += 48)
	{
		unsigned char buffer[48];
		if (fcd_bl_read_block(dev, buffer))
		{
			return -6;
		}
		if (memcmp(buffer, data+addr, sizeof(buffer)))
		{
			return 1;
		}
	}
	return 0;
}


int fcd_set_dc_correction(FCD *dev, int i, int q)
{
	int16_t correction[2];

	correction[0] = i;
	correction[1] = q;
	if ((i != correction[0]) || (q != correction[1]))
	{
		/* value out of range */
		return -1;
	}
	correction[0] = (int16_t) convert_le_u16((uint16_t) correction[0]);
	correction[1] = (int16_t) convert_le_u16((uint16_t) correction[1]);

	if (fcd_set(dev, FCD_CMD_SET_DC_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
	}
	return 0;
}


int fcd_get_dc_correction(FCD *dev, int *i, int *q)
{
	int16_t correction[2];

	if (fcd_get(dev, FCD_CMD_GET_DC_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
	}

	if (NULL != i)
	{
		*i = (int16_t) convert_le_u16((uint16_t) correction[0]);
	}
	if (NULL != q)
	{
		*q = (int16_t) convert_le_u16((uint16_t) correction[1]);
	}

	return 0;
}


int fcd_set_iq_correction(FCD *dev, int phase, unsigned int gain)
{
	struct {
		int16_t phase;
		uint16_t gain;
	} correction;

	correction.phase = phase;
	correction.gain = gain;
	if ((phase != correction.phase) || (gain != correction.gain))
	{
		/* value out of range */
		return -1;
	}
	correction.phase = (int16_t) convert_le_u16((uint16_t) correction.phase);
	correction.gain = (int16_t) convert_le_u16((uint16_t) correction.gain);

	if (fcd_set(dev, FCD_CMD_SET_IQ_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
	}
	return 0;
}


int fcd_get_iq_correction(FCD *dev, int *phase, unsigned int *gain)
{
	struct {
		int16_t phase;
		uint16_t gain;
	} correction;

	if (fcd_get(dev, FCD_CMD_GET_IQ_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
	}

	if (NULL != phase)
	{
		*phase = (int16_t) convert_le_u16((uint16_t) correction.phase);
	}
	if (NULL != gain)
	{
		*gain = convert_le_u16((uint16_t) correction.gain);
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
