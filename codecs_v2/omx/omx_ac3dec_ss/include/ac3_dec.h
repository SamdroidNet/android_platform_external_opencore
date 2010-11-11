/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#ifndef AC3_DEC_H_INCLUDED
#define AC3_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#include "sAC3Dec.h"

//#define ENABLE_LOG_AC3_OMX
#ifdef ENABLE_LOG_AC3_OMX
#include <utils/Log.h>
#define LOGE_AC3_OMX   LOGE
#else
#define LOGE_AC3_OMX //LOGE
#endif

// Defile if header needs to searched and parsed for each input buffer
#define __AC3_PARSE_HEADER_ALWAYS__
//if using the teamp output and input buffers
//#define __AC3_USE_MEMCPY_IN__
//#define __AC3_USE_MEMCPY_OUT__


typedef enum
    {
        AC3DEC_SUCCESS           =  0,
        AC3DEC_INVALID_FRAME     = 10,
        AC3DEC_INCOMPLETE_FRAME  = 20,
        AC3DEC_LOST_FRAME_SYNC   = 30
    } tPVAC3DecoderErrorCode;

class OmxAc3Decoder
{
    public:
        OmxAc3Decoder();

        OMX_BOOL Ac3DecInit();

        void Ac3DecDeinit();

        int Ac3DecodeFrame(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_AC3TYPE* aAudioAc3Param,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag);

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 iAc3InitFlag;

    private:

    	OMX_BOOL Ac3DecodeHeader(OMX_U8* aInBuffer);
    	OMX_BOOL Ac3SearchHeader(OMX_U8* aInBuffer, OMX_U32 aInputBuffLength, OMX_U32* aHeaderPosition);
    	OMX_BOOL iSamplingFrqReset ;

        OMX_S32 iAc3SmplFreq;
		OMX_S32 iTmpSmplFreq;
		
        OMX_S32 iAc3FrameSize;

#ifdef __AC3_USE_MEMCPY_IN__
    	unsigned char* StreamBuff;
#endif//__AC3_USE_MEMCPY_IN__

#ifdef __AC3_USE_MEMCPY_OUT__
		short* OutBuff;
#endif//__AC3_USE_MEMCPY_OUT__

        sAC3D_INFO iAudioAc3Decoder;
};



#endif	//#ifndef AC3_DEC_H_INCLUDED

