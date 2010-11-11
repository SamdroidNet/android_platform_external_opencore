/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#ifndef EVRC_DEC_H_INCLUDED
#define EVRC_DEC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#include "sEVRC.h"

// Defile if Sampling Frequency need to be calculated for each frame coming
//#define __EVRC_SUPPORT_VERIABLE_SMPLFREQNCY__
//if using the teamp output and input buffers
//#define __EVRC_USE_MEMCPY_IN__
//#define __EVRC_USE_MEMCPY_OUT__


typedef enum
{
    EVRCDEC_SUCCESS           =  0,
    EVRCDEC_INVALID_FRAME     = 10,
    EVRCDEC_INCOMPLETE_FRAME  = 20,
    EVRCDEC_LOST_FRAME_SYNC   = 30
} tPVEVRCDecoderErrorCode;

class OmxEvrcDecoder
{
    public:
        OmxEvrcDecoder();

        OMX_BOOL EvrcDecInit();

        void EvrcDecDeinit();

        int EvrcDecodeFrame(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_S16** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_EVRCTYPE* aAudioEvrcParam,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag);

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 EvrcInitFlag;

    private:
    	OMX_BOOL iSamplingFrqReset ;
		sEVRC_INFO sEVRCD;
};



#endif	//#ifndef EVRC_DEC_H_INCLUDED

