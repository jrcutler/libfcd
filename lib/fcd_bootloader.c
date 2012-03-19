/*! \file
 * \brief FUNcube dongle bootloader interface implementation
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h> /* NULL */
#include <string.h> /* memcmp */
#include "fcd.h" /* FCD */
#include "fcd_cmd.h" /* FCD_CMD_* */
#include "fcd_common.h"


/*
 * Functions
 */


API int fcd_bl_erase_application(FCD *dev)
{
	return fcd_set(dev, FCD_CMD_ERASE_APPLICATION, NULL, 0);
}


API int fcd_bl_set_address(FCD *dev, unsigned int addr)
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


API int fcd_bl_get_address_range(FCD *dev, unsigned int *start,
	unsigned int *end)
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


API int fcd_bl_read_block(FCD *dev, unsigned char *block)
{
	if (fcd_get(dev, FCD_CMD_READ_BLOCK, block, 48) != 48)
	{
		return -1;
	}
	return 0;
}


API int fcd_bl_write_block(FCD *dev, const unsigned char *block)
{
	/* use 1 byte skip, as write block data starts at 3 for unknown reason */
	if (fcd_set_skip(dev, FCD_CMD_WRITE_BLOCK, block, 48, 1) != 48)
	{
		return -1;
	}
	return 0;
}


API int fcd_bl_flash_write(FCD *dev, const unsigned char *data,
	unsigned int size)
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


API int fcd_bl_flash_verify(FCD *dev, const unsigned char *data,
	unsigned int size)
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

