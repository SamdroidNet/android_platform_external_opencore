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
#include "amr_dec.h"

#define AMR_MAX_SIZE 64
#define AMR_WB_MAX_SIZE 122

#define AMR_NB_OUT_SIZE 320 /* 160 samples */
#define AMR_WB_OUT_SIZE 640 /* 320 samples */

/* Compressed audio formats */
#define AMR_NB_CONF   0
#define AMR_NB_IF1    1
#define AMR_NB_IF2    2
#define AMR_NB_FSF    3
#define AMR_NB_RTP    4

#define AMR_WB_FSF    5
#define AMR_WB_RTP    6

//#define ENABLE_LOG
#ifdef ENABLE_LOG
#include <utils/Log.h>
#define LOGE_AMR_OMX LOGE
#else
#define LOGE_AMR_OMX // LOGE
#endif

// Find frame size for each frame type
static const OMX_S32 WBIETFFrameSize[16] =
{
    18		// AMR-WB 6.60 Kbps
    , 24		// AMR-WB 8.85 Kbps
    , 33		// AMR-WB 12.65 Kbps
    , 37		// AMR-WB 14.25 Kbps
    , 41		// AMR-WB 15.85 Kbps
    , 47		// AMR-WB 18.25 Kbps
    , 51		// AMR-WB 19.85 Kbps
    , 59		// AMR-WB 23.05 Kbps
    , 61		// AMR-WB 23.85 Kbps
    , 6		// AMR-WB SID
    , 1
    , 1
    , 1
	, 1
    , 1		// WBAMR Frame No Data
    , 1		// WBAMR Frame No Data
};
static const OMX_S32 IETFFrameSize[16] =
{
    13		// AMR 4.75 Kbps
    , 14	// AMR 5.15 Kbps
    , 16	// AMR 5.90 Kbps
    , 18	// AMR 6.70 Kbps
    , 20	// AMR 7.40 Kbps
    , 21	// AMR 7.95 Kbps
    , 27	// AMR 10.2 Kbps
    , 32	// AMR 12.2 Kbps
    , 6		// GsmAmr comfort noise
    , 7		// Gsm-Efr comfort noise
    , 6		// IS-641 comfort noise
    , 6		// Pdc-Efr comfort noise
    , 1		// future use; 0 length but set to 1 to skip the frame type byte
    , 1		// future use; 0 length but set to 1 to skip the frame type byte
    , 1		// future use; 0 length but set to 1 to skip the frame type byte
    , 1		// AMR Frame No Data
};


OmxAmrDecoder::OmxAmrDecoder()
{
    is_amrnb = 0;
	frame_num_in_toc = 0;
    toc = NULL;

	/* Initialize decoder homing flags */
    iDecHomingFlag = 0;
    iDecHomingFlagOld = 1;
	
	oscl_memset(&samrnbd, 0, sizeof(sAMR_NB_Dec_Struct));
	oscl_memset(&sAMRWBDec, 0, sizeof(sAMRWD_INFO) );
}

/* Decoder Initialization function */
OMX_BOOL OmxAmrDecoder::AmrDecInit(OMX_AUDIO_AMRFRAMEFORMATTYPE aInFormat, 
								OMX_AUDIO_AMRBANDMODETYPE aInMode)
{
    int ret_val = 0;

	LOGE_AMR_OMX(" AMRDecoder :: Info :: Entered AmrDecInit ");
	
	LOGE_AMR_OMX(" AmrDecInit :: Info :: aInFormat=%d, aInMode=%d ", aInFormat, aInMode);

	is_amrnb = 0;
	in_format = (int)aInFormat;
	frame_num_in_toc = 0;
	is_init = 0;

	if (aInMode >= OMX_AUDIO_AMRBandModeNB0 
		&& aInMode <= OMX_AUDIO_AMRBandModeNB7)
	{
		/* Create an instance of AMR-NB decoder */
		sAMR_NB_handler = sAMR_NBCreateDec(&ret_val, &samrnbd);
		if(ret_val != 0) 
		{
			LOGE_AMR_OMX(" AmrDecInit :: Error :: Hardware is not matched with this library!! ");

			return OMX_FALSE;
		}
		else 
		{
			LOGE_AMR_OMX(" AmrDecInit :: Info :: AMR-NB decoder created successfully  ");
		}

		is_amrnb = 1;
	} 

	else if(aInMode >= OMX_AUDIO_AMRBandModeWB0 
		    && aInMode <= OMX_AUDIO_AMRBandModeWB8)
	{
	    /* AMR-WB */
		is_amrnb = 0;
	}
	else
	{
		return OMX_FALSE;
	}

	/* Set bitstream format incase of AMR WB Decoder */
    if(1 != is_amrnb)
    {
    	if(OMX_AUDIO_AMRFrameFormatConformance == aInFormat)
		{
		    sAMRWBDec.bitstreamformat = 1;
		}
		else if(OMX_AUDIO_AMRFrameFormatIF1 == aInFormat)
		{
		    
		}
		else if(OMX_AUDIO_AMRFrameFormatIF2 == aInFormat)
		{
		    sAMRWBDec.bitstreamformat = 0;
		}
		else if(OMX_AUDIO_AMRFrameFormatFSF == aInFormat)
		{
            in_format = AMR_WB_FSF;
			sAMRWBDec.bitstreamformat = 2;
		}
		else if(OMX_AUDIO_AMRFrameFormatRTPPayload == aInFormat)
		{
            in_format = AMR_WB_RTP;
			sAMRWBDec.bitstreamformat = 2;
		}
		
        /* Create a WB-AMR decoder instance */
		sAMR_WB_handler = sAMRWBCreateDec(&sAMRWBDec);
		
		if(NULL == sAMR_WB_handler)
		{
			LOGE_AMR_OMX(" AmrDecInit :: Error :: AMR-WB Decoder Creation Failed ");
			
			return OMX_FALSE;
		} 
		else 
		{
			LOGE_AMR_OMX(" AmrDecInit :: Info :: AMR-WB Decoder Creation Successfull ");
		}

    }

    LOGE_AMR_OMX(" AMRDecoder :: Info :: Exiting AmrDecInit ");
	return OMX_TRUE;
}

