/* ------------------------------------------------------------------
 * Copyright (C) 2008 PacketVideo
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
#ifndef MP3_DEC_H_INCLUDED
#define MP3_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif


#ifndef PVMP3_DECODER_H
//#include "pvmp3_decoder.h"
#include "oscl_mem.h"
#include "oscl_base.h"
#include "sMP3Dec.h"
#endif

#define BUFF_SIZE 0x780		// 20081127 - JM

typedef int8        Char;
typedef uint8       UChar;
typedef signed int  Int;
typedef unsigned int    UInt;

typedef enum
    {
        flat       = 0,
        bass_boost = 1,
        rock       = 2,
        pop        = 3,
        jazz       = 4,
        classical  = 5,
        talk       = 6,
        flat_      = 7

    } e_equalization;

typedef enum
    {
        MP3DEC_SUCCESS           =  0,
        MP3DEC_INVALID_FRAME     = 10,
        MP3DEC_INCOMPLETE_FRAME  = 20,
        MP3DEC_LOST_FRAME_SYNC   = 30
    } tPVMP3DecoderErrorCode;

class Mp3Decoder
{
    public:

        Mp3Decoder();

        OMX_BOOL Mp3DecInit(OMX_AUDIO_CONFIG_EQUALIZERTYPE* aEqualizerType);
        void Mp3DecDeinit();

        Int Mp3DecodeAudio(OMX_S16* aOutBuff,
                           OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                           OMX_U32* aInBufSize,
                           OMX_S32* aIsFirstBuffer,
                           OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                           OMX_AUDIO_PARAM_MP3TYPE* aAudioMp3Param,
                           OMX_BOOL aMarkerFlag,
                           OMX_BOOL* aResizeFlag);

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 iInitFlag;

    private:

        mp3_dec_struct dec_struct;
        //short PCMBuf[1152*2];
		//unsigned char BSBuf[BUFF_SIZE];
		short *PCMBuf;
		unsigned char *BSBuf;

		FILE *fp;

};



#endif	//#ifndef MP3_DEC_H_INCLUDED

