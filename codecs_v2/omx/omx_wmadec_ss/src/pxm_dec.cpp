/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/

#include "oscl_mem.h"
#include "oscl_base.h"
#include "pxm_dec.h"

#define MAX_INPUT_BUFFER_SIZE_PXM 	128*100
#define MAX_OUTPUT_BUFFER_SIZE_PXM	16384

OmxPxmDecoder::OmxPxmDecoder()
{
	LOGE_PXM_OMX("In OmxPxmDecoder::OmxPxmDecoder");
	iPxmInitFlag = 0;
	iInputUsedLength = 0;
	LOGE_PXM_OMX("Out OmxPxmDecoder::OmxPxmDecoder");
}

/* Decoder Initialization function */
OMX_BOOL OmxPxmDecoder::PxmDecInit()
{
	int ret;
	LOGE_PXM_OMX("In OmxPxmDecoder::PxmDecInit");

#ifdef __PXM_USE_MEMCPY_IN__
    StreamBuff = (unsigned char*)oscl_malloc(MAX_INPUT_BUFFER_SIZE_PXM*sizeof(unsigned char));
    if(StreamBuff == NULL)
    {
	    LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecInit1 Error");
	    return OMX_FALSE;
    }
#endif//__PXM_USE_MEMCPY_IN__

#ifdef __PXM_USE_MEMCPY_OUT__
	OutBuff = (short*)oscl_malloc(MAX_OUTPUT_BUFFER_SIZE_PXM*sizeof(short));
    if(OutBuff == NULL)
    {
	    LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecInit2 Error");
	    return OMX_FALSE;
    }
#endif//__PXM_USE_MEMCPY_OUT__

    memset(&iAudioPxmDecoder,0,sizeof(SWMAD_INFO));

#ifdef __PXM_USE_MEMCPY_IN__
	iAudioPxmDecoder.pInBuff = (unsigned char*) StreamBuff;
#endif//__PXM_USE_MEMCPY_IN__

#ifdef __PXM_USE_MEMCPY_OUT__
	iAudioPxmDecoder.pOutBuff = (short*) OutBuff;
#endif//__PXM_USE_MEMCPY_OUT__

	iPxmInitFlag = 0;
	iInputUsedLength = 0;

	LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecInit3");
	return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxPxmDecoder::PxmDecDeinit()
{
	LOGE_PXM_OMX("In OmxPxmDecoder::PxmDecDeinit");
	sWMADeleteDec(&iAudioPxmDecoder);
#ifdef __PXM_USE_MEMCPY_IN__
    oscl_free(StreamBuff);
#endif//__PXM_USE_MEMCPY_IN__

#ifdef __PXM_USE_MEMCPY_OUT__
	oscl_free(OutBuff);
#endif//__PXM_USE_MEMCPY_OUT__

	iPxmInitFlag = 0;
	iInputUsedLength = 0;
	LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecDeinit");
}


void OmxPxmDecoder::ResetDecoder()
{
	LOGE_PXM_OMX("In OmxPxmDecoder::ResetDecoder");

	if(iPxmInitFlag)
	{
		sWMAResetDec(&iAudioPxmDecoder);
		iAudioPxmDecoder.pBuff_cnt = 0;
	}
	LOGE_PXM_OMX("Out OmxPxmDecoder::ResetDecoder");
}

/* Decode function for all the input formats */
int OmxPxmDecoder::PxmDecodeFrame(OMX_S16* aOutBuff,
		                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
		                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
		                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
		                               OMX_AUDIO_PARAM_WMATYPE* aAudioWmaParam,
		                               OMX_BOOL aMarkerFlag,
		                               OMX_BOOL* aResizeFlag)
{
	LOGE_PXM_OMX("In OmxPxmDecoder::PxmDecodeFrame");
	int ret;
	*aResizeFlag = OMX_FALSE;

	if(iPxmInitFlag == 0)
	{
		if(OMX_FALSE == PxmDecodeHeader((*aInputBuf)))
		{
			return -1;
		}

		ret = sWMACreateDec(&iAudioPxmDecoder);
		
		if(ret)
		{
			LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecInit3 Error");
			return -1;
		}
				
		if(*aFrameCount != 0)
		{
			//Do we need to do anything here???
		}
		
	        iPxmInitFlag = 1;
		*aInBufSize = 0;
		*aOutputLength = 0;
		iInputUsedLength = 0;
		
		return PXMDEC_INVALID_FRAME;
	}

	/* Zero length buffer can come at the end of the file */
	if(*aInBufSize == 0)
	{
		LOGE_PXM_OMX("OmxPxmDecoder::PxmDecInit ::  Zero length buffer has come");
		iInputUsedLength = 0;
		iAudioPxmDecoder.pBuff_cnt = 0;
		return PXMDEC_INVALID_FRAME;
	}

	LOGE_PXM_OMX("*aInBufSize = %d,  *aInputBuf %d",*aInBufSize,(*aInputBuf+iInputUsedLength));
	LOGE_PXM_OMX("*aInputBuf = %x %x %x %x",*(*aInputBuf+iInputUsedLength),*(*aInputBuf+iInputUsedLength+1),*(*aInputBuf+iInputUsedLength+2),*(*aInputBuf+iInputUsedLength+3));

#ifdef __PXM_USE_MEMCPY_IN__
	oscl_memcpy((void *)iAudioPxmDecoder.pInBuff,(void *)(*aInputBuf+iInputUsedLength),*aInBufSize);
#else//__PXM_USE_MEMCPY_IN__
	iAudioPxmDecoder.pInBuff = (unsigned char*) (*aInputBuf+iInputUsedLength);
#endif//__PXM_USE_MEMCPY_IN__

#ifdef __PXM_USE_MEMCPY_OUT__
#else//__PXM_USE_MEMCPY_OUT__
	iAudioPxmDecoder.pOutBuff = (short*) aOutBuff;
#endif//__PXM_USE_MEMCPY_OUT__

	LOGE_PXM_OMX("Before sWMADecode");
	LOGE_PXM_OMX("handle before calling sWMADecode = 0x%x\n", (int)(iAudioPxmDecoder.handle_MI));
	ret = sWMADecode(&iAudioPxmDecoder);
	LOGE_PXM_OMX("handle after calling sWMADecode = 0x%x\n", (int)(iAudioPxmDecoder.handle_MI));
	
	if(ret != 0)
	{
		*aInBufSize = 0;
		iInputUsedLength = 0;
		LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecodeFrame2 Error");
		return PXMDEC_INVALID_FRAME;
	}

    /*
    if(iAudioPxmDecoder.flag != SWMAD_STATUS_NEED_MORE_DECODING)
    {
	    LOGE_PXM_OMX(" No More Decoding is Needed for Current Frame");
    }
    else if(iAudioPxmDecoder.pBuff_cnt >= *aInBufSize)
    {
	    *aInBufSize = 0;
	    iAudioPxmDecoder.pBuff_cnt = 0;
	    *aOutputLength = 0;
	    LOGE_PXM_OMX("Returning for new input buffer");
	    return PXMDEC_INVALID_FRAME;
    }
    */
    
	LOGE_PXM_OMX("While Loop iAudioPxmDecoder.pBuff_cnt = %d, iAudioPxmDecoder.cbPacketSize = %d",
		iAudioPxmDecoder.pBuff_cnt,iAudioPxmDecoder.cbPacketSize);

#if 0
	/* This is assuming that we always get one frame of data from the parser */
    if(iAudioPxmDecoder.pBuff_cnt >= *aInBufSize)
    {
		iAudioPxmDecoder.pBuff_cnt = 0;
	    *aInBufSize = 0;
    }
#else

    /* This is assuming that parser can send more than one frame of data in single buffer */
    if(iAudioPxmDecoder.pBuff_cnt >= iAudioPxmDecoder.cbPacketSize)
    {
        LOGE_PXM_OMX(" Info :: received more than one frame of data :: iAudioPxmDecoder.pBuff_cnt is %d", iAudioPxmDecoder.pBuff_cnt);
		
	    iAudioPxmDecoder.pBuff_cnt = 0;
	    *aInBufSize -= iAudioPxmDecoder.cbPacketSize;
	    if(*aInBufSize == 0)//decoder expects only one frame data. Not more than that.
	    {
		    iInputUsedLength = 0;
	    }
	    else
	    {
		    iInputUsedLength += iAudioPxmDecoder.cbPacketSize;
	    }
    }
#endif

    /* We have an output buffer ready to send */
    if(iAudioPxmDecoder.flag != SWMAD_STATUS_NEED_MORE_DECODING)
    {
	    *aOutputLength = iAudioPxmDecoder.NumSmpls*iAudioPxmDecoder.Ch_x_Byte_per_sample/2;

		LOGE_PXM_OMX("iAudioPxmDecoder.Ch_x_Byte_per_sample = %d, iAudioPxmDecoder.NumSmpls = %d ",
			iAudioPxmDecoder.Ch_x_Byte_per_sample, iAudioPxmDecoder.NumSmpls);

		LOGE_PXM_OMX(" Info :: decoded output buffer length is %d ", *aOutputLength);
    }


#ifdef __PXM_USE_MEMCPY_OUT__
	oscl_memcpy((void *)aOutBuff,(void *)iAudioPxmDecoder.pOutBuff,iAudioPxmDecoder.NumSmpls*iAudioPxmDecoder.Ch_x_Byte_per_sample);//in bytes
#endif//__PXM_USE_MEMCPY_OUT__

	//After decoding the first frame, update all the input & output port settings
	if((0 == *aFrameCount) && (*aOutputLength != 0))
	{
		LOGE_PXM_OMX("iAudioPxmDecoder.Ch_x_Byte_per_sample = %d, iAudioPxmDecoder.NumSmpls = %d ",
			iAudioPxmDecoder.Ch_x_Byte_per_sample, iAudioPxmDecoder.NumSmpls);

		//Output Port Parameters
		aAudioPcmParam->nSamplingRate = iAudioPxmDecoder.nSamplesPerSec;
		aAudioPcmParam->nChannels = iAudioPxmDecoder.nChannels;

		//Input Port Parameters
		aAudioWmaParam->nSamplingRate = iAudioPxmDecoder.nSamplesPerSec;

		//Set the Resize flag to send the port settings changed callback
		*aResizeFlag = OMX_TRUE;
	}

	if(*aOutputLength != 0)
	{
		(*aFrameCount)++;
	}

	LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecodeFrame3");
	return PXMDEC_SUCCESS;
}

OMX_BOOL OmxPxmDecoder::PxmDecodeHeader(OMX_U8* aInBuffer)
{
	LOGE_PXM_OMX("In OmxPxmDecoder::PxmDecodeHeader");

	LOGE_PXM_OMX("SIZE OF sizeof(CONFIG_DATA) = %d",sizeof(CONFIG_DATA));
	
	if(aInBuffer)
	{
		oscl_memcpy((void *)(&iConfigData),(void *)(aInBuffer),sizeof(CONFIG_DATA));
	}

	iAudioPxmDecoder.cbPacketSize = iConfigData.nBlockAlign;
	
	switch (iConfigData.wFormatTag)
	{
		case 354:
		case 355:
			iAudioPxmDecoder.nVersion = 3;
		break;
		case 353:
			iAudioPxmDecoder.nVersion = 2;
		break;
		case 352:
			iAudioPxmDecoder.nVersion = 1;
		break;
		default :
		{
			LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecInit:Error Wrong Version");
			return OMX_FALSE;
		}
	}
	
	iAudioPxmDecoder.wFormatTag = iConfigData.wFormatTag;
	iAudioPxmDecoder.nChannels = iConfigData.nChannels;
	iAudioPxmDecoder.nSamplesPerSec = iConfigData.nSamplesPerSec;
	iAudioPxmDecoder.nAvgBytesPerSec = iConfigData.nAvgBytesPerSec;
	iAudioPxmDecoder.nBlockAlign = iConfigData.nBlockAlign;
	iAudioPxmDecoder.wBitsPerSample = iConfigData.wBitsPerSample;
	iAudioPxmDecoder.nSamplesPerBlock = iConfigData.nSamplesPerBlock;
	iAudioPxmDecoder.nEncodeOpt = iConfigData.nEncodeOpt;
	iAudioPxmDecoder.bHasDRM = 0;

	LOGE_PXM_OMX("iConfigData.nBlockAlign = %d, iAudioPxmDecoder.nVersion = %d iConfigData.wFormatTag = %d,iConfigData.nChannels = %d,iConfigData.nSamplesPerSec = %d,iConfigData.nAvgBytesPerSec = %d,iConfigData.nBlockAlign = %d,iConfigData.wBitsPerSample = %d,iConfigData.nSamplesPerBlock = %d,iConfigData.nEncodeOpt = %d",iConfigData.nBlockAlign,iAudioPxmDecoder.nVersion,iConfigData.wFormatTag,iConfigData.nChannels,iConfigData.nSamplesPerSec,iConfigData.nAvgBytesPerSec,iConfigData.nBlockAlign,iConfigData.wBitsPerSample,iConfigData.nSamplesPerBlock,iConfigData.nEncodeOpt);
	LOGE_PXM_OMX("Out OmxPxmDecoder::PxmDecodeHeader");

	return OMX_TRUE;
}