/* Decoder De-Initialization function */
void OmxAmrDecoder::AmrDecDeinit()
{
    int ret_val = 0;
	
	LOGE_AMR_OMX(" AMRDecoder :: Info :: Entered AmrDecDeinit ");
	
	if (is_amrnb == 1)
	{
		sAMR_NBDeleteDec(sAMR_NB_handler, &samrnbd);
		
	}
	else 
	{
	    sAMRWBDeleteDec(sAMR_WB_handler, &sAMRWBDec);
	}

	LOGE_AMR_OMX(" AMRDecoder :: Info :: Exiting AmrDecDeinit ");
}

void OmxAmrDecoder::ResetDecoder()
{
    LOGE_AMR_OMX(" AMRDecoder :: Info :: Entered ResetDecoder ");

	int ret_val = 0;
	
	iBytesProcessed = 0;
	
	if (is_amrnb == 1)
	{
		sAMR_NBDeleteDec(sAMR_NB_handler, &samrnbd);
		sAMR_NB_handler = sAMR_NBCreateDec(&ret_val, &samrnbd);

		if(ret_val != 0)
		{
			LOGE_AMR_OMX(" ResetDecoder :: Error :: Hardware not matched for decoder library ");
		}
	}
	else 
	{
		sAMRWBDeleteDec(sAMR_WB_handler, &sAMRWBDec);
		
		sAMR_WB_handler = sAMRWBCreateDec(&sAMRWBDec);
		
		if(NULL == sAMR_WB_handler) 
		{
			LOGE_AMR_OMX(" ResetDecoder :: Error :: AMR-WB Decoder Creation Failed ");
		} 
		else 
		{
			LOGE_AMR_OMX(" ResetDecoder :: Info :: AMR-WB Decoder Creation Successfull ");
		}		
	}

	frame_num_in_toc = 0;
	
	LOGE_AMR_OMX(" AMRDecoder :: Info :: Exiting ResetDecoder ");
}

/* Find the start point & size of TOC table in case of IETF_Combined format */
void OmxAmrDecoder::GetStartPointsForIETFCombinedMode(OMX_U8* aPtrIn, 
													OMX_U32 aLength, 
													OMX_U8* &aTocPtr, 
													OMX_S32* aNumOfBytes)
{
	OMX_U8 Fbit 	 = 0x80;
	OMX_U32 FrameCnt = 0;

    //LOGE_AMR_OMX(" -- Info AMRDecoder :: Entered GetStartPointsForIETFCombinedMode \n");
	
	/* Count number of frames */
	aTocPtr = aPtrIn;
	while ((*(aTocPtr + FrameCnt) & Fbit) && (FrameCnt < aLength))
	{
		FrameCnt++;
	}

	aTocPtr = aPtrIn + FrameCnt;
	FrameCnt++;
	*aNumOfBytes = FrameCnt;
}


