/* ------------------------------------------------------------------
 * Copyright (C) 2008 PacketVideo
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
#include "mp3_dec.h"
//#include "pvmp3decoder_api.h"

//#define PRINT_LOGS

#ifdef PRINT_LOGS
#include <utils/Log.h>
#endif


#define USE_MEMCPY //Without memcpy the output is corrupted with lot of noise.

extern unsigned short FrameSizeID0[45],FrameSizeID1[45],FrameSizeIDex0[45];		// 20081218 - JM

Mp3Decoder::Mp3Decoder()
{
    iInputUsedLength = 0;
    iInitFlag = 0;
    fp = NULL;    
}

OMX_BOOL Mp3Decoder::Mp3DecInit(OMX_AUDIO_CONFIG_EQUALIZERTYPE* aEqualizerType)
{
    //Default equalization type
    //e_equalization EqualizType = (e_equalization) aEqualizerType->sBandIndex.nValue;
    
    // Initialize the decoder
#ifdef PRINT_LOGS
	LOGE("##### void Mp3Decoder::Mp3DecInit() #### \n");    
#endif

    //Input/output buffer allocation and CrcEnabler has been kept as false
    BSBuf = (unsigned char*)oscl_malloc(BUFF_SIZE*sizeof(unsigned char));
    if(BSBuf == NULL)
    {
	    return OMX_FALSE;
    }
    PCMBuf = (short*)oscl_malloc(1152*2*sizeof(short));
    if(PCMBuf == NULL)
    {
	    return OMX_FALSE;
    }
    
    memset(&dec_struct,0,sizeof(mp3_dec_struct));
    
    dec_struct.p_in = BSBuf;
	dec_struct.p_out = PCMBuf;
	dec_struct.read_bytes = 0;
	
    sMP3CreateDec(&dec_struct);

    iInitFlag = 0;
    iInputUsedLength = 0;

    return OMX_TRUE;
}

void Mp3Decoder::ResetDecoder()
{    
#ifdef PRINT_LOGS	
	LOGE("##### void Mp3Decoder::ResetDecoder() #### \n");
#endif		
	sMP3ResetDec(&dec_struct);	
}

void Mp3Decoder::Mp3DecDeinit()
{
#ifdef PRINT_LOGS	
	LOGE("##### void Mp3Decoder::Mp3DecDeinit() #### \n");
#endif	
	sMP3DeleteDec(&dec_struct);
	oscl_free(BSBuf);
	BSBuf = NULL;
	oscl_free(PCMBuf);
	PCMBuf = NULL;
	iInitFlag = 0;
}


int Mp3Decoder::Mp3DecodeAudio(OMX_S16* aOutBuff,
                               OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                               OMX_U32* aInBufSize, OMX_S32* aFrameCount,
                               OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                               OMX_AUDIO_PARAM_MP3TYPE* aAudioMp3Param,
                               OMX_BOOL aMarkerFlag,
                               OMX_BOOL* aResizeFlag)
{

    int32 Status = MP3DEC_SUCCESS;
    int32 framesize = 0;
    int32 i = 0;
    
    unsigned long slot;
    long SyncWord,	paddingbit;
	unsigned short stmp;
	unsigned short Id, Idex;
	unsigned short BitrateIndex, SamplingRate;
	OMX_U8 *aTempBuff;
		
    *aResizeFlag = OMX_FALSE;
    
#ifdef PRINT_LOGS
	LOGE("##### void Mp3Decoder::Mp3DecodeAudio() #### \n");
#endif	

    if (iInitFlag == 0)
    {
        //Initialization is required again when the client inbetween rewinds the input bitstream
        //Added to pass khronous conformance tests
        if (*aFrameCount != 0)
        {
	        memset(&dec_struct,0,sizeof(mp3_dec_struct));
	        dec_struct.p_in = BSBuf;			
			dec_struct.p_out = aOutBuff;
			dec_struct.read_bytes = 0;
			    
	        sMP3ResetDec(&dec_struct);	        
            iInputUsedLength = 0;            
        }

        iInitFlag = 1;
    }
    
    aTempBuff = (*aInputBuf+iInputUsedLength);
    SyncWord = (aTempBuff[0]<<4) + (aTempBuff[1]>>4);
    if (SyncWord != 0xfff && SyncWord != 0xffe)
    {
	    *aInBufSize = 0;
        iInputUsedLength = 0;

#ifdef PRINT_LOGS
        LOGE("##### SyncWork MP3DEC_INVALID_FRAME @@@@#### \n");
#endif
	    return MP3DEC_INVALID_FRAME;
    }			
	Id = (aTempBuff[1]>>3) & 0x1;
	Idex = (aTempBuff[1]>>4) & 0x1;
	BitrateIndex = (aTempBuff[2] >> 4) & 0xf;
	SamplingRate = (aTempBuff[2] >> 2) & 0x3;
	stmp = BitrateIndex*3 + SamplingRate; 
	
	paddingbit = (aTempBuff[2] >> 1) & 0x1; // yesjoon 081224 for 44100,22050,11025 Hz

	
	if( Id )	
	{
		slot = FrameSizeID1[stmp];
	}
	else
	{
		if( Idex == 0 )
		{
			slot = FrameSizeIDex0[stmp];
		}
		else
		{
			slot = FrameSizeID0[stmp];
		}
	}
	slot += paddingbit; 				// yesjoon 081224 for 44100,22050,11025 Hz    
	
	if(slot > *aInBufSize)//we dont have full frame to preocess
	{
		*aInputBuf += iInputUsedLength;        
        iInputUsedLength = 0;
        return MP3DEC_INCOMPLETE_FRAME;
	}

#ifdef USE_MEMCPY	
    oscl_memcpy((void *)BSBuf,(void *)(*aInputBuf+iInputUsedLength),slot);
	dec_struct.p_in = BSBuf;
#else
	dec_struct.p_in = (*aInputBuf+iInputUsedLength);	
#endif    	
	
	dec_struct.p_out = aOutBuff;
	dec_struct.read_bytes = 0;
    
    framesize = sMP3DecFrame(&dec_struct);
    
#ifdef PRINT_LOGS
    LOGE("dec_struct.read_bytes(*aInBufSize) after decoding : %d \n",dec_struct.read_bytes);
#endif

    if(framesize == 0)//error from decoder
    {
	    *aInBufSize = 0;
        iInputUsedLength = 0;
        
#ifdef PRINT_LOGS
        LOGE("##### framesize == 0 MP3DEC_INVALID_FRAME @@@@#### \n");
#endif
	    return MP3DEC_INVALID_FRAME;
    }
    
	*aInBufSize -= dec_struct.read_bytes;
    
	if(*aInBufSize == 0)//decoder expects only one frame data. Not more than that.
    {	    
	    iInputUsedLength = 0;	    
    }
    else
    {	    
	    iInputUsedLength += dec_struct.read_bytes;	    
    }
    
#ifdef PRINT_LOGS
    LOGE("output buff size after decoding : %d \n",framesize);
#endif
    
    *aOutputLength = framesize;
    
    //After decoding the first frame, update all the input & output port settings
    if (0 == *aFrameCount)
    {
        (*aFrameCount)++;

        //Output Port Parameters
        aAudioPcmParam->nSamplingRate = dec_struct.samplerate;
        aAudioPcmParam->nChannels = dec_struct.n_ch;

        //Input Port Parameters
        aAudioMp3Param->nSampleRate = dec_struct.samplerate;

        //Set the Resize flag to send the port settings changed callback
        *aResizeFlag = OMX_TRUE;
    }
    
    return MP3DEC_SUCCESS;
}
