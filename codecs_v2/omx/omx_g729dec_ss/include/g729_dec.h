/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

#ifndef G729_DEC_H_INCLUDED
#define G729_DEC_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

// G729 Codec Library allocatec in and out buffers, which causes a mem leak.
// To avoid the Leak, we will store these pointer and give when we call delete decoder
#define _CODEC_LIB_ALLOC_BUFFER_

#include "sG729.h"

typedef enum
    {
        G729DEC_SUCCESS           =  0,
        G729DEC_INVALID_FRAME     = 10,
        G729DEC_INCOMPLETE_FRAME  = 20,
        G729DEC_LOST_FRAME_SYNC   = 30
    } tPVG729DecoderErrorCode;

class OmxG729Decoder
{
    public:
        OmxG729Decoder();

        OMX_BOOL G729DecInit();

        void G729DecDeinit();

        int G729DecodeFrame(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_G729TYPE* aAudioG729Param,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag);

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 iG729InitFlag;

    private:

		OMX_S32 iG729SmplFreq;
        OMX_S32 iG729FrameSize;
		sG729D_INFO sG729D;
        unsigned char InBuf[10];  // Frame size of G.729 is 10 bytes
        short OutBuf[L_FRAME];
#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
	void *sG729D_handler;
#endif
		short  iframe;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	//#ifndef G729_DEC_H_INCLUDED