/* Decode function for all the input formats */
OMX_BOOL OmxAmrDecoder::AmrDecodeNarrowBandFrame(OMX_S16* aOutputBuffer,
                                       OMX_U32* aOutputLength, OMX_U8** aInBuffer,
                                       OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer,
                                       OMX_BOOL* aResizeFlag)
{
	OMX_U8 * toc_ptr;
	OMX_U32 offset = 0;
	OMX_U8 buf[AMR_MAX_SIZE];
	OMX_S32 frm_sz = 0;
	OMX_U32 frm_type;

	LOGE_AMR_OMX(" AMRDecoder :: Info :: Entering AmrDecodeNarrowBandFrame ");
	
	LOGE_AMR_OMX(" AmrDecodeNBFrame :: Info :: in_len=%d, *aIsFirstBuffer=%d ", *aInBufSize,
														*aIsFirstBuffer);

	*aResizeFlag = OMX_FALSE;
	
	(*aIsFirstBuffer)++;
	/* After decoding the first frame, modify all the input & output port settings */
	if (1 == *aIsFirstBuffer)
	{
		/* Set the Resize flag to send the port settings changed callback */
		*aResizeFlag = OMX_TRUE;
	}

	if (in_format == AMR_NB_RTP)
	{
		LOGE_AMR_OMX(" AmrDecodeNBFrame :: Info :: frame_num_in_toc=%d ", frame_num_in_toc);
		/* in case of rtp pay load format, 
		    all frames not contain TOC except first frame
		*/
		if (frame_num_in_toc == 0)
		{
			GetStartPointsForIETFCombinedMode(*aInBuffer, *aInBufSize,
											toc_ptr, (OMX_S32 *)&frame_num_in_toc);
			/* frame_num_in_toc is dummy header length + 1 */
			offset = frame_num_in_toc - 1;
			frame_num_in_toc--;
			toc= toc_ptr;
			/* get frame size */
			frm_sz = get_frm_sz(toc, &frm_type);
			LOGE_AMR_OMX(" AmrDecodeNBFrame :: Info :: frame_num_in_toc=%d, frm_sz=%d, *toc=%02x\n", 
				           frame_num_in_toc, frm_sz, *toc);
			oscl_memcpy(buf, *aInBuffer + offset, sizeof(OMX_U8)* frm_sz);

			*aInBuffer += (offset + frm_sz);
			*aInBufSize -=(offset + frm_sz); 
		}
		else 
		{
			frame_num_in_toc--;
			/* get frame size */
			/* frm_sz includes toc and frame data length */
			frm_sz = get_frm_sz(toc, &frm_type);

			/* copy toc into the first buffer */
			buf[0] = *toc;
			oscl_memcpy(&buf[1], *aInBuffer + offset, sizeof(OMX_U8) * (frm_sz - 1));
			
			*aInBuffer += (frm_sz - 1);
			*aInBufSize -=(frm_sz - 1); 
		}


		samrnbd.p_in = buf;
		samrnbd.p_out = aOutputBuffer;
	}
	else
	{
		/* local play */
		if (*aIsFirstBuffer == 0 && is_init == 0)
		{
			is_init = 1;
			/* check AMR magic number : '#!AMR\n' (0x2321414D520A) */
			if (*aInBuffer[0] == 0x23 && *aInBuffer[1] == 0x21 
				&& *aInBuffer[2] == 0x41 && *aInBuffer[3] == 0x4D && *aInBuffer[4] == 0x52)
				offset = 6;
		}
			
		samrnbd.p_in = *aInBuffer + offset;
		samrnbd.p_out = aOutputBuffer;
		frm_sz = get_frm_sz(samrnbd.p_in, &frm_type);
		
		*aInBuffer += (offset + frm_sz);
		*aInBufSize -=(offset + frm_sz); 
	}

	if (sAMR_NBDecFrame(sAMR_NB_handler, &samrnbd))
	{
		LOGE_AMR_OMX(" AmrDecodeNBFrame :: Error :: decoding a frame %d ", *aIsFirstBuffer);
		*aOutputLength = 0;
		return OMX_FALSE;
	}

	*aOutputLength = AMR_NB_OUT_SIZE;
	
	LOGE_AMR_OMX(" AMRDecoder :: Info :: Exiting AmrDecodeNarrowBandFrame ");

	return OMX_TRUE;
}

