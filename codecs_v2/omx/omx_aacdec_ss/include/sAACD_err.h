#ifndef _SAACD_ERR_
#define  _SAACD_ERR_

/* return values */ 
#define SAAC_OK										(0)
#define SAACD_ERR									(-1)
#define SAACD_ERR_INVALID_ARG						(-2)



#define ERR_HUFFMAN_DECODING						(-1000)
#define ERR_ID_CCE_ELEMENT							(-1100)
#define ERR_UNKNOWN_ID_SYN_ELE						(-1200)
#define ERR_UNSURPPORTED_PREDICTION_FUNC			(-1310)
#define ERR_UNSURPPORTED_GAIN_CONTROL_FUNC			(-1320)
#define ERR_INVALID_CODE_BOOK						(-1400)
#define ERR_TNS_BITSTEAM							(-1500)
#define ERR_PULSE_DEC_MEM_OVERFLOW					(-1600)

#define ERR_ADTS_HEADER								(-1910)
#define ERR_ADIF_HEADER								(-1920)

#define ERR_INVERSE_QUANTIZATION_DECODING			(-2000)

#define ERR_SBR_DEC									(-3100)
#define ERR_SBR_NOSYNCH								(-3101)
#define ERR_SBR_ILLEGAL_PROGRAM						(-3102)
#define ERR_SBR_ILLEGAL_TAG							(-3103)
#define ERR_SBR_ILLEGAL_CHN_CONFIG					(-3104)
#define ERR_SBR_ILLEGAL_SECTION						(-3105)
#define ERR_SBR_ILLEGAL_SCFACTORS					(-3106)
#define ERR_SBR_ILLEGAL_PULSE_DATA					(-3107)
#define ERR_SBR_MAIN_PROFILE_NOT_IMPLEMENTED		(-3108)
#define ERR_SBR_GC_NOT_IMPLEMENTED					(-3109)
#define ERR_SBR_ILLEGAL_PLUS_ELE_ID					(-3110)
#define ERR_SBR_CREATE_ERROR						(-3111)
#define ERR_SBR_NOT_INITIALIZED						(-3112)
#define ERR_SBR_ILLEGAL_NSFB						(-3113)
#define ERR_SBR_GET_ENVELOPE						(-3114)
#define ERR_SBR_OUT_OF_NO_NOISE_BANDS				(-3115)
#define ERR_SBR_DOWN_SAMPLE_LO_RES					(-3116)
#define ERR_SBR_OUT_OF_PATCH_SIZE					(-3117)
#define ERR_SBR_OUT_OF_NO_LOBAND					(-3118)
#define ERR_SBR_CRC_LENGTH							(-3119)
#define ERR_SBR_FREQ_BAND_TBL						(-3120)
#define ERR_SBR_IDX_START_ERR						(-3121)



#define ERR_SBR_INIT								(-3200)
#define ERR_SBR_NUM_OF_SBR_ELEMENTS					(-3300)



#define UNSURPPORTED_SAMPLING_FREQUENCY				(-4002)
#define UNSURPPORTED_ELEMENTS						(-4003)
#define UNSURPPORTED_AUDIO_OBJECT_TYPE				(-4004)
#define UNSURPPORTED_NUM_OF_ELEMENTS				(-4005)
#define UNSURPPORTED_CH_NORMALIZE_MODE 				(-4100)
#define UNSURPPORTED_CH_PACK_MODE					(-4101)


#define UNKNOWN_CODE_MODE							(-5100)



#define ERR_IN_BUFFER_OVERFLOW						(-9000)
#define ERR_IN_BUFFER_INSUFFICIENT					(-9001)

#endif