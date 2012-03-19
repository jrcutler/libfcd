/*! \file
 * \brief FUNcube dongle common internal interface definition
 */
#ifndef FCD_COMMON_H
# define FCD_COMMON_H

# ifdef HAVE_STDINT_H
#  include <stdint.h> /* [u]int*_t */
# endif
#include "hidapi/hidapi.h" /* hid_* */

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

/*! \brief Perform a get command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[out]    data output data pointer
 * \param         len  output data length
 * \returns length of received data or -1 on error
 */
int fcd_get(FCD *dev, unsigned char cmd, void *data, unsigned char len);

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
	unsigned char len, unsigned char skip);

/*! \brief Perform a set command
 * \param[in,out] dev  open \ref FCD
 * \param         cmd  command ID
 * \param[in]     data input data pointer
 * \param         len  input data length
 * \returns length of sent data or -1 on error
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

#endif /* FCD_H */
