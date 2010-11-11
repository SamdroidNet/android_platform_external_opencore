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

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/

#include "amr_enc.h"
#include <stdlib.h>

//#define ENABLE_LOG_AMRENC_OMX
#ifdef ENABLE_LOG_AMRENC_OMX
#include <utils/Log.h>
#define LOGE_AMRENC_OMX LOGE
#else
#define LOGE_AMRENC_OMX // LOGE
#endif

OmxAmrEncoder::OmxAmrEncoder()
{
    // Codec and encoder setting structure

	iStartTime						= 0;
    iStopTime						= 0;
    iNextStartTime					= 0;

    iMaxNumOutputFramesPerBuffer	= MAX_NUM_OUTPUT_FRAMES_PER_BUFFER;
    iOneInputFrameLength			= AMR_ONE_INPUTFRAME_DEFAULT_LENGTH;

#ifdef DEBUG_AMR_ENC_INFO
	fp	= fopen("/data/fileDebugLSIAMR.txt", "w");
	frameCount						= 0;
#endif // DEBUG_AMR_ENC_INFO

}


/* Encoder Initialization function */
OMX_BOOL OmxAmrEncoder::AmrEncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                                   OMX_AUDIO_PARAM_AMRTYPE aAmrParam,
                                   OMX_U32* aInputFrameLength,
                                   OMX_U32* aMaxNumberOutputFrames)
{
	LOGE_AMRENC_OMX(" AMR Encoder :: Info :: Entering in AmrEncInit ");
    int ret_val = 0;

	// Encoder Bitrate Configuration
	if (OMX_AUDIO_AMRBandModeNB0 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR475;
    }
    else if (OMX_AUDIO_AMRBandModeNB1 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR515;
    }
    else if (OMX_AUDIO_AMRBandModeNB2 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR59;
    }
    else if (OMX_AUDIO_AMRBandModeNB3 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR67;
    }
    else if (OMX_AUDIO_AMRBandModeNB4 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR74;
    }
    else if (OMX_AUDIO_AMRBandModeNB5 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR795;
    }
    else if (OMX_AUDIO_AMRBandModeNB6 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR102;
    }
    else if (OMX_AUDIO_AMRBandModeNB7 == aAmrParam.eAMRBandMode)
    {
        sAMRNBEnc.rate = MODE_MR122;
    }
    else
    {
        //unsupported mode
        return OMX_FALSE;
    }


    //Adding all the param verification here
    if ((16 != aPcmMode.nBitPerSample) ||
        (8000 != aPcmMode.nSamplingRate) ||
        (1 != aPcmMode.nChannels))
    {
        return OMX_FALSE;
    }

	//Encoder Configuration
	sAMRNBEnc.p_in  = NULL;
	sAMRNBEnc.p_out = NULL;
	sAMRNBEnc.dtx   = 0;

	// Create an instance of the NB-AMR Encoder
	sAMR_NBEnc_handler = sAMR_NBCreateEnc(&ret_val, &sAMRNBEnc);
    if(ret_val != 0)
    {
		LOGE_AMRENC_OMX(" AmrEncInit :: Error :: Hardware not matched with this library ");

		return OMX_FALSE;
	}

    iOneInputFrameLength = AMR_FRAME_LENGTH_IN_TIMESTAMP * aPcmMode.nSamplingRate * aPcmMode.nBitPerSample / 8000;

    *aInputFrameLength = iOneInputFrameLength;
    *aMaxNumberOutputFrames = iMaxNumOutputFramesPerBuffer;

    LOGE_AMRENC_OMX(" AMR Encoder :: Info :: Exiting AmrEncInit ");

    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxAmrEncoder::AmrEncDeinit()
{
    LOGE_AMRENC_OMX(" AMR Encoder :: Info :: Entering AmrEncDeinit ");
#ifdef DEBUG_AMR_ENC_INFO
	if(fp)
	{
		fprintf(fp,"Inside: %s at Line: %d :: In fclose () File Pointer \n", __FUNCTION__, __LINE__);
		fclose(fp);
	}
#endif //DEBUG_AMR_ENC_INFO

    sAMR_NBDeleteEnc(sAMR_NBEnc_handler, &sAMRNBEnc);

    LOGE_AMRENC_OMX(" AMR Encoder :: Info :: Exiting AmrEncDeinit ");
}


/* Decode function for all the input formats */
OMX_BOOL OmxAmrEncoder::AmrEncodeFrame(OMX_U8*    aOutputBuffer,
                                       OMX_U32*   aOutputLength,
                                       OMX_U8*    aInBuffer,
                                       OMX_U32    aInBufSize,
                                       OMX_TICKS  aInTimeStamp,
                                       OMX_TICKS* aOutTimeStamp)
{

    LOGE_AMRENC_OMX(" AMR Encoder :: Info :: Entering AmrEncodeFrame ");

    int32 InputFrameNum;
    int enc_size = 0;

    //Calculate the number of input frames to be encoded
    InputFrameNum = aInBufSize / iOneInputFrameLength;

#ifdef DEBUG_AMR_ENC_INFO
	fprintf(fp,"Inside: %s at Line: %d :: InputFrameNum :%d \n", __FUNCTION__, __LINE__, InputFrameNum);
#endif //DEBUG_AMR_ENC_INFO

    // required for next frame timestamp
	iStartTime			= (iNextStartTime >= aInTimeStamp  ? iNextStartTime : aInTimeStamp);
    iStopTime			= iStartTime + AMR_FRAME_LENGTH_IN_TIMESTAMP * InputFrameNum; // 20ms
    iNextStartTime		= iStopTime; // for the next encoding


	//Encoding of Multiple frames at one time
	for(int i = 0; i < InputFrameNum; i++)
	{

#ifdef DEBUG_AMR_ENC_INFO
		frameCount++;
		fprintf(fp,"Inside: %s at Line: %d :: frameCount :%d \n",__FUNCTION__, __LINE__, frameCount);
		fflush(fp);
#endif //DEBUG_AMR_ENC_INFO

		sAMRNBEnc.p_in = (short *)(aInBuffer+(iOneInputFrameLength*i));
		sAMRNBEnc.p_out = aOutputBuffer;

		// Encoding single input frame
        enc_size = sAMR_NBEncFrame(sAMR_NBEnc_handler, &sAMRNBEnc);

		// Then incrementing aOutputBuffer for next output frame
		aOutputBuffer = aOutputBuffer + sAMRNBEnc.output_size;

		// Total output buffer length in each call to loop
 		*aOutputLength += sAMRNBEnc.output_size;

	}

    //Set the output buffer timestamp equal to the input buffer start time.
    *aOutTimeStamp = iStartTime;

    LOGE_AMRENC_OMX(" AMR Encoder :: Info :: Exiting AmrEncodeFrame ");

    return OMX_TRUE;
}



