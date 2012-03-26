/*! \file
 * \brief FUNcube dongle tuner hardware enumerations
 * \note Values, names, and descriptions are derived from \c FCHID008.zip.
 */

/*! \brief Tuner LNA gain values */
typedef enum
{
	FCD_TLGE_N5_0DB=0,   /*!< \brief -5.0 dB */
	FCD_TLGE_N2_5DB=1,   /*!< \brief -2.5 dB */
	FCD_TLGE_P0_0DB=4,   /*!< \brief 0.0 dB */
	FCD_TLGE_P2_5DB=5,   /*!< \brief 2.5 dB */
	FCD_TLGE_P5_0DB=6,   /*!< \brief 5.0 dB */
	FCD_TLGE_P7_5DB=7,   /*!< \brief 7.5 dB */
	FCD_TLGE_P10_0DB=8,  /*!< \brief 10.0 dB */
	FCD_TLGE_P12_5DB=9,  /*!< \brief 12.5 dB */
	FCD_TLGE_P15_0DB=10, /*!< \brief 15.0 dB */
	FCD_TLGE_P17_5DB=11, /*!< \brief 17.5 dB */
	FCD_TLGE_P20_0DB=12, /*!< \brief 20.0 dB */
	FCD_TLGE_P25_0DB=13, /*!< \brief 25.0 dB */
	FCD_TLGE_P30_0DB=14  /*!< \brief 30.0 dB */
} FCD_TUNER_LNA_GAIN_ENUM;

/*! \brief Tuner LNA enhance values */
typedef enum
{
	FCD_TLEE_OFF=0, /*!< \brief Off */
	FCD_TLEE_0=1,   /*!< \brief 0 */
	FCD_TLEE_1=3,   /*!< \brief 1 */
	FCD_TLEE_2=5,   /*!< \brief 2 */
	FCD_TLEE_3=7    /*!< \brief 3 */
} FCD_TUNER_LNA_ENHANCE_ENUM;

/*! \brief Tuner band selection values */
typedef enum
{
	FCD_TBE_VHF2, /*!< \brief VHF II (DC - 220 MHz) */
	FCD_TBE_VHF3, /*!< \brief VHF III (220 MHz - 350 MHz) */
	FCD_TBE_UHF,  /*!< \brief UHF (350 MHz - 1 GHz) */
	FCD_TBE_LBAND /*!< \brief L band (> 1 GHz) */
} FCD_TUNER_BAND_ENUM;

