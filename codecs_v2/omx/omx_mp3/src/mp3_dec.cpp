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
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


//#define ENABLE_LOG_MP3_PV_OMX
#ifdef ENABLE_LOG_MP3_PV_OMX
#include <utils/Log.h>
#define LOGE_MP3_PV_OMX   LOGE
#else
#define LOGE_MP3_PV_OMX //LOGE
#endif

#include "mp3_dec.h"
#include "pvmp3decoder_api.h"

#include "pvmp3_tables.h"
#include "pv_mp3dec_fxd_op.h"

#define MP3DEC_MP3_FRAME_HEADER_SIZE 4

#define SEARCH_SYNC_WORD

//#define FILE_DUMP

Mp3Decoder::Mp3Decoder()
{
	LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3Decoder");
    iMP3DecExt = NULL;
    iAudioMp3Decoder = NULL;
    iInputUsedLength = 0;
    iInitFlag = 0;
    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3Decoder");
}

OMX_BOOL Mp3Decoder::Mp3DecInit(OMX_AUDIO_CONFIG_EQUALIZERTYPE* aEqualizerType)
{
	LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3DecInit");

    //Default equalization type
    e_equalization EqualizType = (e_equalization) aEqualizerType->sBandIndex.nValue;

    iAudioMp3Decoder = CPvMP3_Decoder::NewL();

    if (!iAudioMp3Decoder)
    {
	    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecInit Error1");
        return OMX_FALSE;
    }

    // create iMP3DecExt
    if (!iMP3DecExt)
    {
        iMP3DecExt = OSCL_NEW(tPVMP3DecoderExternal, ());
        if (!iMP3DecExt)
        {
	        LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecInit Error2");
            return OMX_FALSE;
        }
        iMP3DecExt->inputBufferCurrentLength = 0;
    }

    // Initialize the decoder
    // Input/output buffer allocation and CrcEnabler has been kept as false
    iAudioMp3Decoder->StartL(iMP3DecExt, false, false, false, EqualizType);
    iMP3DecExt->inputBufferMaxLength = 512;

    iInitFlag = 0;
    iInputUsedLength = 0;

    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecInit");

#ifdef FILE_DUMP
	fp_dump = fopen("/sdcard/mp3_dump_data_pv.raw","wb+");
	if(fp_dump == NULL)
	{
		LOGE_MP3_PV_OMX("In OmxAacDecoder::Cannot open file for dumping Error");
		return OMX_TRUE;
	}
#endif

    return OMX_TRUE;
}

void Mp3Decoder::ResetDecoder()
{
	LOGE_MP3_PV_OMX("In Mp3Decoder::ResetDecoder");

	iInputUsedLength = 0;

    if (iAudioMp3Decoder)
    {
        iAudioMp3Decoder->ResetDecoderL();
    }
    LOGE_MP3_PV_OMX("Out Mp3Decoder::ResetDecoder");
}

void Mp3Decoder::Mp3DecDeinit()
{
	LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3DecDeinit");

    if (iAudioMp3Decoder)
    {
        iAudioMp3Decoder->TerminateDecoderL();
        delete iAudioMp3Decoder;
        iAudioMp3Decoder = NULL;

        if (iMP3DecExt)
        {
            OSCL_DELETE(iMP3DecExt);
            iMP3DecExt = NULL;
        }
    }

#ifdef FILE_DUMP
    fclose(fp_dump);
    fp_dump = NULL;
#endif

	LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecDeinit");
}


