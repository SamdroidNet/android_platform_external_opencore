/* ---------------------------------------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name    : MP3 Encoder.
 * @File Name              : mp3_enc.h
 * @File Description       : Mp3 Encoder header file.
 * @Author                 : Sudhir Vyas, Renuka V
 * @Created Date           : 05-06-2009
 * @Modification History
 * Version:            Date:             By:                Change:
 * ----------------------------------------------------------------------------------------------------
 */
#ifndef MP3_ENC_H_INCLUDED
#define MP3_ENC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif
#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#include "sMP3Enc.h"


//#define ENABLE_LOG_MP3_ENC_OMX

#ifdef ENABLE_LOG_MP3_ENC_OMX
#include <utils/Log.h>
#define LOGE_MP3_ENC_OMX   LOGE
#else
#define LOGE_MP3_ENC_OMX //LOGE
#endif

#define MP3_FRAME_LENGTH_IN_TIMESTAMP            20
#define MAX_MP3_FRAME_SIZE                       1988        // Set according to encoder
#define MAX_NUM_OUTPUT_FRAMES_PER_BUFFER         10            // Set according to encoder
#define MP3_ONE_INPUTFRAME_DEFAULT_LENGTH        1152        // Other values can be 384, 576

// Sampling Rates
#define SAMPLING_RATE_8K                         8000
#define CHANNELS_MONO                            1
#define BITS_PER_SAMPLE_16                       16


class OmxMp3Encoder
{
    public:

        OmxMp3Encoder();
        void Mp3EncDeinit();

        OMX_BOOL Mp3EncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                            OMX_AUDIO_PARAM_MP3TYPE aMP3Param,
                            OMX_U32* aInputFrameLength,
                            OMX_U32* aMaxNumberOutputFrames);

        OMX_BOOL Mp3EncodeAudio(OMX_U8*    aOutputBuffer,
                                OMX_U32*   aOutputLength,
                                OMX_U8*    aInBuffer,
                                OMX_U32    aInBufSize,
                                OMX_TICKS  aInTimeStamp,
                                OMX_TICKS* aOutTimeStamp);


    private:
        // All variables related to LSI MP3 encoder
        sMP3_Enc_Struct     enc_struct;
        unsigned char       *MP3InputBuffer ;
        unsigned char       *MP3OutputBuffer ;

        // Encoder properties
        // As uint32 is giving build errors, unsigned int is being used, we can use uint32_t also
        unsigned int        iMaxNumOutputFramesPerBuffer;
        unsigned int        iOneInputFrameLength;
        unsigned int        iMaxInputSize;
        unsigned int        iStartTime;
        unsigned int        iStopTime;
        unsigned int        iNextStartTime;

        #ifdef __MI_USE__ // Used for multiple instance
            void *sMP3E_handler;
        #endif

};

#endif    //#ifndef MP3_ENC_H_INCLUDED



