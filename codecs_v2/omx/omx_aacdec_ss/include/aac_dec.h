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
#ifndef AAC_DEC_H_INCLUDED
#define AAC_DEC_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef PVMP4AUDIODECODER_API_H
#include "pvmp4audiodecoder_api.h"
#endif

#include "sAACplusDec.h"
#include "sAACplusBuffCfg.h"

#define BYTES_PER_PCM_SAMPLE 2
#define AACDEC_PCM_FRAME_SAMPLE_SIZE 1024

static const uint32 AACSamplingRateTbl[12] =
{
	96000,
	88200,
	64000,
	48000,
	44100,
	32000,
	24000,
	22050,
	16000,
	12000,
	11025,
	8000
};

class OmxAacDecoder
{
    public:
        OmxAacDecoder();

        OMX_BOOL AacDecInit(OMX_U32 aDesiredChannels);

        void AacDecDeinit();

        Int AacDecodeFrames(OMX_S16* aOutputBuffer,
                            OMX_U32* aOutputLength, OMX_U8** aInBuffer,
                            OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer,
                            OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                            OMX_AUDIO_PARAM_AACPROFILETYPE* aAudioAacParam,
                            OMX_U32* aSamplesPerFrame, OMX_BOOL* aResizeFlag,
                            OMX_BOOL aEndofStream);

        void UpdateAACPlusEnabled(OMX_BOOL flag);
        void UpdateAACStreamFormat(OMX_AUDIO_AACSTREAMFORMATTYPE aAACStreamFormat);

        void ResetDecoder(); // for repositioning

        OMX_S32 iAacInitFlag;
        OMX_S32 iInputUsedLength;

    private:
        /* AAC Decoder --> LSI Codec */
        SAACD sAACDec_Handler;
		SAACD_INIT_DSC init_dsc;
		SAACD_STAT     dec_stat;
		SCMN_BITB      bitb_in;
	    SCMN_AUDB      audb_out;

        unsigned char	*InBuffHeader;
        int         	InBuffHeaderSize;
        int  iSamplingRate;
		int  iChannels;
		int  iDecoderCreate;

		OMX_BOOL iAacPlusEnabled;
		OMX_AUDIO_AACSTREAMFORMATTYPE iAACStreamFormat;

		OMX_BOOL ParserAACHeader();

		FILE *fp_dump;
};


#endif

