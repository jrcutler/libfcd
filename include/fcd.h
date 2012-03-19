/*! \file
 * \brief FUNcube dongle interface definition
 */
#ifndef FCD_H
# define FCD_H

/*! \def API
 * \brief Necessary platform-specific declarations needed for library functions
 */
#ifdef _WIN32
#define API __declspec(dllexport) __stdcall
#else
#define API
#endif

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
extern API int fcd_for_each(fcd_path_callback *fn, void *context);

/*!
 * \brief Open a FUNcube dongle device
 * \param[in] path USB path uniquely identifying device (or \c NULL for any)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 */
extern API FCD * fcd_open(const char *path);

/*!
 * \brief Close a FUNcube dongle device
 * \param[in,out] dev open \ref FCD (or \c NULL)
 * \retval non-NULL pointer to new open \ref FCD
 * \retval NULL     error
 * \post \p dev is no longer valid
 */
extern API void fcd_close(FCD *dev);

/*!
 * \brief Query a FUNcube dongle device
 * \param[in,out] dev open \ref FCD (or \c NULL)
 * \param[out]    str output buffer
 * \param         len length of output buffer
 * \retval NULL     error (\p str has not changed)
 * \retval non-NULL success (\p str)
 */
extern API char * fcd_query(FCD *dev, char *str, int len);

/*!
 * \brief Erase FUNcube dongle application code
 * \param[in,out] dev open \ref FCD
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_erase_application(FCD *dev);

/*!
 * \brief Set FUNcube dongle flash address
 * \param[in,out] dev  open \ref FCD
 * \param         addr new address
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_set_address(FCD *dev, unsigned int addr);

/*!
 * \brief Get FUNcube dongle flash address range
 * \param[in,out] dev   open \ref FCD
 * \param[out]    start start address pointer (NULL to ignore)
 * \param[out]    end   end address pointer (NULL to ignore)
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_get_address_range(FCD *dev, unsigned int *start,
	unsigned int *end);

/*!
 * \brief Read block from FUNcube dongle
 * \param[in,out] dev   open \ref FCD
 * \param[out]    block block data (48 bytes)
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_read_block(FCD *dev, unsigned char *block);

/*!
 * \brief Write block to FUNcube dongle
 * \param[in,out] dev   open \ref FCD
 * \param[in]     block block data (48 bytes)
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_write_block(FCD *dev, const unsigned char *block);

/*!
 * \brief Write new application to FUNcube dongle
 * \param[in,out] dev  open \ref FCD
 * \param[in]     data flash image data
 * \param         size size of \p data
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_flash_write(FCD *dev, const unsigned char *data,
	unsigned int size);

/*!
 * \brief Verify application from FUNcube dongle
 * \param[in,out] dev  open \ref FCD
 * \param[in]     data flash image data
 * \param         size size of \p data
 * \pre FUNcube dongle must be in bootloader
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_bl_flash_verify(FCD *dev, const unsigned char *data,
	unsigned int size);

/*!
 * \brief Set DC offset correction values
 * \param[in,out] dev  open \ref FCD
 * \param         i    DC I correction value (-32768..32767)
 * \param         q    DC Q correction value (-32768..32767)
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_set_dc_correction(FCD *dev, int i, int q);

/*!
 * \brief Get DC offset correction (I/Q) values
 * \param[in,out] dev  open \ref FCD
 * \param[out]    i    DC I correction value output
 * \param[out]    q    DC Q correction value output
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_get_dc_correction(FCD *dev, int *i, int *q);

/*!
 * \brief Set I/Q phase and gain balance values
 * \param[in,out] dev   open \ref FCD
 * \param         phase phase correction value (-32768..32767)
 * \param         gain  gain correction value (0..65535)
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_set_iq_correction(FCD *dev, int phase, unsigned int gain);

/*!
 * \brief Get I/Q phase and gain balance values
 * \param[in,out] dev   open \ref FCD
 * \param[out]    phase phase correction value output
 * \param[out]    gain  gain correction value output
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_get_iq_correction(FCD *dev, int *phase, unsigned int *gain);

/*!
 * \brief Set frequency (in Hz)
 * \param[in,out] dev  open \ref FCD
 * \param         freq frequency (in Hz)
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_set_frequency_Hz(FCD *dev, unsigned int freq);

/*!
 * \brief Get frequency (in Hz)
 * \param[in,out] dev  open \ref FCD
 * \param[out]    freq frequency output (in Hz)
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_get_frequency_Hz(FCD *dev, unsigned int *freq);

/*!
 * \brief Reset all FUNcube dongles to bootloader
 * \param delay_ms delay time after reset (in ms)
 */
extern API void fcd_reset_bootloader(unsigned int delay_ms);

/*!
 * \brief Reset all FUNcube dongles to application
 * \param delay_ms delay time after reset (in ms)
 */
extern API void fcd_reset_application(unsigned int delay_ms);


# ifdef __cplusplus
}
# endif

#endif /* FCD_H */
