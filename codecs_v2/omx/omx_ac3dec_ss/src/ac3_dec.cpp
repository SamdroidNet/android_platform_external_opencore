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
#include "ac3_dec.h"

#include "ac3_frm_code.h"

#if 1
#define MAX_INPUT_BUFFER_SIZE_AC3 4000*10
#define MAX_OUTPUT_BUFFER_SIZE_AC3 4608
#else
#define MAX_INPUT_BUFFER_SIZE_AC3 4000
#define MAX_OUTPUT_BUFFER_SIZE_AC3 4608
#endif


OmxAc3Decoder::OmxAc3Decoder()
{
	LOGE_AC3_OMX("In OmxAc3Decoder::OmxAc3Decoder");
    iInputUsedLength = 0;
    iAc3InitFlag = 0;
    iSamplingFrqReset = OMX_FALSE ;
    LOGE_AC3_OMX("Out OmxAc3Decoder::OmxAc3Decoder");
}

/* Decoder Initialization function */
OMX_BOOL OmxAc3Decoder::Ac3DecInit()
{
	LOGE_AC3_OMX("In OmxAc3Decoder::Ac3DecInit");

#ifdef __AC3_USE_MEMCPY_IN__
    StreamBuff = (unsigned char*)oscl_malloc(MAX_INPUT_BUFFER_SIZE_AC3*sizeof(unsigned char));
    if(StreamBuff == NULL)
    {
	    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecInit1 Error");
	    return OMX_FALSE;
    }
#endif//__AC3_USE_MEMCPY_IN__

#ifdef __AC3_USE_MEMCPY_OUT__
	OutBuff = (short*)oscl_malloc(MAX_OUTPUT_BUFFER_SIZE_AC3*sizeof(short));
    if(OutBuff == NULL)
    {
	    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecInit2 Error");
	    return OMX_FALSE;
    }
#endif//__AC3_USE_MEMCPY_OUT__

    memset(&iAudioAc3Decoder,0,sizeof(sAC3D_INFO));
#ifdef __AC3_USE_MEMCPY_IN__
	iAudioAc3Decoder.pInBuff = (unsigned char*) StreamBuff;
#endif//__AC3_USE_MEMCPY_IN__

#ifdef __AC3_USE_MEMCPY_OUT__
	iAudioAc3Decoder.pOutBuff = (short*) OutBuff;
#endif//__AC3_USE_MEMCPY_OUT__

	iAudioAc3Decoder.stoutacmod = 2; // 1: mono out, 2: stereo out    yesjoon 080408
	iAudioAc3Decoder.flush_set = 0; // 1: reset data, 0 : default

    sAC3CreateDec();

    iAc3InitFlag = 0;
    iInputUsedLength = 0;
	LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecInit3");
    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxAc3Decoder::Ac3DecDeinit()
{
	LOGE_AC3_OMX("In OmxAc3Decoder::Ac3DecDeinit");
	sAC3DeleteDec();
#ifdef __AC3_USE_MEMCPY_IN__
    oscl_free(StreamBuff);
#endif//__AC3_USE_MEMCPY_IN__

#ifdef __AC3_USE_MEMCPY_OUT__
	oscl_free(OutBuff);
#endif//__AC3_USE_MEMCPY_OUT__

	iAc3InitFlag = 0;
	iSamplingFrqReset = OMX_FALSE ;
	LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecDeinit");
}


void OmxAc3Decoder::ResetDecoder()
{
	LOGE_AC3_OMX("In OmxAc3Decoder::ResetDecoder");
	sAC3DeleteDec();
	sAC3CreateDec();
	LOGE_AC3_OMX("Out OmxAc3Decoder::ResetDecoder");
}

/* Decode function for all the input formats */
int OmxAc3Decoder::Ac3DecodeFrame(OMX_S16* aOutBuff,
		                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
		                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
		                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
		                               OMX_AUDIO_PARAM_AC3TYPE* aAudioAc3Param,
		                               OMX_BOOL aMarkerFlag,
		                               OMX_BOOL* aResizeFlag)
{
	LOGE_AC3_OMX("In OmxAc3Decoder::Ac3DecodeFrame");
    int ret;
    OMX_U32 headerPosition = 0;
    *aResizeFlag = OMX_FALSE;
    if(iAc3InitFlag == 0)
    {
	    if(*aFrameCount != 0)
        {
	        //Do we need to do anything here???
	        iInputUsedLength = 0;
        }
        iAc3InitFlag = 1;
    }
	if(*aInBufSize < 5)
	{
		*aInputBuf += iInputUsedLength;
		iInputUsedLength = 0;
		return AC3DEC_INCOMPLETE_FRAME;
	}
    /* This function is called because AVI parser might not give buffers starting with frame header */
    if(OMX_TRUE != Ac3SearchHeader((*aInputBuf+iInputUsedLength),*aInBufSize,&headerPosition))
    {
	    iInputUsedLength = 0;
	    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame1 Error");
	    return AC3DEC_INVALID_FRAME;
    }

	OMX_U8* tmp_for_log = (*aInputBuf+iInputUsedLength+headerPosition);

    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame [%02x %02x %02x %02x] after Ac3SearchHeader aInBufSize = %d, iInputUsedLength = %d, headerPosition = %d", *tmp_for_log++, *tmp_for_log++, *tmp_for_log++, *tmp_for_log++,
		*aInBufSize, iInputUsedLength,headerPosition);
    /* Need to scan the header for first time */
#ifdef __AC3_PARSE_HEADER_ALWAYS__
	if(OMX_TRUE != Ac3DecodeHeader((*aInputBuf+iInputUsedLength+headerPosition)))
    {
	    iInputUsedLength = 0;
		iSamplingFrqReset = OMX_FALSE;
	    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame2 Error");
	    return AC3DEC_INVALID_FRAME;
    }
#else
	if(0 == *aFrameCount)
    {
	    //scan the header here
	    if(OMX_TRUE != Ac3DecodeHeader((*aInputBuf+iInputUsedLength+headerPosition)))
	    {
		    iInputUsedLength = 0;
		    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame3 Error");
		    return AC3DEC_INVALID_FRAME;
	    }
    }
#endif //__AC3_PARSE_HEADER_ALWAYS__

    if((iAc3FrameSize+headerPosition) > *aInBufSize)//we dont have full frame to process
    {
		*aInputBuf += iInputUsedLength;
        iInputUsedLength = 0;
		iSamplingFrqReset = OMX_FALSE;
        LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame4 Error");
        return AC3DEC_INCOMPLETE_FRAME;
    }
	
#ifdef __AC3_USE_MEMCPY_IN__
	oscl_memcpy((void *)StreamBuff,(void *)(*aInputBuf+iInputUsedLength+headerPosition),iAc3FrameSize);
#else//__AC3_USE_MEMCPY_IN__
	iAudioAc3Decoder.pInBuff = (unsigned char*) (*aInputBuf+iInputUsedLength+headerPosition);
#endif//__AC3_USE_MEMCPY_IN__

#ifdef __AC3_USE_MEMCPY_OUT__
#else//__AC3_USE_MEMCPY_OUT__
	iAudioAc3Decoder.pOutBuff = (short*) aOutBuff;
#endif//__AC3_USE_MEMCPY_OUT__

    iAudioAc3Decoder.InbuffSize = iAc3FrameSize;

    ret = sAC3DecFrame(&iAudioAc3Decoder);
    if(ret != 0)
    {
		*aInBufSize = 0;
        iInputUsedLength = 0;
        (*aFrameCount)++;
		iSamplingFrqReset = OMX_FALSE;
        LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame5 Error");
	    return AC3DEC_INVALID_FRAME;
    }
    *aInBufSize -= (iAudioAc3Decoder.InbuffSize+headerPosition);

    if(*aInBufSize == 0)//decoder expects only one frame data. Not more than that.
    {
	    iInputUsedLength = 0;
    }
    else
    {
	    iInputUsedLength += (iAudioAc3Decoder.InbuffSize+headerPosition);
    }

    *aOutputLength = iAudioAc3Decoder.nOutPCM;

    LOGE_AC3_OMX("Info OmxAc3Decoder::decoded o/p length is %d, iInputUsedLength = %d",  *aOutputLength, iInputUsedLength);

#ifdef __AC3_USE_MEMCPY_OUT__
	oscl_memcpy((void *)aOutBuff,(void *)OutBuff,iAudioAc3Decoder.nOutPCM*2);//in bytes
#endif//__AC3_USE_MEMCPY_OUT__

	iAc3SmplFreq = iTmpSmplFreq;

    //After decoding the first frame, update all the input & output port settings
    if (0 == *aFrameCount || iSamplingFrqReset == OMX_TRUE )
    {
        //Output Port Parameters
        aAudioPcmParam->nSamplingRate = iAc3SmplFreq;
        aAudioPcmParam->nChannels = 2;//Need to calculate this

        //Input Port Parameters
        aAudioAc3Param->nSampleRate = iAc3SmplFreq;

        //Set the Resize flag to send the port settings changed callback
        *aResizeFlag = OMX_TRUE;
    }
    (*aFrameCount)++;

    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeFrame6");

    return AC3DEC_SUCCESS;
}

