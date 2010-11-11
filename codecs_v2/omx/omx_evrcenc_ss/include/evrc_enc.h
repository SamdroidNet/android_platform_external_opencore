/* ----------------------------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name	: EVRC Encoder
 * @File Name			: evrc_enc.h
 * @File Description	: Header file for evrc encoder.
 * @Author				: Sudhir Vyas
 * @Created Date		: 16-06-2009
 * @Modification History
 * Version:				Date:				By:				Change:
 * ----------------------------------------------------------------------------------------
 */
#ifndef EVRC_ENC_H_INCLUDED
#define EVRC_ENC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#define ENABLE_LOG_EVRC_OMX
#ifdef ENABLE_LOG_EVRC_OMX
#include <utils/Log.h>
#define LOGE_EVRC_OMX   LOGE
#else
#define LOGE_EVRC_OMX //LOGE
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#include "sEVRC.h"

#define EVRC_FRAME_LENGTH_IN_TIMESTAMP				20
#define MAX_EVRC_FRAME_SIZE							24		// 12 in words as encoder always gives 12 words as output buffer.
#define MAX_NUM_OUTPUT_FRAMES_PER_BUFFER			1
#define EVRC_ONE_INPUTFRAME_DEFAULT_LENGTH			320		// 160 in words as encoder always takes 160 words as input buffer.

#define NUM_CHANNELS_2								2

#define EVRC_MIN_FRAME_RATE							1
#define EVRC_MAX_FRAME_RATE							4
#define NOISE_SUPPRESSION                           1
#define SAMPLING_RATE8K								8000

class OmxEvrcEncoder
{
    public:
        OmxEvrcEncoder();

        OMX_BOOL EvrcEncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
							 OMX_AUDIO_PARAM_EVRCTYPE aEVRCParam,
                             OMX_U32* aInputFrameLength,
                             OMX_U32* aMaxNumberOutputFrames);

        void EvrcEncDeinit();

        OMX_BOOL EvrcEncodeAudio(OMX_U8*    aOutputBuffer,
								OMX_U32*   aOutputLength,
								OMX_U8*    aInBuffer,
								OMX_U32    aInBufSize,
								OMX_TICKS  aInTimeStamp,
								OMX_TICKS* aOutTimeStamp);

    private:
		// All variables related to LSI EVRC encoder
		uint32				ret;

		// Enocder structure
		sEVRC_INFO			sEVRCE;

		// This padding is temperory solution, as the sEVRCEncFrame function call always corrupt 12 bytes of data, which is declared after
		// encoder structure, this issue will be fixed when library team will solve the same.
		uint32				PaddingToAvoidBytesCorruption[3];

		// Other encoder related variables
		uint32				iStartTime;
		uint32				iStopTime;
		uint32				iNextStartTime;
		uint32				iOneInputFrameLength;
		uint32				iMaxNumOutputFramesPerBuffer;

};

#endif	//#ifndef EVRC_ENC_H_INCLUDED