/*! \brief Tuner RF filter selection values */
typedef enum
{
	/* Band 0, VHF II */
	FCD_TRFE_LPF268MHZ=0,   /*!< \brief 268 MHz low-pass */
	FCD_TRFE_LPF299MHZ=8,   /*!< \brief 299 MHz low-pass */
	/* Band 1, VHF III */
	FCD_TRFE_LPF509MHZ=0,   /*!< \brief 509 MHz low-pass */
	FCD_TRFE_LPF656MHZ=8,   /*!< \brief 656 MHz low-pass */
	/* Band 2, UHF */
	FCD_TRFE_BPF360MHZ=0,   /*!< \brief 360 MHz band-pass */
	FCD_TRFE_BPF380MHZ=1,   /*!< \brief 380 MHz band-pass */
	FCD_TRFE_BPF405MHZ=2,   /*!< \brief 405 MHz band-pass */
	FCD_TRFE_BPF425MHZ=3,   /*!< \brief 425 MHz band-pass */
	FCD_TRFE_BPF450MHZ=4,   /*!< \brief 450 MHz band-pass */
	FCD_TRFE_BPF475MHZ=5,   /*!< \brief 475 MHz band-pass */
	FCD_TRFE_BPF505MHZ=6,   /*!< \brief 505 MHz band-pass */
	FCD_TRFE_BPF540MHZ=7,   /*!< \brief 540 MHz band-pass */
	FCD_TRFE_BPF575MHZ=8,   /*!< \brief 575 MHz band-pass */
	FCD_TRFE_BPF615MHZ=9,   /*!< \brief 615 MHz band-pass */
	FCD_TRFE_BPF670MHZ=10,  /*!< \brief 670 MHz band-pass */
	FCD_TRFE_BPF720MHZ=11,  /*!< \brief 720 MHz band-pass */
	FCD_TRFE_BPF760MHZ=12,  /*!< \brief 760 MHz band-pass */
	FCD_TRFE_BPF840MHZ=13,  /*!< \brief 840 MHz band-pass */
	FCD_TRFE_BPF890MHZ=14,  /*!< \brief 890 MHz band-pass */
	FCD_TRFE_BPF970MHZ=15,  /*!< \brief 970 MHz band-pass */
	/* Band 2, L band */
	FCD_TRFE_BPF1300MHZ=0,  /*!< \brief 1300 MHz band-pass */
	FCD_TRFE_BPF1320MHZ=1,  /*!< \brief 1320 MHz band-pass */
	FCD_TRFE_BPF1360MHZ=2,  /*!< \brief 1360 MHz band-pass */
	FCD_TRFE_BPF1410MHZ=3,  /*!< \brief 1410 MHz band-pass */
	FCD_TRFE_BPF1445MHZ=4,  /*!< \brief 1445 MHz band-pass */
	FCD_TRFE_BPF1460MHZ=5,  /*!< \brief 1460 MHz band-pass */
	FCD_TRFE_BPF1490MHZ=6,  /*!< \brief 1490 MHz band-pass */
	FCD_TRFE_BPF1530MHZ=7,  /*!< \brief 1530 MHz band-pass */
	FCD_TRFE_BPF1560MHZ=8,  /*!< \brief 1560 MHz band-pass */
	FCD_TRFE_BPF1590MHZ=9,  /*!< \brief 1590 MHz band-pass */
	FCD_TRFE_BPF1640MHZ=10, /*!< \brief 1640 MHz band-pass */
	FCD_TRFE_BPF1660MHZ=11, /*!< \brief 1660 MHz band-pass */
	FCD_TRFE_BPF1680MHZ=12, /*!< \brief 1680 MHz band-pass */
	FCD_TRFE_BPF1700MHZ=13, /*!< \brief 1700 MHz band-pass */
	FCD_TRFE_BPF1720MHZ=14, /*!< \brief 1720 MHz band-pass */
	FCD_TRFE_BPF1750MHZ=15  /*!< \brief 1750 MHz band-pass */
} FCD_TUNER_RF_FILTER_ENUM;

/*! \brief Tuner mixer gain values */
typedef enum
{
	FCD_TMGE_P4_0DB=0, /*!< \brief 4.0 dB */
	FCD_TMGE_P12_0DB=1 /*!< \brief 12.0 dB */
} FCD_TUNER_MIXER_GAIN_ENUM;

/*! \brief Tuner bias current selection values */
typedef enum
{
	FCD_TBCE_LBAND=0, /*!< \brief L band */
	FCD_TBCE_1=1,     /*!< \brief 01? */
	FCD_TBCE_2=2,     /*!< \brief 10? */
	FCD_TBCE_VUBAND=3 /*!< \brief V/U band */
} FCD_TUNER_BIAS_CURRENT_ENUM;

/*! \brief Tuner mixer filter selection values */
typedef enum
{
	FCD_TMFE_27_0MHZ=0, /*!< \brief 27.0 MHz */
	FCD_TMFE_4_6MHZ=8,  /*!< \brief 4.6 MHz */
	FCD_TMFE_4_2MHZ=9,  /*!< \brief 4.2 MHz */
	FCD_TMFE_3_8MHZ=10, /*!< \brief 3.8 MHz */
	FCD_TMFE_3_4MHZ=11, /*!< \brief 3.4 MHz */
	FCD_TMFE_3_0MHZ=12, /*!< \brief 3.0 MHz */
	FCD_TMFE_2_7MHZ=13, /*!< \brief 2.7 MHz */
	FCD_TMFE_2_3MHZ=14, /*!< \brief 2.3 MHz */
	FCD_TMFE_1_9MHZ=15  /*!< \brief 1.9 MHz */
} FCD_TUNER_MIXER_FILTER_ENUM;

