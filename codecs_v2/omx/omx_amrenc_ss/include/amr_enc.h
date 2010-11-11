/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
#ifndef AMR_ENC_H_INCLUDED
#define AMR_ENC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#ifndef LSI_AMRENCODER_H_FILE
#include "sAMR_NB.h"
#endif

//#define DEBUG_AMR_ENC_INFO

#define AMR_FRAME_LENGTH_IN_TIMESTAMP				20
#define MAX_AMR_FRAME_SIZE							32
#define MAX_NUM_OUTPUT_FRAMES_PER_BUFFER			10
#define AMR_ONE_INPUTFRAME_DEFAULT_LENGTH			320


class OmxAmrEncoder
{
    public:
        OmxAmrEncoder();

        OMX_BOOL AmrEncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                            OMX_AUDIO_PARAM_AMRTYPE aAmrParam,
                            OMX_U32* aInputFrameLength,
                            OMX_U32* aMaxNumberOutputFrames);

        void AmrEncDeinit();

        OMX_BOOL AmrEncodeFrame(OMX_U8* aOutputBuffer,
                                OMX_U32* aOutputLength,
                                OMX_U8* aInBuffer,
                                OMX_U32 aInBufSize,
                                OMX_TICKS aInTimeStamp,
                                OMX_TICKS* aOutTimeStamp);

    private:

        //AMR NB Encoder --> LSI Codec
		sAMR_NB_Enc_Struct	sAMRNBEnc;
        void *sAMR_NBEnc_handler;

        //Encoding Settings parameters
        uint32				iMaxNumOutputFramesPerBuffer;
        uint32				iOneInputFrameLength;

		uint32				iStartTime;
		uint32				iStopTime;
		uint32				iNextStartTime;


#ifdef DEBUG_AMR_ENC_INFO
		FILE			 *fp;
		uint32			 frameCount;
#endif //DEBUG_AMR_ENC_INFO

};

#endif	//#ifndef AMR_ENC_H_INCLUDED

