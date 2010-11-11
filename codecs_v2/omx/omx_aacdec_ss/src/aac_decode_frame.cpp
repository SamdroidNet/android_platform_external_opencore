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

/*----------------------------------------------------------------------------
 INCLUDES
----------------------------------------------------------------------------*/
#include "aac_dec.h"

//#define ENABLE_LOG_AAC_OMX
#ifdef ENABLE_LOG_AAC_OMX
#include <utils/Log.h>
#define LOGE_AAC_OMX   LOGE
#else
#define LOGE_AAC_OMX //LOGE
#endif

//#define FILE_DUMP

OmxAacDecoder::OmxAacDecoder()
{
    iAacInitFlag     = 0;
    iInputUsedLength = 0;
    InBuffHeaderSize = 0;

    iAACStreamFormat = OMX_AUDIO_AACStreamFormatMP2ADTS;
    sAACDec_Handler = NULL;

    oscl_memset(&bitb_in, 0, sizeof(SCMN_BITB));
    oscl_memset(&dec_stat, 0, sizeof(SAACD_STAT));
    oscl_memset(&init_dsc, 0, sizeof(SAACD_INIT_DSC));

}


OMX_BOOL OmxAacDecoder::AacDecInit(OMX_U32 aDesiredChannels)
{
    int ret_val = 0;

	LOGE_AAC_OMX(" AACDecoder :: Info :: Entering AacDecInit ");

    /* Initialize AAC Decoder */
    if(saac_init() != SAAC_OK)
    {
		LOGE_AAC_OMX(" AacDecInit :: Error :: Cannot initialize AAC Decoder!! ");

		return OMX_FALSE;
	}

    InBuffHeader = (unsigned char *)oscl_malloc(MAX_BS_BUFF);
    if(!InBuffHeader)
    {
		LOGE_AAC_OMX("In AacDecInit :: Header allocation failed!! ");
		return OMX_FALSE;
	}

    // default setting
	bitb_in.err = 0;
	bitb_in.mt  = SCMN_MT_AUD_AAC;

	/* initialize SAACD_INIT_DESC structure */
	init_dsc.use_pack     = SCMN_AUDB_CMT_PACK;
	init_dsc.use_normchan = 0;
	init_dsc.use_normbits = BYTES_PER_PCM_SAMPLE; /* Note: default value is 0 */
	init_dsc.use_accel    = 0;
	init_dsc.use_lc_only  = 0;
	init_dsc.chan         = aDesiredChannels;

#ifdef FILE_DUMP
	fp_dump = fopen("/sdcard/aac_dec.raw","wb+");
	if(fp_dump == NULL)
	{
		LOGE_AAC_OMX("In AacDecInit::Cannot open file for dumping ");
		return OMX_TRUE;
	}
#endif

    LOGE_AAC_OMX(" AACDecoder: Info: Exiting AacDecInit ");

    return OMX_TRUE;
}


void OmxAacDecoder::AacDecDeinit()
{

    LOGE_AAC_OMX(" AACDecoder: Info: Entering AacDecDeinit ");

    iSamplingRate = 0;
    iChannels = 0;
    iDecoderCreate = 0;

    /* Delete Decoder */
    saacd_delete(sAACDec_Handler);

    saac_deinit();

    iAacInitFlag = 0;
    iInputUsedLength = 0;
    InBuffHeaderSize = 0;

    if(InBuffHeader)
    {
		free(InBuffHeader);
		InBuffHeader = NULL;
	}

#ifdef FILE_DUMP
    fclose(fp_dump);
    fp_dump = NULL;
#endif

    LOGE_AAC_OMX(" AACDecoder: Info: Exiting AacDecDeinit ");

}

void OmxAacDecoder::ResetDecoder()
{

   LOGE_AAC_OMX(" AACDecoder: Info: Entering ResetDecoder ");

   iInputUsedLength = 0;

   /* Reset AAC Decoder */
   if(iAacInitFlag != 0)
   {
	   saacd_reset(sAACDec_Handler);
   }

   LOGE_AAC_OMX(" AACDecoder: Info: Exiting ResetDecoder ");

}