/*! \brief Tuner IF amplifier 1 gain values */
typedef enum
{
	FCD_TIG1E_N3_0DB=0, /*!< \brief-3.0 dB  */
	FCD_TIG1E_P6_0DB=1  /*!< \brief 6.0 dB */
} FCD_TUNER_IF_GAIN1_ENUM;

/*! \brief IF gain mode selection values */
typedef enum
{
	FCD_TIGME_LINEARITY=0,  /*!< \brief Linearity */
	FCD_TIGME_SENSITIVITY=1 /*!< \brief Sensitivity */
} FCD_TUNER_IF_GAIN_MODE_ENUM;

/*! \brief IF RC filter selection values */
typedef enum
{
	FCD_TIRFE_21_4MHZ=0, /*!< \brief 21.4 MHz */
	FCD_TIRFE_21_0MHZ=1, /*!< \brief 21.0 MHz */
	FCD_TIRFE_17_6MHZ=2, /*!< \brief 17.6 MHz */
	FCD_TIRFE_14_7MHZ=3, /*!< \brief 14.7 MHz */
	FCD_TIRFE_12_4MHZ=4, /*!< \brief 12.4 MHz */
	FCD_TIRFE_10_6MHZ=5, /*!< \brief 10.6 MHz */
	FCD_TIRFE_9_0MHZ=6,  /*!< \brief 9.0 MHz */
	FCD_TIRFE_7_7MHZ=7,  /*!< \brief 7.7 MHz */
	FCD_TIRFE_6_4MHZ=8,  /*!< \brief 6.4 MHz */
	FCD_TIRFE_5_3MHZ=9,  /*!< \brief 5.3 MHz */
	FCD_TIRFE_4_4MHZ=10, /*!< \brief 4.4 MHz */
	FCD_TIRFE_3_4MHZ=11, /*!< \brief 3.4 MHz */
	FCD_TIRFE_2_6MHZ=12, /*!< \brief 2.6 MHz */
	FCD_TIRFE_1_8MHZ=13, /*!< \brief 1.8 MHz */
	FCD_TIRFE_1_2MHZ=14, /*!< \brief 1.2 MHz */
	FCD_TIRFE_1_0MHZ=15  /*!< \brief 1.0 MHz */
} FCD_TUNER_IF_RC_FILTER_ENUM;

/*! \brief Tuner IF amplifier 2 gain values */
typedef enum
{
	FCD_TIG2E_P0_0DB=0, /*!< \brief 0.0 dB */
	FCD_TIG2E_P3_0DB=1, /*!< \brief 3.0 dB */
	FCD_TIG2E_P6_0DB=2, /*!< \brief 6.0 dB */
	FCD_TIG2E_P9_0DB=3  /*!< \brief 9.0 dB */
} FCD_TUNER_IF_GAIN2_ENUM;

/*! \brief Tuner IF amplifier 3 gain values */
typedef enum
{
	FCD_TIG3E_P0_0DB=0, /*!< \brief 0.0 dB */
	FCD_TIG3E_P3_0DB=1, /*!< \brief 3.0 dB */
	FCD_TIG3E_P6_0DB=2, /*!< \brief 6.0 dB */
	FCD_TIG3E_P9_0DB=3  /*!< \brief 9.0 dB */
} FCD_TUNER_IF_GAIN3_ENUM;

/*! \brief Tuner IF amplifier 4 gain values */
typedef enum
{
	FCD_TIG4E_P0_0DB=0, /*!< \brief 0.0 dB */
	FCD_TIG4E_P1_0DB=1, /*!< \brief 1.0 dB */
	FCD_TIG4E_P2_0DB=2  /*!< \brief 2.0 dB */
} FCD_TUNER_IF_GAIN4_ENUM;

