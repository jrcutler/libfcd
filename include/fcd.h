/*! \file
 * \brief FUNcube dongle interface definition
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

/*!
 * \brief FUNcube dongle get/set 1-byte value identifiers
 * \note Values, names, and descriptions are derived from \c FCHID008.zip.
 */
typedef enum
{
	/*! \brief LNA (front-end) gain (\ref FCD_TUNER_LNA_GAIN_ENUM) */
	FCD_VALUE_LNA_GAIN = 0,
	/*! \brief LNA enhance mode (\ref FCD_TUNER_LNA_ENHANCE_ENUM) */
	FCD_VALUE_LNA_ENHANCE,
	/*! \brief RF band selection (\ref FCD_TUNER_BAND_ENUM) */
	FCD_VALUE_BAND,
	/*! \brief RF filter selection (\ref FCD_TUNER_RF_FILTER_ENUM) */
	FCD_VALUE_RF_FILTER,
	/*! \brief Mixer gain (\ref FCD_TUNER_MIXER_GAIN_ENUM) */
	FCD_VALUE_MIXER_GAIN,
	/*! \brief Bias current (\ref FCD_TUNER_BIAS_CURRENT_ENUM) */
	FCD_VALUE_BIAS_CURRENT,
	/*! \brief Mixer filter selection (\ref FCD_TUNER_MIXER_FILTER_ENUM) */
	FCD_VALUE_MIXER_FILTER,
	/*! \brief IF amplifier 1 gain (\ref FCD_TUNER_IF_GAIN1_ENUM) */
	FCD_VALUE_IF_GAIN1,
	/*! \brief IF gain mode selection (\ref FCD_TUNER_IF_GAIN_MODE_ENUM) */
	FCD_VALUE_IF_GAIN_MODE,
	/*! \brief IF RC filter selection (\ref FCD_TUNER_IF_RC_FILTER_ENUM) */
	FCD_VALUE_IF_RC_FILTER,
	/*! \brief IF amplifier 2 gain (\ref FCD_TUNER_IF_GAIN2_ENUM) */
	FCD_VALUE_IF_GAIN2,
	/*! \brief IF amplifier 3 gain (\ref FCD_TUNER_IF_GAIN3_ENUM) */
	FCD_VALUE_IF_GAIN3,
	/*! \brief IF filter selection (\ref FCD_TUNER_IF_FILTER_ENUM) */
	FCD_VALUE_IF_FILTER,
	/*! \brief IF amplifier 4 gain (\ref FCD_TUNER_IF_GAIN4_ENUM) */
	FCD_VALUE_IF_GAIN4,
	/*! \brief IF amplifier 5 gain (\ref FCD_TUNER_IF_GAIN5_ENUM) */
	FCD_VALUE_IF_GAIN5,
	/*! \brief IF amplifier 6 gain (\ref FCD_TUNER_IF_GAIN6_ENUM) */
	FCD_VALUE_IF_GAIN6,
	/*! \brief Bias tee control (1 on, 0 off) */
	FCD_VALUE_BIAS_TEE,
	/*! \brief Undefined value identifier (for range checking) */
	FCD_VALUE_UNDEFINED
} FCD_VALUE_ENUM;


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
 * \brief Set bias tee state
 * \param[in,out] dev   open \ref FCD
 * \param         state state (0 off, 1 on)
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_set_bias_tee(FCD *dev, unsigned char state);

/*!
 * \brief Get bias tee state
 * \param[in,out] dev   open \ref FCD
 * \param[out]    state state output (0 off, 1 on)
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_get_bias_tee(FCD *dev, unsigned char *state);

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
 * \brief Set 1-byte value
 * \param[in,out] dev   open \ref FCD
 * \param         id    value identifier
 * \param         value new value
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_set_value(FCD *dev, FCD_VALUE_ENUM id, unsigned char value);

/*!
 * \brief Get 1-byte value
 * \param[in,out] dev   open \ref FCD
 * \param         id    value identifier
 * \param[out]    value new value
 * \retval 0     success
 * \retval non-0 failure
 */
extern API int fcd_get_value(FCD *dev, FCD_VALUE_ENUM id, unsigned char *value);

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
