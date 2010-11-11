/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#ifndef G729_ENC_H_INCLUDED
#define G729_ENC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#include "sG729.h"

#define G729_FRAME_LENGTH_IN_TIMESTAMP 20
#define MAX_G729_FRAME_SIZE 80
#define MAX_NUM_OUTPUT_FRAMES_PER_BUFFER 10

#define ENABLE_LOG_G729_OMX
#ifdef ENABLE_LOG_G729_OMX
#include <utils/Log.h>
#define LOGE_G729_OMX   LOGE
#else
#define LOGE_G729_OMX //LOGE
#endif

class OmxG729Encoder
{
    public:
        OmxG729Encoder();

        OMX_BOOL G729EncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                            OMX_AUDIO_PARAM_G729TYPE aG729Param,
                            OMX_U32* aInputFrameLength,
                            OMX_U32* aMaxNumberOutputFrames);

        void G729EncDeinit();

        OMX_BOOL G729EncodeFrame(OMX_U8* aOutputBuffer,
                                OMX_U32* aOutputLength,
                                OMX_U8* aInBuffer,
                                OMX_U32 aInBufSize,
                                OMX_TICKS aInTimeStamp,
                                OMX_TICKS* aOutTimeStamp);

    private:

        //Codec and encoder settings
        sG729E_INFO   ipG729Encoder;
		
#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		void *sG729E_handler;
#endif

        //Encoding Settings parameters

        int32*			 ipSizeArrayForOutputFrames;
        uint32			 iMaxNumOutputFramesPerBuffer;
        uint32			 iOneInputFrameLength;
        uint32			 iMaxInputSize;
        PVMFFormatType	 iOutputFormat;
        uint32				iStartTime;
		uint32				iStopTime;
		uint32				iNextStartTime;

        OMX_S32 iG729InitFlag;

};



#endif	//#ifndef G711_ENC_H_INCLUDED

