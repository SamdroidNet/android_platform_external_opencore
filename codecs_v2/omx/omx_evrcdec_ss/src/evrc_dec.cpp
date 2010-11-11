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
#include "evrc_dec.h"

#include <utils/Log.h>

#define MAX_INPUT_BUFFER_SIZE_EVRC 4000
#define MAX_OUTPUT_BUFFER_SIZE_EVRC 4608

#define ENABLE_LOG
#ifdef ENABLE_LOG
#include <utils/Log.h>
#define LOGE_EVRC_OMX LOGE
#else
#define LOGE_EVRC_OMX //LOGE
#endif

OmxEvrcDecoder::OmxEvrcDecoder()
{
    iInputUsedLength = 0;
    EvrcInitFlag = 0;
    iSamplingFrqReset = OMX_FALSE ;
}

/* Decoder Initialization function */
OMX_BOOL OmxEvrcDecoder::EvrcDecInit()
{
    memset(&sEVRCD, 0 , sizeof(sEVRC_INFO));
	sEVRCD.post_filter =1;
	/* Initialization */
	sEVRCCreateDec(&sEVRCD);
    EvrcInitFlag = 0;
    iInputUsedLength = 0;

    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxEvrcDecoder::EvrcDecDeinit()
{
	sEVRCDeleteDec(&sEVRCD);
	memset(&sEVRCD, 0 , sizeof(sEVRC_INFO) );
	EvrcInitFlag = 0;
	iSamplingFrqReset = OMX_FALSE ;
}


void OmxEvrcDecoder::ResetDecoder()
{
	sEVRCDeleteDec(&sEVRCD);
	sEVRCCreateDec(&sEVRCD);
}

/* Decode function for all the input formats */
int OmxEvrcDecoder::EvrcDecodeFrame(OMX_S16* aOutBuff,
		                               OMX_U32* aOutputLength, OMX_S16** aInputBuf,
		                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
		                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
		                               OMX_AUDIO_PARAM_EVRCTYPE* aAudioEvrcParam,
		                               OMX_BOOL aMarkerFlag,
		                               OMX_BOOL* aResizeFlag)
{
    int ret;
    *aResizeFlag = OMX_FALSE;
	short *temp = (short *)(*aInputBuf) ;
	if ( *aInBufSize == 0 )
	{
		LOGE_EVRC_OMX(" Zero Size Input - Invalid ");
		return EVRCDEC_INVALID_FRAME;
	}
    if(EvrcInitFlag == 0)
    {
	    if(*aFrameCount != 0)
        {
	        //Do we need to do anything here???
	        iInputUsedLength = 0;
        }
        EvrcInitFlag = 1;
    }

#ifdef DUMP_EVRC_PARSER_DATA
    FILE* fp = fopen("/sdcard/EVRCencodefile.evrc", "ab+");
	fwrite(*aInputBuf, sizeof(short), 12, fp);
	fclose(fp);
#endif //DUMP_EVRC_PARSER_DATA

	/* Process input with compressed samples */
	sEVRCD.rate = temp[0];
	sEVRCD.p_in = ( short *)(temp + 1 );
	//sEVRCD.p_in = ( short *)(*aInputBuf + 2);
	sEVRCD.p_out = ( short *)aOutBuff;
	sEVRCD.ibuf_len = BITSTREAM_BUFFER_LEN - 1;
	sEVRCD.obuf_len = SPEECH_BUFFER_LEN;
	sEVRCD.post_filter = 0;
	sEVRCDecFrame(&sEVRCD);
#ifdef DUMP_EVRC_DECODED_DATA
	FILE* fp1 = fopen("/sdcard/EVRCDecodedfile.pcm", "ab+");
	fwrite(sEVRCD.p_out, sizeof(short), sEVRCD.obuf_len, fp1);
	fclose(fp1);
#endif //DUMP_EVRC_DECODED_DATA
//	usleep(2000);
	*aInBufSize -= (BITSTREAM_BUFFER_LEN * 2);
    if(*aInBufSize == 0)//decoder expects only one frame data. Not more than that.
    {
	    iInputUsedLength = 0;
    }
    else
    {
	    iInputUsedLength += BITSTREAM_BUFFER_LEN;
    }
    *aOutputLength = SPEECH_BUFFER_LEN;

    //After decoding the first frame, update all the input & output port settings
    if (0 == *aFrameCount || iSamplingFrqReset == OMX_TRUE )
    {
        //Output Port Parameters
        aAudioPcmParam->nSamplingRate = 8000;
        aAudioPcmParam->nChannels = 1;//Need to calculate this

        //Input Port Parameters
        //aAudioEvrcParam->nSampleRate = 8000;

        //Set the Resize flag to send the port settings changed callback
        *aResizeFlag = OMX_TRUE;
    }
    (*aFrameCount)++;
	LOGE_EVRC_OMX("Sucessfully Out Of the DecodeFrame");
    return EVRCDEC_SUCCESS;
}

