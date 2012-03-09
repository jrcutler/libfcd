/*! \file
 * \brief FUNcube dongle interface definition
 */
#ifndef FCD_H
# define FCD_H

# ifdef __cplusplus
extern "C"
{
# endif


/*
 * Defines
 */

/*! FUNcube dongle USB vendor ID */
#define FCD_USB_VID 0x04d8
/*! FUNcube dongle USB product ID */
#define FCD_USB_PID 0xfb56


/*
 * Types
 */

/* Forward declaration of opaque FUNcube dongle structure */
struct FCD_impl;
/*! \brief Opaque FUNcube dongle handle */
typedef struct FCD_impl FCD;

/*!
 * \brief FUNcube dongle path callback function
 * \param[in]     path    path to a FUNcube dongle
 * \param[in,out] context user context pointer
 * \retval 0     success
 * \retval non-0 failure
 */
typedef int (fcd_path_callback)(const char *path, void *context);


/*
 * Functions
 */

/*!
 * \brief Call a user-provided path handler for all FUNcube dongles present
 * \param         fn      callback function
 * \param[in,out] context context pointer
 * \retval 0     success
 * \retval non-0 failure
 * \note Terminates on the first call to \p fn that fails.
 */
extern int fcd_for_each(fcd_path_callback *fn, void *context);

/*!
 * \brief Open a FUNcube dongle device
 * \param[in] path USB path uniquely identifying device (or \c NULL for any)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 */
extern FCD * fcd_open(const char *path);

/*!
 * \brief Close a FUNcube dongle device
 * \param[in,out] dev open \ref FCD (or \c NULL)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 * \post \p dev is no longer valid
 */
extern void fcd_close(FCD *dev);

/*!
 * \brief Query a FUNcube dongle device
 * \param[in,out] dev open \ref FCD (or \c NULL)
 * \param[out]    str output buffer
 * \param         len length of output buffer
 * \retval NULL     error (\p str has not changed)
 * \retval non-NULL success (\p str)
 */
extern char * fcd_query(FCD *dev, char *str, int len);

/*!
 * \brief Erase FUNcube dongle application code
 * \param[in,out] dev open \ref FCD
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern int fcd_bl_erase_application(FCD *dev);

/*!
 * \brief Set FUNcube dongle flash address
 * \param[in,out] dev  open \ref FCD
 * \param         addr new address
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern int fcd_bl_set_address(FCD *dev, unsigned int addr);

/*!
 * \brief Get FUNcube dongle flash address range
 * \param[in,out] dev   open \ref FCD
 * \param[out]    start start address pointer (NULL to ignore)
 * \param[out]    end   end address pointer (NULL to ignore)
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern int fcd_bl_get_address_range(FCD *dev, unsigned int *start,
	unsigned int *end);

/*!
 * \brief Read block from FUNcube dongle
 * \param[in,out] dev   open \ref FCD
 * \param[out]    block block data (48 bytes)
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern int fcd_bl_read_block(FCD *dev, unsigned char *block);

/*!
 * \brief Write block to FUNcube dongle
 * \param[in,out] dev   open \ref FCD
 * \param[in]     block block data (48 bytes)
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern int fcd_bl_write_block(FCD *dev, const unsigned char *block);

/*!
 * \brief Reset to bootloader
 */
extern void fcd_reset_bootloader(void);

/*!
 * \brief Reset to application
 */
extern void fcd_reset_application(void);


# ifdef __cplusplus
}
# endif

#endif /* FCD_H */