Int Mp3Decoder::Mp3DecodeAudio(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_MP3TYPE* aAudioMp3Param,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag)
{

    int32 Status = MP3DEC_SUCCESS;
    *aResizeFlag = OMX_FALSE;

    LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3DecodeAudio");

#ifdef FILE_DUMP
	fwrite((*aInputBuf+iInputUsedLength), sizeof(char), *aInBufSize, fp_dump);
	*aInBufSize = 0;
    iInputUsedLength = 0;
    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio file dump");
	return MP3DEC_INVALID_FRAME;
#endif

    if (iInitFlag == 0)
    {
        //Initialization is required again when the client inbetween rewinds the input bitstream
        //Added to pass khronous conformance tests
        if (*aFrameCount != 0)
        {
            e_equalization EqualizType = iMP3DecExt->equalizerType;
            iMP3DecExt->inputBufferCurrentLength = 0;
            iInputUsedLength = 0;
            iAudioMp3Decoder->StartL(iMP3DecExt, false, false, false, EqualizType);
        }

        iInitFlag = 1;
    }

#ifdef SEARCH_SYNC_WORD
	OMX_U32 syncWordPosition = 0;

	Status = Mp3SearchSyncWord((*aInputBuf+iInputUsedLength),*aInBufSize,&syncWordPosition);
	//in case of Success(Sync word found and exist in next frame)
	//in case of Buffer incomplete.
	//in case of sync word not found.
    //LOGE("Mp3SearchSyncWord returns %d, %d, %d, %d", Status, syncWordPosition, *aInBufSize, iInputUsedLength);
	if(MP3DEC_SUCCESS==Status)
	{
	    iInputUsedLength += syncWordPosition;
		*aInputBuf       += iInputUsedLength;
		*aInBufSize -= syncWordPosition;
		iInputUsedLength = 0;
	}
	else if(MP3DEC_INCOMPLETE_FRAME==Status)
	{
		iInputUsedLength += syncWordPosition;
		*aInputBuf += iInputUsedLength;
		*aInBufSize -= syncWordPosition;
		iInputUsedLength = 0;	//Sync word found, but not enough data.
		return MP3DEC_INCOMPLETE_FRAME;
	}
	else if(MP3DEC_INVALID_FRAME==Status)
	{
		iInputUsedLength += syncWordPosition + 1;
		*aInputBuf += iInputUsedLength;
		*aInBufSize -= syncWordPosition + 1;
		iInputUsedLength = 0;	//Sync word found, but next sync word not exist.
		return MP3DEC_INCOMPLETE_FRAME;
	}
	else
	{
		LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio :: Invalid Frame ");
	    *aInBufSize = 0;
        iInputUsedLength = 0;
		return MP3DEC_INVALID_FRAME; //Sync word not exist
	}

#endif

    LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3DecodeAudio :: Input buff size (*aInBufSize) = %d, iInputUsedLength = 0x%x,InBuffContents = 0x%x,0x%x,0x%x,0x%x,0x%x ",(*aInBufSize), iInputUsedLength,*(*aInputBuf + iInputUsedLength),*(*aInputBuf + iInputUsedLength+1),*(*aInputBuf + iInputUsedLength+2),*(*aInputBuf + iInputUsedLength+3),*(*aInputBuf + iInputUsedLength+4));

    iMP3DecExt->pInputBuffer = *aInputBuf + iInputUsedLength;
    iMP3DecExt->pOutputBuffer = &aOutBuff[0];

    iMP3DecExt->inputBufferCurrentLength = *aInBufSize;
    iMP3DecExt->inputBufferUsedLength = 0;

    if (OMX_FALSE == aMarkerFlag)
    {
    	LOGE_MP3_PV_OMX("OMX_FALSE == aMarkerFlag");
        //If the input buffer has finished off, do not check the frame boundaries just return from here
        //This will detect the EOS for without marker test case.
        if (0 == iMP3DecExt->inputBufferCurrentLength)
        {
            iInputUsedLength = 0;
            LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Error1");
            return MP3DEC_INCOMPLETE_FRAME;
        }
        //If the marker flag is not set, find out the frame boundaries
        else
        {
            Status = iAudioMp3Decoder->SeekMp3Synchronization(iMP3DecExt);

            if (1 == Status)
            {
                if (0 == iMP3DecExt->inputBufferCurrentLength)
                {
                    //This indicates the case of corrupt frame, discard input bytes equal to inputBufferMaxLength
                    *aInBufSize -= iMP3DecExt->inputBufferMaxLength;
                    iInputUsedLength += iMP3DecExt->inputBufferMaxLength;
                    iMP3DecExt->inputBufferUsedLength += iMP3DecExt->inputBufferMaxLength;;

                    //return sucess so that we can continue decoding with rest of the buffer,
                    //after discarding the corrupted bit-streams
                    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Success1");
                    return MP3DEC_SUCCESS;
                }
                else
                {
                    *aInputBuf += iInputUsedLength;
                    iMP3DecExt->inputBufferUsedLength = 0;
                    iInputUsedLength = 0;
                    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Error2");
                    return MP3DEC_INCOMPLETE_FRAME;
                }
            }
        }
    }

    Status = iAudioMp3Decoder->ExecuteL(iMP3DecExt);

    if (MP3DEC_SUCCESS == Status)
    {
        *aInBufSize -= iMP3DecExt->inputBufferUsedLength;

        if (0 == *aInBufSize)
        {
            iInputUsedLength = 0;
        }
        else
        {
            iInputUsedLength += iMP3DecExt->inputBufferUsedLength;
        }

        *aOutputLength = iMP3DecExt->outputFrameSize * iMP3DecExt->num_channels;

        //After decoding the first frame, update all the input & output port settings
        if (0 == *aFrameCount)
        {

            //Output Port Parameters
            aAudioPcmParam->nSamplingRate = iMP3DecExt->samplingRate;
            aAudioPcmParam->nChannels = iMP3DecExt->num_channels;

            //Input Port Parameters
            aAudioMp3Param->nSampleRate = iMP3DecExt->samplingRate;

            //Set the Resize flag to send the port settings changed callback
            *aResizeFlag = OMX_TRUE;
        }

		LOGE_MP3_PV_OMX("Mp3Decoder::Mp3DecodeAudio :: aOutputLength = %d, aInBufSize = %d", *aOutputLength, *aInBufSize);
        (*aFrameCount)++;

        LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Success2");

        return Status;

    }
    else if (Status == MP3DEC_INVALID_FRAME)
    {
	    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Error MP3DEC_INVALID_FRAME %d %d", iInputUsedLength, *aInBufSize);
#ifdef SEARCH_SYNC_WORD

		ResetDecoder();

		Status = MP3DEC_INCOMPLETE_FRAME;
		iInputUsedLength += 1;
		*aInputBuf += iInputUsedLength;
		iMP3DecExt->inputBufferUsedLength = 0;
		iInputUsedLength = 0;
#else
        *aInBufSize = 0;
        iInputUsedLength = 0;
#endif
    }
    else if (Status == MP3DEC_INCOMPLETE_FRAME)
    {
		LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Error MP3DEC_INCOMPLETE_FRAME %d", *aInBufSize);
        *aInputBuf += iInputUsedLength;
        iMP3DecExt->inputBufferUsedLength = 0;
        iInputUsedLength = 0;
    }
    else
    {
		ResetDecoder();
		LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Error Else part error");
        *aInputBuf += iInputUsedLength;
        iInputUsedLength = 0;
    }

    LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3DecodeAudio Error3");

    return Status;

}


