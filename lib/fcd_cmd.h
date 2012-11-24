/*! \file
 * \brief FUNcube dongle command definitions
 * \author Justin R. Cutler
 * \note Values, names, and descriptions are derived from \c FCHIDBL001.zip
 * and \c FCHID008.zip.
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
#ifndef FCD_CMD_H
# define FCD_CMD_H

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

/*! \brief Set 1 byte value command offset (added to \ref FCD_VALUE_ENUM) */
#define FCD_CMD_SET_VALUE_OFFSET   110

/*! \brief Get 1 byte value command offset (added to \ref FCD_VALUE_ENUM) */
#define FCD_CMD_GET_VALUE_OFFSET   150

#define FCD_CMD_I2C_SEND_BYTE      200
#define FCD_CMD_I2C_RECEIVE_BYTE   201

/*! \brief Reset to bootloader
 * \pre FUNcube dongle is in application mode
 * \post FUNcube dongle has reconnected and is in bootloader mode
 */
#define FCD_CMD_RESET_BOOTLOADER   255


#endif /* FCD_CMD_H */
