/*! \file
 * \brief FUNcube dongle common internal interface definition
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
#ifndef FCD_COMMON_H
# define FCD_COMMON_H

# ifdef HAVE_STDINT_H
#  include <stdint.h> /* [u]int*_t */
# endif
# include "hidapi/hidapi.h" /* hid_* */

# ifdef __cplusplus
extern "C"
{
# endif


/*
 * Types
 */


/*! \brief Implementation of \ref FCD */
struct FCD_impl
{
	/*! \brief HID device path */
	char *path;
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
 * Inline Functions
 */


/*!
 * \brief Convert 32-bit integer to/from little-endian
 * \param v value
 * \returns \p v on little-endian systems or byte-swapped \p v on big-endian
 * systems
 */
static inline uint32_t convert_le_u32(uint32_t v)
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
static inline uint16_t convert_le_u16(uint16_t v)
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


/*
 * Functions
 */


/*!
 * \brief Sleep for some number of milliseconds
 * \param ms number of milliseconds to sleep
 * \note POSIX allows for usleep to return without delay for sleep >= 1 second
 */
void ms_sleep(unsigned int ms);

/*! \brief Perform an I/O command
 * \param[in,out] dev   open \ref FCD
 * \param         cmd   command ID
 * \param         iskip number of input data bytes to skip (normally 0)
 * \param[in]     idata input data pointer
 * \param         ilen  input data length
 * \param[out]    odata output data pointer
 * \param         olen  output data length
 * \retval 0     success
 * \retval non-0 failure
 */
int fcd_io(FCD *dev, unsigned char cmd, unsigned char iskip, const void *idata,
	unsigned char ilen, void *odata, unsigned char olen);

/*! \brief Perform a get command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[out]    data output data pointer
 * \param         len  output data length
 * \retval 0     success
 * \retval non-0 failure
 */
int fcd_get(FCD *dev, unsigned char cmd, void *data, unsigned char len);

/*! \brief Perform a set command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[in]     data input data pointer
 * \param         len  input data length
 * \retval 0     success
 * \retval non-0 failure
 */
int fcd_set(FCD *dev, unsigned char cmd, const void *data, unsigned char len);

/*! \copydetails fcd_path_callback
 * \brief Reset FUNcube dongle
 * \note \p context points to specified reset command
 */
int fcd_reset(const char *path, void *context);


# ifdef __cplusplus
}
# endif

#endif /* FCD_COMMON_H */