OMX_BOOL OmxAc3Decoder::Ac3DecodeHeader(OMX_U8* aInBuffer)
{
	LOGE_AC3_OMX("In OmxAc3Decoder::Ac3DecodeHeader");
	OMX_S16* aSyncWord = (OMX_S16*)aInBuffer;
	unsigned char aSmplFreq = aInBuffer[4];
	unsigned char aFrameSzCode = aInBuffer[4];

	uint32 iTempSmpFrq = 0;

	if((aInBuffer[0] != 0x0B) || (aInBuffer[1] != 0x77))
    {
	    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeHeader1 Error");
	    return OMX_FALSE;
    }

    aSmplFreq = aSmplFreq >> 6;
    aFrameSzCode = (aFrameSzCode & 0x3F);
	
	if(aFrameSzCode > 37)
	{
		return OMX_FALSE;
	}
		
    if(aSmplFreq == 2)
    {
        iTempSmpFrq = 32000; //Duration : 48ms
        iAc3FrameSize = codesz32[aFrameSzCode]*2;
    }
    else if(aSmplFreq == 1)
    {
        iTempSmpFrq = 44100;//Duration : 34.83ms
        iAc3FrameSize = codesz441[aFrameSzCode]*2;
    }
    else if (aSmplFreq == 0)
    {
        iTempSmpFrq = 48000;//Duration : 32ms
        iAc3FrameSize = codesz48[aFrameSzCode]*2;
    }
    else
    {
	    LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeHeader2 Error");
	    return OMX_FALSE;
    }

	if(iTempSmpFrq != iAc3SmplFreq)
	{
		iSamplingFrqReset = OMX_TRUE;
	}
	else
	{
		iSamplingFrqReset = OMX_FALSE;
	}
//	iAc3SmplFreq = iTempSmpFrq;
	iTmpSmplFreq = iTempSmpFrq;

	LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3DecodeHeader3");
	return OMX_TRUE;
}

OMX_BOOL OmxAc3Decoder::Ac3SearchHeader(OMX_U8* aInBuffer, OMX_U32 aInputBuffLength, OMX_U32* aHeaderPosition)
{
	LOGE_AC3_OMX("In OmxAc3Decoder::Ac3SearchHeader");
	OMX_U32 count = 0;

	*aHeaderPosition = 0;
	for(count = 0; count < aInputBuffLength; count++)
	{
		if((aInBuffer[count] == 0x0B) && (aInBuffer[count+1] == 0x77))
		{
			*aHeaderPosition = count;
			LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3SearchHeader1, header position = %d",count);
			return OMX_TRUE;
		}
	}
	LOGE_AC3_OMX("Out OmxAc3Decoder::Ac3SearchHeader2 Error");
	return OMX_FALSE;
}