Int OmxAacDecoder::AacDecodeFrames(OMX_S16* aOutputBuffer,
                                   OMX_U32* aOutputLength, OMX_U8** aInBuffer,
                                   OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer,
                                   OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                                   OMX_AUDIO_PARAM_AACPROFILETYPE* aAudioAacParam,
                                   OMX_U32* aSamplesPerFrame,
                                   OMX_BOOL* aResizeFlag,
                                   OMX_BOOL aEndofStream)
{
	int ret_val = 0;
	int SyncWord = 0;
	int len = 0;
	Int32 StreamType;

	LOGE_AAC_OMX(" AACDecoder: Info: Entering AacDecodeFrames ");

    LOGE_AAC_OMX("In AacDecodeFrame: Input buffer size is %d", *aInBufSize);

#ifdef FILE_DUMP
	fwrite((*aInBuffer+iInputUsedLength), sizeof(char), *aInBufSize, fp_dump);
	*aInBufSize = 0;
    iInputUsedLength = 0;

	return MP4AUDEC_INVALID_FRAME;
#endif

    *aResizeFlag = OMX_FALSE;

	if(aEndofStream || (*aInBufSize == 0))
	{
		LOGE_AAC_OMX("In AacDecodeFrame:: Received an end of stream!! ");
		*aInBufSize = 0;
		iInputUsedLength = 0;
		return MP4AUDEC_SUCCESS;
    }


    if(0 == iAacInitFlag)
    {
        LOGE_AAC_OMX(" In AacDecodeFrame: Info: Header details... ");

        oscl_memcpy(InBuffHeader, *aInBuffer, *aInBufSize);
        InBuffHeaderSize = *aInBufSize;

        iInputUsedLength = 0;
        *aInBufSize      = 0;
        iAacInitFlag     = 1;

        LOGE_AAC_OMX("In AacDecodeFrame: InBuffHeaderSize = %d ", InBuffHeaderSize);

        if(InBuffHeaderSize == 2)
        {
			LOGE_AAC_OMX("In AacDecodeFrame: Header contents: 0x%x 0x%x ", InBuffHeader[0], InBuffHeader[1]);

		}
		else
		{
			LOGE_AAC_OMX("In AacDecodeFrame: Header contents: 0x%x 0x%x 0x%x 0x%x 0x%x ", InBuffHeader[0], InBuffHeader[1], InBuffHeader[2], InBuffHeader[3], InBuffHeader[4]);

		}

		return MP4AUDEC_SUCCESS;

	}

    if(iDecoderCreate == 0)
    {
		LOGE_AAC_OMX(" In AacDecodeFrame: Create a decoder instance ");

		if(OMX_TRUE != ParserAACHeader())
		{
			*aInBufSize = 0;
			iInputUsedLength = 0;

			LOGE_AAC_OMX(" In AacDecodeFrame: Error : Invalid Header!! ");

		}

		bitb_in.addr  = *aInBuffer;
		bitb_in.size  = SAACD_MAX_INPUT_BUF_SIZE;
		audb_out.a[0] = (unsigned char*)aOutputBuffer;
		init_dsc.chan = iChannels;
		init_dsc.hz   = iSamplingRate;

		sAACDec_Handler = saacd_create(&init_dsc, &ret_val);
		if(ret_val)
		{
			*aInBufSize = 0;
            iInputUsedLength = 0;
			LOGE_AAC_OMX(" In AacDecodeFrame: Error : AACDecoder creation failed!! ");
			return MP4AUDEC_INVALID_FRAME;
		}

		ret_val = saacd_info(sAACDec_Handler, &bitb_in, &audb_out, &dec_stat);

		LOGE_AAC_OMX("In AacDecodeFrame: Info: Channels %d", dec_stat.chan);
		LOGE_AAC_OMX("In AacDecodeFrame: Info: SampFrequency %d", dec_stat.hz);
		LOGE_AAC_OMX("In AacDecodeFrame: Info: sbr enabled %d", dec_stat.sbr_on);
		LOGE_AAC_OMX("In AacDecodeFrame: Info: ps enabled  %d", dec_stat.ps_on);

		if((dec_stat.hz == 96000) || (dec_stat.hz == 88200))
		{
			LOGE_AAC_OMX("In AacDecodeFrame: Sampling rate is not supported!! ");
			*aInBufSize      = 0;
			iInputUsedLength = 0;
			iAacInitFlag     = 0;
	    	return MP4AUDEC_INVALID_FRAME;
		}

		iDecoderCreate = 1;
	}

    /* Input buffer stream */
    bitb_in.addr = (*aInBuffer+iInputUsedLength);
    bitb_in.pddr = NULL;
    bitb_in.size = *aInBufSize;

    /* Audio ouput PCM data */
    audb_out.a[0] = (unsigned char*)aOutputBuffer;

    LOGE_AAC_OMX("In AacDecodeFrame: Input buffer contents: 0x%x 0x%x 0x%x 0x%x 0x%x ", *(*aInBuffer + iInputUsedLength), *(*aInBuffer + iInputUsedLength + 1), *(*aInBuffer + iInputUsedLength + 2), *(*aInBuffer + iInputUsedLength + 3), *(*aInBuffer + iInputUsedLength + 4));

    ret_val = saacd_decode(sAACDec_Handler, &bitb_in, &audb_out, &dec_stat);
    if(ret_val == 0)
    {

	    if(dec_stat.fa) /* Check for decoder frame availablity */
	    {
			*aInBufSize -= dec_stat.read;

			if (0 == *aInBufSize)
			{
				iInputUsedLength = 0;
			}
			else
			{
				iInputUsedLength += dec_stat.read;
			}

			*aOutputLength = audb_out.s[0];

            /* update the number of samples per frame */
			if(dec_stat.sbr_on)
			{
				*aSamplesPerFrame = 2 * AACDEC_PCM_FRAME_SAMPLE_SIZE;
			}
			else
			{
				*aSamplesPerFrame = AACDEC_PCM_FRAME_SAMPLE_SIZE;
			}

			(*aIsFirstBuffer)++;

			LOGE_AAC_OMX(" AacDecodeFrame: debug: dec_stat.read=%d, inputusedlen=%d, outputlength=%d, FrameCount=%d", dec_stat.read , iInputUsedLength, *aOutputLength, *aIsFirstBuffer);
            LOGE_AAC_OMX("In AacDecodeFrame: ch=%d, sf=%d, sbr=%d, ps=%d ", dec_stat.chan, dec_stat.hz, dec_stat.sbr_on, dec_stat.ps_on);
            dec_stat.read = 0;

			//After decoding the first frame, modify all the input & output port settings
			if (1 == *aIsFirstBuffer)
			{
				if(!(dec_stat.sbr_on) && !(dec_stat.ps_on))
				{
					aAudioAacParam->eAACProfile = OMX_AUDIO_AACObjectMain;
				}
				//Output Port Parameters
				aAudioPcmParam->nSamplingRate = dec_stat.hz;
				aAudioPcmParam->nChannels     = dec_stat.chan;

				//Input Port Parameters
				aAudioAacParam->nSampleRate   = dec_stat.hz;

				//Set the Resize flag to send the port settings changed callback
				*aResizeFlag = OMX_TRUE;
        	}

		}

        return MP4AUDEC_SUCCESS;
	}
	else
	{
		LOGE_AAC_OMX("In AacDecodeFrame: Info: Invalid Frame!! doing soft reset -- error %d ", ret_val);

		*aInBufSize      = 0;
		iInputUsedLength = 0;

        saacd_reset(sAACDec_Handler);

   	    return MP4AUDEC_INVALID_FRAME;

	}

    LOGE_AAC_OMX(" AACDecoder: Info: Exiting AacDecodeFrames ");
}

