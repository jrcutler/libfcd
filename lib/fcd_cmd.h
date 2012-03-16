/*! \file
 * \brief FUNcube dongle command definitions
 * \note Values, names, and descriptions are derived from \c FCHIDBL001.zip
 * and \c FCHID008.zip.
 */
#ifndef FCD_CMD_H
# define FCD_CMD_H

/*
 * Types
 */


/*
 * Global commands
 */

/*! \brief Returns string with "FCDAPP version" */
#define FCD_CMD_QUERY                1

/*
 * Bootloader Commands
 */

/*! \brief Reset to application
 * \pre FUNcube dongle is in bootloader mode
 * \post FUNcube dongle has reconnected and is in application mode
 */
#define FCD_CMD_RESET_APPLICATION    8
/*! \brief Erase application
 * \pre FUNcube dongle is in bootloader mode
 */
#define FCD_CMD_ERASE_APPLICATION   24
/*! \brief Set application byte address
 * \pre FUNcube dongle is in bootloader mode
 */
#define FCD_CMD_SET_BYTE_ADDR       25
/*! \brief Get application byte address range
 * \pre FUNcube dongle is in bootloader mode
 */
#define FCD_CMD_GET_BYTE_ADDR_RANGE 26
/*! \brief Write application block
 * \note This command does not follow the normal layout. Instead, the data
 * segment is offset by one byte (via fcd_set_skip()).
 * \pre FUNcube dongle is in bootloader mode
 */
#define FCD_CMD_WRITE_BLOCK         27
/*! \brief Read application block
 * \pre FUNcube dongle is in bootloader mode
 */
#define FCD_CMD_READ_BLOCK          28

/*
 * Application Commands
 */

/*! \brief Send with 3 byte unsigned little endian frequency in kHz.
 */
#define FCD_CMD_SET_FREQUENCY      100
/*! \brief Send with 4 byte unsigned little endian frequency in Hz, returns with actual frequency set in Hz
 */
#define FCD_CMD_SET_FREQUENCY_HZ   101
/*! \brief Returns 4 byte unsigned little endian frequency in Hz.
 */
#define FCD_CMD_GET_FREQUENCY_HZ   102

/*! \brief Returns 1 byte unsigned IF RSSI, -35dBm ~=0, -10dBm ~=70.
 */
#define FCD_CMD_GET_IF_RSSI        104
/*! \brief Returns 1 bit, true if locked
 */
#define FCD_CMD_GET_PLL_LOCK       105

/*! \brief Send with 2 byte unsigned I DC correction followed by 2 byte unsigned Q DC correction. 32768 is the default centre value.
 */
#define FCD_CMD_SET_DC_CORR        106
/*! \brief Returns 2 byte unsigned I DC correction followed by 2 byte unsigned Q DC correction. 32768 is the default centre value.
 */
#define FCD_CMD_GET_DC_CORR        107
/*! \brief Send with 2 byte signed phase correction followed by 2 byte unsigned gain correction. 0 is the default centre value for phase correction, 32768 is the default centre value for gain.
 */
#define FCD_CMD_SET_IQ_CORR        108
/*! \brief Returns 2 byte signed phase correction followed by 2 byte unsigned gain correction. 0 is the default centre value for phase correction, 32768 is the default centre value for gain.
 */
#define FCD_CMD_GET_IQ_CORR        109

/* Send a 1 byte value, see enums for reference */
#define FCD_CMD_SET_LNA_GAIN       110
#define FCD_CMD_SET_LNA_ENHANCE    111
#define FCD_CMD_SET_BAND           112
#define FCD_CMD_SET_RF_FILTER      113
#define FCD_CMD_SET_MIXER_GAIN     114
#define FCD_CMD_SET_BIAS_CURRENT   115
#define FCD_CMD_SET_MIXER_FILTER   116
#define FCD_CMD_SET_IF_GAIN1       117
#define FCD_CMD_SET_IF_GAIN_MODE   118
#define FCD_CMD_SET_IF_RC_FILTER   119
#define FCD_CMD_SET_IF_GAIN2       120
#define FCD_CMD_SET_IF_GAIN3       121
#define FCD_CMD_SET_IF_FILTER      122
#define FCD_CMD_SET_IF_GAIN4       123
#define FCD_CMD_SET_IF_GAIN5       124
#define FCD_CMD_SET_IF_GAIN6       125
/*! \brief Send with a byte, 1 on, 0 off
 */
#define FCD_CMD_SET_BIAS_TEE       126

/* Retrieve a 1 byte value, see enums for reference */
#define FCD_CMD_GET_LNA_GAIN       150
#define FCD_CMD_GET_LNA_ENHANCE    151
#define FCD_CMD_GET_BAND           152
#define FCD_CMD_GET_RF_FILTER      153
#define FCD_CMD_GET_MIXER_GAIN     154
#define FCD_CMD_GET_BIAS_CURRENT   155
#define FCD_CMD_GET_MIXER_FILTER   156
#define FCD_CMD_GET_IF_GAIN1       157
#define FCD_CMD_GET_IF_GAIN_MODE   158
#define FCD_CMD_GET_IF_RC_FILTER   159
#define FCD_CMD_GET_IF_GAIN2       160
#define FCD_CMD_GET_IF_GAIN3       161
#define FCD_CMD_GET_IF_FILTER      162
#define FCD_CMD_GET_IF_GAIN4       163
#define FCD_CMD_GET_IF_GAIN5       164
#define FCD_CMD_GET_IF_GAIN6       165
/*! \brief Returns 1 bit, true if on
 */
#define FCD_CMD_GET_BIAS_TEE       166

#define FCD_CMD_I2C_SEND_BYTE      200
#define FCD_CMD_I2C_RECEIVE_BYTE   201

/*! \brief Reset to bootloader
 * \pre FUNcube dongle is in application mode
 * \post FUNcube dongle has reconnected and is in bootloader mode
 */
#define FCD_CMD_RESET_BOOTLOADER   255


#endif /* FCD_CMD_H */