/*! \brief Tuner IF filter selection values */
typedef enum
{
	FCD_TIFE_5_50MHZ=0,  /*!< \brief 5.50 MHz */
	FCD_TIFE_5_30MHZ=1,  /*!< \brief 5.30 MHz */
	FCD_TIFE_5_00MHZ=2,  /*!< \brief 5.00 MHz */
	FCD_TIFE_4_80MHZ=3,  /*!< \brief 4.80 MHz */
	FCD_TIFE_4_60MHZ=4,  /*!< \brief 4.60 MHz */
	FCD_TIFE_4_40MHZ=5,  /*!< \brief 4.40 MHz */
	FCD_TIFE_4_30MHZ=6,  /*!< \brief 4.30 MHz */
	FCD_TIFE_4_10MHZ=7,  /*!< \brief 4.10 MHz */
	FCD_TIFE_3_90MHZ=8,  /*!< \brief 3.90 MHz */
	FCD_TIFE_3_80MHZ=9,  /*!< \brief 3.80 MHz */
	FCD_TIFE_3_70MHZ=10, /*!< \brief 3.70 MHz */
	FCD_TIFE_3_60MHZ=11, /*!< \brief 3.60 MHz */
	FCD_TIFE_3_40MHZ=12, /*!< \brief 3.40 MHz */
	FCD_TIFE_3_30MHZ=13, /*!< \brief 3.30 MHz */
	FCD_TIFE_3_20MHZ=14, /*!< \brief 3.20 MHz */
	FCD_TIFE_3_10MHZ=15, /*!< \brief 3.10 MHz */
	FCD_TIFE_3_00MHZ=16, /*!< \brief 3.00 MHz */
	FCD_TIFE_2_95MHZ=17, /*!< \brief 2.95 MHz */
	FCD_TIFE_2_90MHZ=18, /*!< \brief 2.90 MHz */
	FCD_TIFE_2_80MHZ=19, /*!< \brief 2.80 MHz */
	FCD_TIFE_2_75MHZ=20, /*!< \brief 2.75 MHz */
	FCD_TIFE_2_70MHZ=21, /*!< \brief 2.70 MHz */
	FCD_TIFE_2_60MHZ=22, /*!< \brief 2.60 MHz */
	FCD_TIFE_2_55MHZ=23, /*!< \brief 2.55 MHz */
	FCD_TIFE_2_50MHZ=24, /*!< \brief 2.50 MHz */
	FCD_TIFE_2_45MHZ=25, /*!< \brief 2.45 MHz */
	FCD_TIFE_2_40MHZ=26, /*!< \brief 2.40 MHz */
	FCD_TIFE_2_30MHZ=27, /*!< \brief 2.30 MHz */
	FCD_TIFE_2_28MHZ=28, /*!< \brief 2.28 MHz */
	FCD_TIFE_2_24MHZ=29, /*!< \brief 2.24 MHz */
	FCD_TIFE_2_20MHZ=30, /*!< \brief 2.20 MHz */
	FCD_TIFE_2_15MHZ=31  /*!< \brief 2.15 MHz */
} FCD_TUNER_IF_FILTER_ENUM;

/*! \brief Tuner IF amplifier 5 gain values */
typedef enum
{
	FCD_TIG5E_P3_0DB=0,  /*!< \brief 3.0 dB */
	FCD_TIG5E_P6_0DB=1,  /*!< \brief 6.0 dB */
	FCD_TIG5E_P9_0DB=2,  /*!< \brief 9.0 dB */
	FCD_TIG5E_P12_0DB=3, /*!< \brief 12.0 dB */
	FCD_TIG5E_P15_0DB=4  /*!< \brief 15.0 dB */
} FCD_TUNER_IF_GAIN5_ENUM;

/*! \brief Tuner IF amplifier 6 gain values */
typedef enum
{
	FCD_TIG6E_P3_0DB=0,  /*!< \brief 3.0 dB */
	FCD_TIG6E_P6_0DB=1,  /*!< \brief 6.0 dB */
	FCD_TIG6E_P9_0DB=2,  /*!< \brief 9.0 dB */
	FCD_TIG6E_P12_0DB=3, /*!< \brief 12.0 dB */
	FCD_TIG6E_P15_0DB=4  /*!< \brief 15.0 dB */
} FCD_TUNER_IF_GAIN6_ENUM;
