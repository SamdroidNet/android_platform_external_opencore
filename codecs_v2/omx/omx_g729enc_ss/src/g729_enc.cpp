/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/

#include "g729_enc.h"


OmxG729Encoder::OmxG729Encoder()
{
	LOGE_G729_OMX("OmxG729Encoder::OmxG729Encoder In");
	iStartTime						= 0;
    iStopTime						= 0;
    iNextStartTime					= 0;

    //iOutputFormat			  = PVMF_MIME_G729;
    ipSizeArrayForOutputFrames = NULL;

    iMaxNumOutputFramesPerBuffer = MAX_NUM_OUTPUT_FRAMES_PER_BUFFER;
    iOneInputFrameLength		 = 160;
    iMaxInputSize				 = 0;
    iG729InitFlag 				 = 0;
    LOGE_G729_OMX("OmxG729Encoder::OmxG729Encoder Out");
}


/* Decoder Initialization function */
OMX_BOOL OmxG729Encoder::G729EncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                                   OMX_AUDIO_PARAM_G729TYPE aG729Param,
                                   OMX_U32* aInputFrameLength,
                                   OMX_U32* aMaxNumberOutputFrames)
{
    OMX_U32	MaxOutputBufferSize;
    OMX_U32 RetCode = 0;

    LOGE_G729_OMX("OmxG729Encoder::G729EncInit In");

    iG729InitFlag = 0;

    //iOutputFormat = PVMF_MIME_G729;


    //Adding all the param verification here before allocating them into ipEncProps
    if ((16 != aPcmMode.nBitPerSample) ||
            (8000 != aPcmMode.nSamplingRate) ||
            (1 != aPcmMode.nChannels))
    {
	    LOGE_G729_OMX("OmxG729Encoder::G729EncInit Error 1");
        return OMX_FALSE;
    }


    // initialize the G729 encoder
    MaxOutputBufferSize = iMaxNumOutputFramesPerBuffer * MAX_G729_FRAME_SIZE;

	ipG729Encoder.p_in  = NULL ;
	ipG729Encoder.p_out = NULL ;


#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729E_handler = sG729CreateEnc(&ipG729Encoder);
#else
		sG729CreateEnc(&ipG729Encoder);
#endif

    ipSizeArrayForOutputFrames = (int32*) oscl_malloc(iMaxNumOutputFramesPerBuffer * sizeof(int32));
    oscl_memset(ipSizeArrayForOutputFrames, 0, iMaxNumOutputFramesPerBuffer * sizeof(int32));

    //iOneInputFrameLength = G729_FRAME_LENGTH_IN_TIMESTAMP * ipEncProps->iInSamplingRate * ipEncProps->iInBitsPerSample / 8000;
    iOneInputFrameLength = 160;
    iMaxInputSize = iMaxNumOutputFramesPerBuffer * iOneInputFrameLength;

    *aInputFrameLength = iOneInputFrameLength;

    *aMaxNumberOutputFrames = iMaxNumOutputFramesPerBuffer;

    LOGE_G729_OMX("OmxG729Encoder::G729EncInit Out");
    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxG729Encoder::G729EncDeinit()
{
	LOGE_G729_OMX("OmxG729Encoder::G729EncDeInit In \n\n\n");

	ipG729Encoder.p_in = NULL ;
	ipG729Encoder.p_out = NULL ;

#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729DeleteEnc(sG729E_handler, &ipG729Encoder);
#else
		sG729DeleteEnc(&ipG729Encoder);
#endif

	if(ipSizeArrayForOutputFrames)
    {
        oscl_free(ipSizeArrayForOutputFrames);
        ipSizeArrayForOutputFrames = NULL;
    }
    LOGE_G729_OMX("OmxG729Encoder::G729EncDeInit Out");
}


/* Decode function for all the input formats */
OMX_BOOL OmxG729Encoder::G729EncodeFrame(OMX_U8*    aOutputBuffer,
                                       OMX_U32*   aOutputLength,
                                       OMX_U8*    aInBuffer,
                                       OMX_U32    aInBufSize,
                                       OMX_TICKS  aInTimeStamp,
                                       OMX_TICKS* aOutTimeStamp)
{
    int32 InputFrameNum;

    LOGE_G729_OMX("OmxG729Encoder::G729EncodeFrame In");

    //Calculate the number of input frames to be encoded
    InputFrameNum = aInBufSize / iOneInputFrameLength;
    LOGE_G729_OMX("OmxG729Encoder::G729EncodeFrame InputFrameNum = %d",InputFrameNum);

    // required for next frame timestamp
	iStartTime			= (iNextStartTime >= aInTimeStamp  ? iNextStartTime : aInTimeStamp);
    iStopTime			= iStartTime + G729_FRAME_LENGTH_IN_TIMESTAMP * InputFrameNum; // 20ms
    iNextStartTime		= iStopTime; // for the next encoding

    //Encoding of Multiple frames at one time
	for(int i = 0; i < InputFrameNum; i++)
	{
		LOGE_G729_OMX("OmxG729Encoder::G729EncodeFrame InLoop = %d",i);

		ipG729Encoder.p_in  = (short *)(aInBuffer+(iOneInputFrameLength*i));
		ipG729Encoder.p_out  = aOutputBuffer;

		// Encoding single input frame

#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729EncFrame(sG729E_handler, &ipG729Encoder);
#else
		sG729EncFrame(&ipG729Encoder);
#endif
		// Then incrementing aOutputBuffer for next output frame
		aOutputBuffer = aOutputBuffer + 10;

		// Total output buffer length in each call to loop
 		*aOutputLength += 10;
	}

	//Set the output buffer timestamp equal to the input buffer start time.
    *aOutTimeStamp = iStartTime;

    LOGE_G729_OMX("OmxG729Encoder::G729EncodeFrame Out");
    return OMX_TRUE;
}