OMX_BOOL OmxAmrDecoder::AmrDecodeWideBandFrame(OMX_S16* aOutputBuffer,
                                       OMX_U32* aOutputLength, OMX_U8** aInBuffer,
                                       OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer,
                                       OMX_BOOL* aResizeFlag)
{

	OMX_U8 * toc_ptr = NULL;
	OMX_U32 offset = 0;
	OMX_U8 buf[AMR_WB_MAX_SIZE];
	OMX_S32 frm_sz = 0;
	OMX_U32 frm_type;
	
	OMX_S32 FrameBytesProcessed = 0, FrameLength;

	LOGE_AMR_OMX(" AMRDecoder :: Info :: Entering AmrDecodeWideBandFrame ");
		
	LOGE_AMR_OMX(" AmrDecodeWBFrame :: Info :: in_len=%d, *aIsFirstBuffer=%d\n", 
		                                 *aInBufSize, *aIsFirstBuffer);

	*aResizeFlag = OMX_FALSE;
	
	(*aIsFirstBuffer)++;
	/* After decoding the first frame, modify all the input & output port settings */
	if (1 == *aIsFirstBuffer)
	{
	    /* Set the Resize flag to send the port settings changed callback */
	    *aResizeFlag = OMX_TRUE;
	}

	if (in_format == AMR_WB_RTP)
	{
		LOGE_AMR_OMX(" AmrDecodeWBFrame :: Info :: frame_num_in_toc=%d ", frame_num_in_toc);
		/* in case of rtp pay load format, 
		   all frames not contain TOC except first frame
		*/
		if (frame_num_in_toc == 0)
		{
			GetStartPointsForIETFCombinedMode(*aInBuffer, *aInBufSize,
											toc_ptr, (OMX_S32 *)&frame_num_in_toc);
			/* frame_num_in_toc is dummy header length + 1 */
			offset = frame_num_in_toc - 1;
			
			frame_num_in_toc--;
			
			toc= toc_ptr;
			
			/* get frame size */
			frm_sz = get_frm_sz(toc, &frm_type);
			LOGE_AMR_OMX(" AmrDecodeWBFrame :: Info :: frame_num_in_toc=%d, frm_sz=%d, *toc=%02x \n", 
											frame_num_in_toc, frm_sz, *toc);
			oscl_memcpy(buf, *aInBuffer + offset, sizeof(OMX_U8)* frm_sz);

			*aInBuffer += (offset + frm_sz);
			*aInBufSize -=(offset + frm_sz); 
		}
		else 
		{
			frame_num_in_toc--;
			/* get frame size */
			/* frm_sz includes toc and frame data length */
			frm_sz = get_frm_sz(toc, &frm_type);

			/* copy toc into the first buffer */
			buf[0] = *toc;
			oscl_memcpy(&buf[1], *aInBuffer + offset, sizeof(OMX_U8) * (frm_sz - 1));
			
			*aInBuffer += (frm_sz - 1);
			*aInBufSize -=(frm_sz - 1); 
		}

		sAMRWBDec.p_in  = buf;
		sAMRWBDec.p_out = aOutputBuffer;

        /* decode a Frame  */
		sAMRWBDecFrame(sAMR_WB_handler, &sAMRWBDec);
		
		*aOutputLength = AMR_WB_OUT_SIZE;

	}
	else
	{
        /* local play */
		OMX_U8* pSpeechBits = *aInBuffer+iBytesProcessed ;
		
        if (*aIsFirstBuffer == 0 && is_init == 0)
        {
            if ('#' == pSpeechBits[0])
            {
                pSpeechBits += 6;
                FrameBytesProcessed = 6;
            }
            is_init = 1;
        }

		frm_sz = get_frm_sz(pSpeechBits, &frm_type);

        sAMRWBDec.p_in = (*aInBuffer+iBytesProcessed);
		sAMRWBDec.p_out = (short *)aOutputBuffer ;

        /* decode a Frame  */
		ret = sAMRWBDecFrame(sAMR_WB_handler, &sAMRWBDec);

		*aOutputLength = AMR_WB_OUT_SIZE;
		*aInBufSize -= ret;
		iBytesProcessed += ret;
		if (*aInBufSize == 0)
		{
			iBytesProcessed = 0 ;
		}
	}

	LOGE_AMR_OMX(" AMRDecoder :: Info :: Exiting AmrDecodeWideBandFrame ");
		
	return OMX_TRUE;	

}

/* get AMR frame size */
OMX_S32 OmxAmrDecoder::get_frm_sz(OMX_U8 * buf, OMX_U32 * frm_type)
{
	OMX_U32 type = (buf[0] >> 3) & 0x0F;

	*frm_type = type;
    if(1 == is_amrnb)
	{
	    return IETFFrameSize[type];
	}
	else 
	{
	    return WBIETFFrameSize[type];
	}
	
}


/* Decode function for all the input formats */
OMX_BOOL OmxAmrDecoder::AmrDecodeSilenceFrame(OMX_S16* aOutputBuffer,
											OMX_U32* aOutputLength)
{
	return OMX_TRUE;
}

