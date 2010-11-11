/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

 #ifndef G711_DEC_H_INCLUDED
#define G711_DEC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

// G711 Codec Library allocatec in and out buffers, which causes a mem leak. 
// To avoid the Leak, we will store these pointer and give when we call delete decoder
#define _CODEC_LIB_ALLOC_BUFFER_ 

#include "sG711.h"

typedef enum
    {
        G711DEC_SUCCESS           =  0,
        G711DEC_INVALID_FRAME     = 10,
        G711DEC_INCOMPLETE_FRAME  = 20,
        G711DEC_LOST_FRAME_SYNC   = 30
    } tPVG711DecoderErrorCode;

class OmxG711Decoder
{
    public:
        OmxG711Decoder();

        OMX_BOOL G711DecInit();

        void G711DecDeinit();

        int G711DecodeFrame(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_G711TYPE* aAudioG711Param,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag);

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 iG711InitFlag;

    private:

#ifdef _CODEC_LIB_ALLOC_BUFFER_ // Codec Library allocatec buffer, which causes a mem leak. To avoid the same	
		unsigned char   *pCodecLibInbuf;	
		short 			*pCodecLibOutbuf;
#endif // _CODEC_LIB_ALLOC_BUFFER_
		OMX_S32 iG711SmplFreq;
        OMX_S32 iG711FrameSize;
		sG711D_INFO sG711D;
		int	  d_rv;
};



#endif	//#ifndef G711_DEC_H_INCLUDED

