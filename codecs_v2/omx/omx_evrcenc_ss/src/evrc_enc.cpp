/* ----------------------------------------------------------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name	: EVRC Encoder
 * @File Name			: evrc_enc.cpp
 * @File Description	: Source file for evrc encoder, it has all function wrappers on evrc encoder library functions.
 * @Author				: Sudhir Vyas
 * @Created Date		: 16-06-2009
 * @Modification History
 * Version:				Date:				By:				Change:
 * -----------------------------------------------------------------------------------------------------------------------
 */

#include "evrc_enc.h"

OmxEvrcEncoder::OmxEvrcEncoder()
{
	LOGE_EVRC_OMX("OmxEvrcEncoder::OmxEvrcEncoder In");
	// Intializing all variables and structures related to Encoder library
	iStartTime						= 0;
    iStopTime						= 0;
    iNextStartTime					= 0;

	ret								= 0;

    iMaxNumOutputFramesPerBuffer	= MAX_NUM_OUTPUT_FRAMES_PER_BUFFER;
    iOneInputFrameLength			= EVRC_ONE_INPUTFRAME_DEFAULT_LENGTH;

    LOGE_EVRC_OMX("OmxEvrcEncoder::OmxEvrcEncoder Out");
}

OMX_BOOL OmxEvrcEncoder::EvrcEncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                                OMX_AUDIO_PARAM_EVRCTYPE aEVRCParam,
                                OMX_U32* aInputFrameLength,
                                OMX_U32* aMaxNumberOutputFrames)
{
	LOGE_EVRC_OMX("OmxEvrcEncoder::EvrcEncInit In");

	/*****************************************************\
		Encoder Configuration
	\*****************************************************/
	sEVRCE.p_in						= NULL;
	sEVRCE.p_out					= NULL;
	sEVRCE.ibuf_len					= SPEECH_BUFFER_LEN;
	sEVRCE.obuf_len					= BITSTREAM_BUFFER_LEN - 1;
	sEVRCE.max_rate					= EVRC_MAX_FRAME_RATE;
	sEVRCE.min_rate					= EVRC_MIN_FRAME_RATE;
	sEVRCE.noise_suppression		= NOISE_SUPPRESSION;

	/*****************************************************\
		Initialize Encoder
	\*****************************************************/

	sEVRCCreateEnc(&sEVRCE);

    *aInputFrameLength				= iOneInputFrameLength;
    *aMaxNumberOutputFrames			= iMaxNumOutputFramesPerBuffer;

    LOGE_EVRC_OMX("OmxEvrcEncoder::EvrcEncInit Out");

    return OMX_TRUE;
}


void OmxEvrcEncoder::EvrcEncDeinit()
{
	LOGE_EVRC_OMX("OmxEvrcEncoder::EvrcEncDeinit In");

	ret	= sEVRCDeleteEnc(&sEVRCE);
	if (ret)
	{
		LOGE_EVRC_OMX("ERROR: sEVRCDeleteEnc failed");
	}
	memset(&sEVRCE, 0, sizeof(sEVRCE));
	LOGE_EVRC_OMX("OmxEvrcEncoder::EvrcEncDeinit Out");
}

OMX_BOOL OmxEvrcEncoder::EvrcEncodeAudio(OMX_U8*    aOutputBuffer,
										 OMX_U32*   aOutputLength,
										 OMX_U8*    aInBuffer,
										 OMX_U32    aInBufSize,
										 OMX_TICKS  aInTimeStamp,
										 OMX_TICKS* aOutTimeStamp)

{
	LOGE_EVRC_OMX("OmxEvrcEncoder::EvrcEncodeAudio In");

	int32 InputFrameNum = 0;

    //Calculate the number of input frames to be encoded

    InputFrameNum		= aInBufSize / iOneInputFrameLength;
    // required for next frame timestamp
	iStartTime			= (iNextStartTime >= aInTimeStamp  ? iNextStartTime : aInTimeStamp);
    iStopTime			= iStartTime + EVRC_FRAME_LENGTH_IN_TIMESTAMP * InputFrameNum; // 20ms
    iNextStartTime		= iStopTime; // for the next encoding

    //Encoding of Multiple frames at one time
	for(int i = 0; i < InputFrameNum; i++)
	{
		sEVRCE.p_in		= (short *)(aInBuffer+(iOneInputFrameLength*i));
		// First 2 bytes of outputput frame are for frame rate.
		sEVRCE.p_out	= (short *)(aOutputBuffer + 2);

		ret = sEVRCEncFrame(&sEVRCE);

		if (ret)
		{
			LOGE_EVRC_OMX("ERROR: sEVRCEncFrame failed");
			return OMX_FALSE;
		}
		// aOutputBuffer is 8 bit data and sEVRCE.rate is short 16 bit data,
		// So 2 bytes of sEVRCE.rate is copied to aOutputBuffer one by one i.e. MSB then LSB.
		oscl_memcpy((void *)aOutputBuffer,(void *)(&(sEVRCE.rate)),2);

		// Then incrementing aOutputBuffer for next output frame
		// sEVRCE.obuf_len is short value, so it is multiplied by 2 to convert it in bytes.
		aOutputBuffer	= aOutputBuffer + (sEVRCE.obuf_len * 2) + 2;

		// Total output buffer length in each call to loop
		// As each outputput frame is of 24 bytes and first 2 bytes are always framerate,
		// so it is incrmented by 2.
 		*aOutputLength	+= (sEVRCE.obuf_len * 2 ) + 2;
	}

	//Set the output buffer timestamp equal to the input buffer start time.
    *aOutTimeStamp = iStartTime;

    LOGE_EVRC_OMX("OmxEvrcEncoder::EvrcEncodeAudio Out");

	return OMX_TRUE;
}