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
#include "g711_dec.h"

#include "g711_frm_code.h"
#include <utils/Log.h>

#define ENABLE_LOG
#ifdef ENABLE_LOG
#include <utils/Log.h>
#define LOGE_G711_OMX LOGE
#else
#define LOGE_G711_OMX //LOGE
#endif

OmxG711Decoder::OmxG711Decoder()
{
    iInputUsedLength = 0;
    iG711InitFlag = 0;
}

/* Decoder Initialization function */
OMX_BOOL OmxG711Decoder::G711DecInit()
{
	memset(&sG711D, 0 , sizeof(sG711D_INFO) );
	sG711D.lawmode = 'A' ;					/* A/u_law select    */
	sG711D.block_size = 160 ;	/* block size select */
	/* Initialization */
	d_rv = sG711CreateDec(&sG711D);
	if (d_rv != 0)
	{
		switch(d_rv)
		{
			case SG711_ERR_LAWMODE:
				LOGE_G711_OMX("** Error: Invalid mode : %c_Law [A or u Law]\n",sG711D.lawmode);
				break;
		}
		return OMX_FALSE;
	}
#ifdef _CODEC_LIB_ALLOC_BUFFER_ // Codec Library allocatec buffer, which causes a mem leak. To avoid the same	
	pCodecLibInbuf = sG711D.inp_buf ;
	pCodecLibOutbuf = sG711D.out_buf ; 
#endif // _CODEC_LIB_ALLOC_BUFFER_
    iG711InitFlag = 0;
    iInputUsedLength = 0;

    return OMX_TRUE;
}


/* Decoder De-Initialization function */
void OmxG711Decoder::G711DecDeinit()
{
#ifdef _CODEC_LIB_ALLOC_BUFFER_ // Codec Library allocatec buffer, which causes a mem leak. To avoid the same
	sG711D.inp_buf = pCodecLibInbuf ;
	sG711D.out_buf = pCodecLibOutbuf ;
#endif // _CODEC_LIB_ALLOC_BUFFER_
	sG711DeleteDec(&sG711D);
	memset(&sG711D, 0 , sizeof(sG711D_INFO) );
	
	iG711InitFlag = 0;
}


void OmxG711Decoder::ResetDecoder()
{
#ifdef _CODEC_LIB_ALLOC_BUFFER_ // Codec Library allocatec buffer, which causes a mem leak. To avoid the same
	sG711D.inp_buf = pCodecLibInbuf ;
	sG711D.out_buf = pCodecLibOutbuf ;
#endif	// _CODEC_LIB_ALLOC_BUFFER_
	sG711DeleteDec(&sG711D);
	
	d_rv = sG711CreateDec(&sG711D);
	if (d_rv != 0)
	{
		switch(d_rv)
		{
			case SG711_ERR_LAWMODE:
				LOGE_G711_OMX("** Error: Invalid mode : %c_Law [A or u Law]\n",sG711D.lawmode);
				break;
		}
	}
#ifdef _CODEC_LIB_ALLOC_BUFFER_ // Codec Library allocatec buffer, which causes a mem leak. To avoid the same	
	pCodecLibInbuf = sG711D.inp_buf ;
	pCodecLibOutbuf = sG711D.out_buf ; 
#endif // _CODEC_LIB_ALLOC_BUFFER_

}

/* Decode function for all the input formats */
int OmxG711Decoder::G711DecodeFrame(OMX_S16* aOutBuff,
		                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
		                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
		                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
		                               OMX_AUDIO_PARAM_G711TYPE* aAudioG711Param,
		                               OMX_BOOL aMarkerFlag,
		                               OMX_BOOL* aResizeFlag)
{
    int ret;
    *aResizeFlag = OMX_FALSE;
	
	if ( *aInBufSize == 0 )
	{
		LOGE_G711_OMX(" Zero Size Input - Invalid ");
		return G711DEC_INVALID_FRAME;
	}
    if(iG711InitFlag == 0)
    {
	    if(*aFrameCount != 0)
        {
	        //Do we need to do anything here???
	        iInputUsedLength = 0;
        }
        iG711InitFlag = 1;
    }
	
	LOGE_G711_OMX("********** Input Buffer Size: %d ******** Frame Count: %d , Block Size= %d ***********",*aInBufSize,*aFrameCount,sG711D.block_size);
	LOGE_G711_OMX("***************** %x  %x  %x  %x  ******************",*(*aInputBuf + iInputUsedLength),*((*aInputBuf + iInputUsedLength)+1),*((*aInputBuf + iInputUsedLength)+2),*((*aInputBuf + iInputUsedLength)+3));
	sG711D.block_size = 160;
	sG711D.inp_buf = (unsigned char  *)(*aInputBuf + iInputUsedLength);
	sG711D.out_buf = ( short *)aOutBuff ;
    /* Process input with compressed samples */
	sG711DecFrame(&sG711D);
	*aInBufSize -= 160;
    if(*aInBufSize == 0)//decoder expects only one frame data. Not more than that.
    {
	    iInputUsedLength = 0;
    }
    else
    {
	    iInputUsedLength += 160;
    }
    *aOutputLength = 160 ;

    //After decoding the first frame, update all the input & output port settings
    if (0 == *aFrameCount )
    {
        //Output Port Parameters
        aAudioPcmParam->nSamplingRate = 8000;
        aAudioPcmParam->nChannels = 1;//Need to calculate this

        //Input Port Parameters
        aAudioG711Param->nSampleRate = 8000;

        //Set the Resize flag to send the port settings changed callback
        *aResizeFlag = OMX_TRUE;
    }
    (*aFrameCount)++;
	LOGE_G711_OMX("Sucessfully Out Of the DecodeFrame");
    return G711DEC_SUCCESS;
}