OMX_U32 Mp3Decoder::Mp3SearchSyncWord(OMX_U8* aInBuffer, OMX_U32 aInputBuffLength, OMX_U32* aHeaderPosition)
{
	LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3SearchSyncWord ");

#ifdef SEARCH_SYNC_WORD

	OMX_U32 count = 0;
	OMX_U32 mp3_frame_header;
	OMX_BOOL header_found = OMX_FALSE;
	OMX_U32 inputLength = aInputBuffLength;
	OMX_U8* 	inBuf = aInBuffer;
	OMX_U32 headerPos = 0;

	LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3SearchSyncWord :: aInputBuffLength = %d",aInputBuffLength);


	*aHeaderPosition = 0;
	while(inputLength > 0)
	{
		//Input Check!!
		//1) Assume aInBuffer is Valid. - pass
		//2) aInputBuffLength should check.
		if(inputLength < MP3DEC_MP3_FRAME_HEADER_SIZE)
		{
			LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3SearchSyncWord :: return position AAA ");

			*aHeaderPosition = headerPos;
			//incomplete header
			return MP3DEC_INCOMPLETE_FRAME; //Need more data
		}

		//seek sync word
		mp3_frame_header = (inBuf[0]<<16) + (inBuf[1]<<8) + (inBuf[2]);
		for(count = 0; count < inputLength-3; count++)
		{
			mp3_frame_header = (mp3_frame_header<<8)|inBuf[count+3];

			//LOGE_MP3_PV_OMX("In Mp3Decoder::Mp3SearchSyncWord SyncWord = %x",SyncWord);

			if ((mp3_frame_header >> 21) == 0x7FF)	//check sync word
			{
				header_found = OMX_TRUE;
				break;
			}
		}

		if(header_found==OMX_FALSE)
		{
			if(((mp3_frame_header<<8)>>21) == 0x7FF)	//check sync word with last two bytes
			{
				header_found = OMX_TRUE;
				count += 1;
			}
			else if(((mp3_frame_header<<16)>>21) == 0x7FF)	//check sync word with last two bytes
			{
				header_found = OMX_TRUE;
				count += 2;
			}
			else
			{
				LOGE_MP3_PV_OMX(" Out Mp3Decoder::Mp3SearchSyncWord :: return position BBB ");

				return MP3DEC_LOST_FRAME_SYNC;	//sync word doesn't exist
			}
		}

		if(header_found==OMX_TRUE)
			headerPos += count;

		if((inputLength-count)<MP3DEC_MP3_FRAME_HEADER_SIZE)
		{
			LOGE_MP3_PV_OMX(" Out Mp3Decoder::Mp3SearchSyncWord :: return position CCC ");
			*aHeaderPosition = headerPos;

			return MP3DEC_INCOMPLETE_FRAME; //incomplete header Need more data
		}

		//after hear, header has found!
		//and check validation of sync word
		//get version
		OMX_S32 version = (OMX_S32)((mp3_frame_header >> 19) & 3);
		//version calculate
		switch(version)
		{
			case 0:
				version = MPEG_2_5; //MPEG version 2.5
				break;
			case 2:
				version = MPEG_2; //MPEG version 2
				break;
			case 3:
				version = MPEG_1; //MPEG version 1
				break;
			default:
				version = INVALID_VERSION; //reserved
				break;
		}

		OMX_U32 bitrate_idx = ((mp3_frame_header >> 12) & 0xF);
		OMX_U32 s_rate_idx = ((mp3_frame_header >> 10) & 3);
		OMX_U32 val = 0;
	//	LOGE("ver = %d, %d, %d, %d", version, bitrate_idx,s_rate_idx, *aHeaderPosition);

		if (version != INVALID_VERSION && (s_rate_idx != 3) && (bitrate_idx!=0 && bitrate_idx!=0xF))
		{

			OMX_U32 frame_size = fxp_mul32_Q28(mp3_bitrate[version][bitrate_idx] << 20,
	        	                                   inv_sfreq[s_rate_idx]);

			frame_size >>= (20 - version);
			if(version != MPEG_1)
			{
				frame_size >>= 1;
			}
			if((mp3_frame_header >> 9) & 1)
			{
				frame_size++;
			}
	//		LOGE("%d", frame_size);

			if((inputLength-count) > (frame_size + 2))
			{
				uint8    *pElem  = (inBuf+count+frame_size);

				uint16 tmp1 = *(pElem++);
	            uint16 tmp2 = *(pElem);
				// LOGI("%x %x %x %x %x %x %x %x", *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--));
			    // LOGI("%x %x %x %x %x %x %x %x", *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--), *(pElem--));

				val = (tmp1 << 3);
	            val |= (tmp2 >> 5);
			}
			else
			{
				*aHeaderPosition = headerPos;
				return MP3DEC_INCOMPLETE_FRAME; //Need more data
			}
		}
		else
		{
			headerPos+=1;
			inputLength = aInputBuffLength - headerPos;
			inBuf = aInBuffer + headerPos;
			LOGE_MP3_PV_OMX("THIS ALSO HAPPEN?????%d %x", s_rate_idx, bitrate_idx);
			continue;
//			return MP3DEC_INVALID_FRAME;	//sync word doesn't exist
		}

		if(val == 0x7FF)
		{
			*aHeaderPosition = headerPos;

			LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3SearchSyncWord :: Success ");

			return MP3DEC_SUCCESS;
		}
		else
		{
			headerPos+=1;
			inputLength = aInputBuffLength - headerPos;
			inBuf = aInBuffer + headerPos;
			LOGE_MP3_PV_OMX("I SAID IT is POSSIBLE %x", val);
			continue;
		}
	}

#endif

	LOGE_MP3_PV_OMX("Out Mp3Decoder::Mp3SearchSyncWord2 Error");

	return MP3DEC_INVALID_FRAME;
}


