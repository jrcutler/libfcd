/*! \file
 * \brief FUNcube dongle application interface implementation
 */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h> /* NULL */
#include "fcd.h" /* FCD */
#include "fcd_cmd.h" /* FCD_CMD_* */
#include "fcd_common.h"


/*
 * Functions
 */


API int fcd_set_dc_correction(FCD *dev, int i, int q)
{
	int16_t correction[2];

	correction[0] = i;
	correction[1] = q;
	if ((i != correction[0]) || (q != correction[1]))
	{
		/* value out of range */
		return -1;
	}
	correction[0] = (int16_t) convert_le_u16((uint16_t) correction[0]);
	correction[1] = (int16_t) convert_le_u16((uint16_t) correction[1]);

	if (fcd_set(dev, FCD_CMD_SET_DC_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
	}
	return 0;
}


API int fcd_get_dc_correction(FCD *dev, int *i, int *q)
{
	int16_t correction[2];

	if (fcd_get(dev, FCD_CMD_GET_DC_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
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
		return -1;
	}
	correction.phase = (int16_t) convert_le_u16((uint16_t) correction.phase);
	correction.gain = (int16_t) convert_le_u16((uint16_t) correction.gain);

	if (fcd_set(dev, FCD_CMD_SET_IQ_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
	}
	return 0;
}


API int fcd_get_iq_correction(FCD *dev, int *phase, unsigned int *gain)
{
	struct {
		int16_t phase;
		uint16_t gain;
	} correction;

	if (fcd_get(dev, FCD_CMD_GET_IQ_CORR, &correction, sizeof(correction)) != sizeof(correction))
	{
		return -1;
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
	if (fcd_set(dev, FCD_CMD_SET_BIAS_TEE, &bias, sizeof(bias)) != sizeof(bias))
	{
		return -1;
	}
	return 0;
}


API int fcd_get_bias_tee(FCD *dev, unsigned char *state)
{
	unsigned char bias;
	if (fcd_get(dev, FCD_CMD_GET_BIAS_TEE, &bias, sizeof(bias)) != sizeof(bias))
	{
		return -1;
	}
	*state = bias;
	return 0;
}


API int fcd_set_frequency_Hz(FCD *dev, unsigned int freq)
{
	uint32_t fHz = convert_le_u32(freq);
	if (fcd_set(dev, FCD_CMD_SET_FREQUENCY_HZ, &fHz, sizeof(fHz)) != sizeof(fHz))
	{
		return -1;
	}
	return 0;
}


API int fcd_get_frequency_Hz(FCD *dev, unsigned int *freq)
{
	uint32_t fHz;
	if (fcd_get(dev, FCD_CMD_GET_FREQUENCY_HZ, &fHz, sizeof(fHz)) != sizeof(fHz))
	{
		return -1;
	}
	*freq = convert_le_u32(fHz);
	return 0;
}