void OmxAacDecoder::UpdateAACPlusEnabled(OMX_BOOL flag)
{
    iAacPlusEnabled = flag;
}

void OmxAacDecoder::UpdateAACStreamFormat(OMX_AUDIO_AACSTREAMFORMATTYPE aAACStreamFormat)
{
    iAACStreamFormat = aAACStreamFormat;
}

OMX_BOOL OmxAacDecoder::ParserAACHeader()
{
    unsigned char aAudioObjectType = 0;
    unsigned char aHeaderChannelInfo = 0;
    unsigned short aSamplingFrqIndex = 0;
    OMX_U16* aTemp16 = (OMX_U16*)InBuffHeader;
    OMX_U32* aTemp32 = (OMX_U32*)(InBuffHeader);

    //LOGE_AAC_OMX("In OmxAacDecoder:: Entering ParserAACHeader ");

    aAudioObjectType = InBuffHeader[0] >> 3;

    aSamplingFrqIndex = InBuffHeader[0];
    aSamplingFrqIndex = aSamplingFrqIndex << 8;
    aSamplingFrqIndex = aSamplingFrqIndex | InBuffHeader[1];
    aSamplingFrqIndex = ( (aSamplingFrqIndex >> 7) & 0xF);

    LOGE_AAC_OMX("In ParserAACHeader :: aAudioObjectType = %d, aSamplingFrqIndex = %d, InBuffHeaderSize = %d",aAudioObjectType,aSamplingFrqIndex, InBuffHeaderSize);

    if(aSamplingFrqIndex == 0xf)
    {
	    if(InBuffHeaderSize >= 5)
	    {
		    iSamplingRate = ( (InBuffHeader[1] << 24) | (InBuffHeader[2] << 16) | (InBuffHeader[3] << 8) | (InBuffHeader[4]) );
		    iSamplingRate = ( ( iSamplingRate  >> 9) & 0xFFFFFF);

		    aHeaderChannelInfo = ( ( InBuffHeader[4] >> 3) & 0xF);
		    iChannels = aHeaderChannelInfo;
		    LOGE_AAC_OMX("In ParserAACHeader::A iSamplingRate = %d, iChannels = %d",iSamplingRate,iChannels);
	    	return OMX_TRUE;
	    }
	    else
	    {
		    LOGE_AAC_OMX("In ParserAACHeader:: Error: Header size is smaller than expected!!");
		    return OMX_FALSE;
	    }
    }
    else
    {
	    iSamplingRate = AACSamplingRateTbl[aSamplingFrqIndex];
	    aHeaderChannelInfo = ( ( InBuffHeader[1] >> 3) & 0xF);

	    iChannels = aHeaderChannelInfo;
	    LOGE_AAC_OMX("In ParserAACHeader::B iSamplingRate = %d, iChannels = %d",iSamplingRate,iChannels);

    }

    /* Incase if the number of channels is zero then find out the front channel elements
     * information from the extended header type(4th byte)  */
    if((iChannels == 0) && (InBuffHeaderSize >= 5))
    {
		aHeaderChannelInfo = (( InBuffHeader[3] >> 2) & 0xF);
		iChannels = aHeaderChannelInfo;

		LOGE_AAC_OMX("In ParserAACHeader::C iSamplingRate = %d, iChannels = %d",iSamplingRate,iChannels);
	}

	return OMX_TRUE;

}

