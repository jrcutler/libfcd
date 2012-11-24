/*! \file
 * \brief FUNcube dongle bootloader interface implementation
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

#include <stdlib.h> /* NULL */
#include <string.h> /* memcmp */
#include "fcd.h" /* FCD */
#include "fcd_cmd.h" /* FCD_CMD_* */
#include "fcd_common.h"


API int fcd_bl_erase_application(FCD *dev)
{
	return fcd_set(dev, FCD_CMD_ERASE_APPLICATION, NULL, 0);
}


API int fcd_bl_set_address(FCD *dev, unsigned int addr)
{
	uint32_t address;

	/* convert from native format */
	address = convert_le_u32(addr);

	/* set address */
	return fcd_set(dev, FCD_CMD_SET_BYTE_ADDR, &address, sizeof(address));
}


API int fcd_bl_get_address_range(FCD *dev, unsigned int *start,
	unsigned int *end)
{
	int result;
	uint32_t range[2];

	/* get raw address range */
	result = fcd_get(dev, FCD_CMD_GET_BYTE_ADDR_RANGE, range, sizeof(range));
	if (result)
	{
		return result;
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
	return fcd_get(dev, FCD_CMD_READ_BLOCK, block, 48);
}


API int fcd_bl_write_block(FCD *dev, const unsigned char *block)
{
	/* use 1 byte skip, as write block data starts at 3 for unknown reason */
	return fcd_io(dev, FCD_CMD_WRITE_BLOCK, 1, block, 48, NULL, 0);
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

