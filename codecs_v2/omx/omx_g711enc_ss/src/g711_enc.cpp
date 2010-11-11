/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/

#include "g711_enc.h"


OmxG711Encoder::OmxG711Encoder()
{
	LOGE_G711_OMX("OmxG711Encoder::OmxG711Encoder In");
	iStartTime						= 0;
    iStopTime						= 0;
    iNextStartTime					= 0;

    //iOutputFormat			  = PVMF_MIME_G711;
    //ipSizeArrayForOutputFrames = NULL;

    iMaxNumOutputFramesPerBuffer = MAX_NUM_OUTPUT_FRAMES_PER_BUFFER;
    iOneInputFrameLength		 = 320;
    //iMaxInputSize				 = 0;
    //iG711InitFlag 				 = 0;
    LOGE_G711_OMX("OmxG711Encoder::OmxG711Encoder Out");
}


/* Decoder Initialization function */
OMX_BOOL OmxG711Encoder::G711EncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                                   OMX_AUDIO_PARAM_G711TYPE aG711Param,
                                   OMX_U32* aInputFrameLength,
                                   OMX_U32* aMaxNumberOutputFrames)
{
    //OMX_U32	MaxOutputBufferSize;
    OMX_U32 RetCode = 0;

    LOGE_G711_OMX("OmxG711Encoder::G711EncInit In");

    //iG711InitFlag = 0;

    //iOutputFormat = PVMF_MIME_G711;


    //Adding all the param verification here before allocating them into ipEncProps
    if ((16 != aPcmMode.nBitPerSample) ||
            (8000 != aPcmMode.nSamplingRate) ||
            (1 != aPcmMode.nChannels))
    {
	    LOGE_G711_OMX("OmxG711Encoder::G711EncInit Error 1");
        return OMX_FALSE;
    }


    // initialize the G711 encoder
    //MaxOutputBufferSize = iMaxNumOutputFramesPerBuffer * MAX_G711_FRAME_SIZE;
    ipG711Encoder.lawmode = 'A';		/* A/u_law select    */
	ipG711Encoder.block_size = 160;		/* block size select */
	ipG711Encoder.inp_buf = NULL ;
	ipG711Encoder.out_buf = NULL ;

	RetCode = sG711CreateEnc(&ipG711Encoder);
	if(RetCode != 0)
    {
	    switch(RetCode)
		{
			case SG711_ERR_LAWMODE:
				LOGE_G711_OMX("OmxG711Encoder::G711EncInit Error 2 Invalid mode %c",ipG711Encoder.lawmode);
			break;
		}
		LOGE_G711_OMX("OmxG711Encoder::G711EncInit Error 3");
        return OMX_FALSE;
    }

    //ipSizeArrayForOutputFrames = (int32*) oscl_malloc(iMaxNumOutputFramesPerBuffer * sizeof(int32));
    //oscl_memset(ipSizeArrayForOutputFrames, 0, iMaxNumOutputFramesPerBuffer * sizeof(int32));

    //iOneInputFrameLength = G711_FRAME_LENGTH_IN_TIMESTAMP * ipEncProps->iInSamplingRate * ipEncProps->iInBitsPerSample / 8000;
    iOneInputFrameLength = 320;
    //iMaxInputSize = iMaxNumOutputFramesPerBuffer * iOneInputFrameLength;

    *aInputFrameLength = iOneInputFrameLength;

    *aMaxNumberOutputFrames = iMaxNumOutputFramesPerBuffer;

    LOGE_G711_OMX("OmxG711Encoder::G711EncInit Out");
    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxG711Encoder::G711EncDeinit()
{
	LOGE_G711_OMX("OmxG711Encoder::G711EncDeInit In \n\n\n");

	ipG711Encoder.inp_buf = NULL ;
	ipG711Encoder.out_buf = NULL ;

    sG711DeleteEnc(&ipG711Encoder);

	/*if(ipSizeArrayForOutputFrames)
    {
        oscl_free(ipSizeArrayForOutputFrames);
        ipSizeArrayForOutputFrames = NULL;
    }*/
    LOGE_G711_OMX("OmxG711Encoder::G711EncDeInit Out");
}


/* Decode function for all the input formats */
OMX_BOOL OmxG711Encoder::G711EncodeFrame(OMX_U8*    aOutputBuffer,
                                       OMX_U32*   aOutputLength,
                                       OMX_U8*    aInBuffer,
                                       OMX_U32    aInBufSize,
                                       OMX_TICKS  aInTimeStamp,
                                       OMX_TICKS* aOutTimeStamp)
{
    int32 InputFrameNum;

    LOGE_G711_OMX("OmxG711Encoder::G711EncodeFrame In");

    //Calculate the number of input frames to be encoded
    InputFrameNum = aInBufSize / iOneInputFrameLength;
    LOGE_G711_OMX("OmxG711Encoder::G711EncodeFrame InputFrameNum = %d",InputFrameNum);

    // required for next frame timestamp
	iStartTime			= (iNextStartTime >= aInTimeStamp  ? iNextStartTime : aInTimeStamp);
    iStopTime			= iStartTime + G711_FRAME_LENGTH_IN_TIMESTAMP * InputFrameNum; // 20ms
    iNextStartTime		= iStopTime; // for the next encoding

    //Encoding of Multiple frames at one time
	for(int i = 0; i < InputFrameNum; i++)
	{
		LOGE_G711_OMX("OmxG711Encoder::G711EncodeFrame InLoop = %d",i);

		ipG711Encoder.inp_buf = (short *)(aInBuffer+(iOneInputFrameLength*i));
		ipG711Encoder.out_buf = aOutputBuffer;

		// Encoding single input frame
		sG711EncFrame(&ipG711Encoder);

		// Then incrementing aOutputBuffer for next output frame
		aOutputBuffer = aOutputBuffer + ipG711Encoder.block_size;

		// Total output buffer length in each call to loop
 		*aOutputLength += ipG711Encoder.block_size;
	}

	//Set the output buffer timestamp equal to the input buffer start time.
    *aOutTimeStamp = iStartTime;

    LOGE_G711_OMX("OmxG711Encoder::G711EncodeFrame Out");
    return OMX_TRUE;
}



