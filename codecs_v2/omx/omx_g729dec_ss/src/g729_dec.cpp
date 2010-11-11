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
#include "g729_dec.h"

#include "g729_frm_code.h"
#include <utils/Log.h>

#define ENABLE_LOG
#ifdef ENABLE_LOG
#include <utils/Log.h>
#define LOGE_G729_OMX LOGE
#else
#define LOGE_G729_OMX //LOGE
#endif

OmxG729Decoder::OmxG729Decoder()
{
    iInputUsedLength = 0;
    iG729InitFlag = 0;
}

/* Decoder Initialization function */
OMX_BOOL OmxG729Decoder::G729DecInit()
{
	memset(&sG729D, 0 , sizeof(sG729D_INFO) );


	sG729D.p_in = InBuf;
	sG729D.p_out = OutBuf;

	/* Initialization */


	#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729D_handler = sG729CreateDec(&sG729D);
	#else
		sG729CreateDec(&sG729D);
	#endif

    iG729InitFlag = 0;
    iInputUsedLength = 0;

    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxG729Decoder::G729DecDeinit()
{

#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
	sG729DeleteDec(sG729D_handler, &sG729D);
#else
	sG729DeleteDec(&sG729D);
#endif
	memset(&sG729D, 0 , sizeof(sG729D_INFO) );

	iG729InitFlag = 0;
}


void OmxG729Decoder::ResetDecoder()
{
	#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729DeleteDec(sG729D_handler, &sG729D);
	#else
		sG729DeleteDec(&sG729D);
	#endif


	sG729D.p_in = InBuf;
	sG729D.p_out = OutBuf;

	/* Initialization */

	#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729D_handler = sG729CreateDec(&sG729D);
	#else
		sG729CreateDec(&sG729D);
	#endif

}

/* Decode function for all the input formats */
int OmxG729Decoder::G729DecodeFrame(OMX_S16* aOutBuff,
		                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
		                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
		                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
		                               OMX_AUDIO_PARAM_G729TYPE* aAudioG729Param,
		                               OMX_BOOL aMarkerFlag,
		                               OMX_BOOL* aResizeFlag)
{
    int ret;
    *aResizeFlag = OMX_FALSE;

	if ( *aInBufSize == 0 )
	{
        LOGE_G729_OMX(" --- G729 Decoder::: Zero Size Input - Invalid  ---");
		return G729DEC_INVALID_FRAME;
	}
    if(iG729InitFlag == 0)
    {
	    if(*aFrameCount != 0)
        {
	        //Do we need to do anything here???
	        iInputUsedLength = 0;
        }
        iG729InitFlag = 1;
        iframe = 0;
    }

	sG729D.p_in = (unsigned char  *)(*aInputBuf + iInputUsedLength);
	sG729D.p_out = ( short *)aOutBuff ;
    /* Process input with compressed samples */

	#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
		sG729DecFrame(sG729D_handler, &sG729D);
	#else
		sG729DecFrame(&sG729D);
	#endif

	iframe++ ;

    *aInBufSize -= INPUT_FRAME_SIZE ;
    if(*aInBufSize == 0)//decoder expects only one frame data. Not more than that.
    {
	    iInputUsedLength = 0;
    }
    else
    {
        iInputUsedLength += INPUT_FRAME_SIZE;
    }
    *aOutputLength = L_FRAME ;

    //After decoding the first frame, update all the input & output port settings
    if (0 == *aFrameCount )
    {
        //Output Port Parameters
        aAudioPcmParam->nSamplingRate = 8000;
        aAudioPcmParam->nChannels = 1;//Need to calculate this


        //Set the Resize flag to send the port settings changed callback
        *aResizeFlag = OMX_TRUE;
    }
    (*aFrameCount)++;

	//LOGE_G729_OMX("Sucessfully Out Of the DecodeFrame");
    return G729DEC_SUCCESS;
}
