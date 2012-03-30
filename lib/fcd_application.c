/*! \file
 * \brief FUNcube dongle application interface implementation
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <errno.h> /* E*, errno */
#include <stdlib.h> /* NULL */
#include "fcd.h" /* FCD */
#include "fcd_cmd.h" /* FCD_CMD_* */
#include "fcd_common.h"


API int fcd_set_dc_correction(FCD *dev, int i, int q)
{
	int16_t correction[2];

	correction[0] = i;
	correction[1] = q;
	if ((i != correction[0]) || (q != correction[1]))
	{
		/* value out of range */
		errno = EOVERFLOW;
		return -1;
	}
	correction[0] = (int16_t) convert_le_u16((uint16_t) correction[0]);
	correction[1] = (int16_t) convert_le_u16((uint16_t) correction[1]);

	return fcd_set(dev, FCD_CMD_SET_DC_CORR, &correction, sizeof(correction));
}


API int fcd_get_dc_correction(FCD *dev, int *i, int *q)
{
	int16_t correction[2];
	int result;

	result = fcd_get(dev, FCD_CMD_GET_DC_CORR, &correction, sizeof(correction));
	if (result)
	{
		return result;
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


API int fcd_set_iq_correction(FCD *dev, int phase, unsigned int gain)
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
		errno = EOVERFLOW;
		return -1;
	}
	correction.phase = (int16_t) convert_le_u16((uint16_t) correction.phase);
	correction.gain = (int16_t) convert_le_u16((uint16_t) correction.gain);

	return fcd_set(dev, FCD_CMD_SET_IQ_CORR, &correction, sizeof(correction));
}


API int fcd_get_iq_correction(FCD *dev, int *phase, unsigned int *gain)
{
	struct {
		int16_t phase;
		uint16_t gain;
	} correction;
	int result;

	result = fcd_get(dev, FCD_CMD_GET_IQ_CORR, &correction, sizeof(correction));
	if (result)
	{
		return result;
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


API int fcd_set_bias_tee(FCD *dev, unsigned char state)
{
	unsigned char bias = state ? 1 : 0;
	return fcd_set_value(dev, FCD_VALUE_BIAS_TEE, bias);
}


API int fcd_get_bias_tee(FCD *dev, unsigned char *state)
{
	return fcd_get_value(dev, FCD_VALUE_BIAS_TEE, state);
}


API int fcd_set_frequency_Hz(FCD *dev, unsigned int freq)
{
	uint32_t fHz = convert_le_u32(freq);
	return fcd_set(dev, FCD_CMD_SET_FREQUENCY_HZ, &fHz, sizeof(fHz));
}


API int fcd_get_frequency_Hz(FCD *dev, unsigned int *freq)
{
	uint32_t fHz;
	int result;

	result = fcd_get(dev, FCD_CMD_GET_FREQUENCY_HZ, &fHz, sizeof(fHz));
	if (result)
	{
		return result;
	}

	*freq = convert_le_u32(fHz);
	return 0;
}


API int fcd_set_value(FCD *dev, FCD_VALUE_ENUM id, unsigned char value)
{
	if (id >= FCD_VALUE_UNDEFINED)
	{
		errno = EINVAL;
		return -1;
	}
	return fcd_set(dev, FCD_CMD_SET_VALUE_OFFSET + id, &value, 1);
}


API int fcd_get_value(FCD *dev, FCD_VALUE_ENUM id, unsigned char *value)
{
	if (id >= FCD_VALUE_UNDEFINED)
	{
		errno = EINVAL;
		return -1;
	}
	return fcd_get(dev, FCD_CMD_GET_VALUE_OFFSET + id, &value, 1);
}
