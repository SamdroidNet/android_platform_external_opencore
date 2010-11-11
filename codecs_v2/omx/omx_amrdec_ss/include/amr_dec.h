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
#ifndef AMR_DEC_H_INCLUDED
#define AMR_DEC_H_INCLUDED

#include "oscl_mem.h"
#include "OMX_Component.h"
#include "sAMRWB_Dec.h"
#include "sAMR_NB.h"

class OmxAmrDecoder
{
public:
	OmxAmrDecoder();

	OMX_BOOL AmrDecInit(OMX_AUDIO_AMRFRAMEFORMATTYPE aInFormat, 
						OMX_AUDIO_AMRBANDMODETYPE aInMode);

	void AmrDecDeinit();

	OMX_BOOL AmrDecodeNarrowBandFrame(OMX_S16* aOutputBuffer,
										   OMX_U32* aOutputLength, OMX_U8** aInBuffer,
										   OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer,
										   OMX_BOOL* aResizeFlag);

    OMX_BOOL AmrDecodeWideBandFrame(OMX_S16* aOutputBuffer,
                                       OMX_U32* aOutputLength, OMX_U8** aInBuffer,
                                       OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer,
                                       OMX_BOOL* aResizeFlag);
	
	OMX_BOOL AmrDecodeSilenceFrame(OMX_S16* aOutputBuffer,
								OMX_U32* aOutputLength);

	void ResetDecoder(); // for repositioning

	OMX_S32 iAmrInitFlag;

private:
	void GetStartPointsForIETFCombinedMode(OMX_U8* aPtrIn, OMX_U32 aLength,
									OMX_U8* &aTocPtr, OMX_S32* aNumOfBytes);
	OMX_S32 get_frm_sz(OMX_U8 * buf, OMX_U32 * frm_type);

	OMX_S32 iBytesProcessed;

	// AMR_NB Decoder --> LSI Codec
	sAMR_NB_Dec_Struct samrnbd;
	void *sAMR_NB_handler;
	int is_init;
	int is_amrnb;
	int in_format;
	int frame_num_in_toc;
	OMX_U8 * toc;

	// AMR_WB Decoder --> LSI Codec
	sAMRWD_INFO sAMRWBDec;
	void *sAMR_WB_handler;
    OMX_S16 iDecHomingFlag;
    OMX_S16 iDecHomingFlagOld;
	short ret;
	
};



#endif	//#ifndef AMR_DEC_H_INCLUDED

