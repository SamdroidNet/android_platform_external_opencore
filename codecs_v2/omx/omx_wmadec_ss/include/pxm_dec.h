/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#ifndef PXM_DEC_H_INCLUDED
#define PXM_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#include "sWMADec.h"

//#define ENABLE_LOG_PXM_OMX
#include <utils/Log.h>
#ifdef ENABLE_LOG_PXM_OMX
#include <utils/Log.h>
#define LOGE_PXM_OMX   LOGE
#else
#define LOGE_PXM_OMX //LOGE
#endif

//#define __PXM_USE_MEMCPY_IN__
//#define __PXM_USE_MEMCPY_OUT__

typedef struct{
	unsigned short  wFormatTag; 		/* format type */
	unsigned short  nChannels;          /* number of channels (i.e. mono, stereo...) */
	unsigned long   nSamplesPerSec;     /* sample rate */
	unsigned long   nAvgBytesPerSec;    /* for buffer estimation */
	unsigned short  nBlockAlign;        /* block size of data */
	unsigned short  wBitsPerSample;     /* number of bits per sample of mono data */
	unsigned short  cbSize;             /* the count in bytes of the size of */
	unsigned short  junk3;              /* Junk data */
	unsigned long   nSamplesPerBlock;   /* for buffer estimation */
	unsigned short  nEncodeOpt;         /* block size of data */
	unsigned short  junk5;              /* Junk data */
}CONFIG_DATA;

typedef enum
    {
        PXMDEC_SUCCESS           =  0,
        PXMDEC_INVALID_FRAME     = 10,
        PXMDEC_INCOMPLETE_FRAME  = 20,
        PXMDEC_LOST_FRAME_SYNC   = 30
    } tPVPXMDecoderErrorCode;

class OmxPxmDecoder
{
    public:
        OmxPxmDecoder();

        OMX_BOOL PxmDecInit();

        void PxmDecDeinit();

        int PxmDecodeFrame(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_WMATYPE* aAudioWmaParam,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag);

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 iPxmInitFlag;

    private:

    	OMX_BOOL PxmDecodeHeader(OMX_U8* aInBuffer);

#ifdef __PXM_USE_MEMCPY_IN__
    	unsigned char* StreamBuff;
#endif//__PXM_USE_MEMCPY_IN__

#ifdef __PXM_USE_MEMCPY_OUT__
		short* OutBuff;
#endif//__PXM_USE_MEMCPY_OUT__

        SWMAD_INFO iAudioPxmDecoder;
		CONFIG_DATA iConfigData;
};



#endif	//#ifndef PXM_DEC_H_INCLUDED

