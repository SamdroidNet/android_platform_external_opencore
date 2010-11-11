/* -------------------------------------------------------------------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name    : MP3 Encoder.
 * @File Name              : mp3_enc.cpp
 * @File Description       : Mp3 Encoder source file.
 * @Author                 : Sudhir Vyas, Renuka V
 * @Created Date           : 05-06-2009
 * @Modification History
 * Version:            Date:             By:                Change:
 * -------------------------------------------------------------------------------------------------------------------------------
 */
#include "mp3_enc.h"

OmxMp3Encoder::OmxMp3Encoder()
{
    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Constructor In\n");
    // Intializing all variables and structures related to Encoder library

    iStartTime                      = 0;
    iStopTime                       = 0;
    iNextStartTime                  = 0;

    iMaxNumOutputFramesPerBuffer    = MAX_NUM_OUTPUT_FRAMES_PER_BUFFER;
    iOneInputFrameLength            = MP3_ONE_INPUTFRAME_DEFAULT_LENGTH;

    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Constructor Out\n");
}

OMX_BOOL OmxMp3Encoder::Mp3EncInit(OMX_AUDIO_PARAM_PCMMODETYPE aPcmMode,
                                   OMX_AUDIO_PARAM_MP3TYPE aMP3Param,
                                   OMX_U32* aInputFrameLength,
                                   OMX_U32* aMaxNumberOutputFrames)
{
    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Mp3EncInit In \n");

    int iReturnVal = 0;
    int InBuffsize = 0;
    char version[30];

    sMP3E_Ver(version);

    /*****************************************************\
        Encoder Configuration
    \*****************************************************/
    enc_struct.bitrate        = BITS_PER_SAMPLE_16;
    enc_struct.n_ch           = CHANNELS_MONO;
    enc_struct.downmix        = 0;
    enc_struct.samplerate     = SAMPLING_RATE_8K;
    enc_struct.mpeg_ver       = 1;

    if( enc_struct.samplerate < 32000 )
        InBuffsize = 576 * enc_struct.n_ch; //576 : as one MP3 sample is of length 576
    else
        InBuffsize = 576 * enc_struct.n_ch * 2;

    MP3InputBuffer  = (unsigned char *)calloc(InBuffsize,sizeof(short));
    MP3OutputBuffer = (unsigned char *)calloc(MAXMP3BUFFER,sizeof(unsigned char));

    enc_struct.p_in  = MP3InputBuffer ;
    enc_struct.p_out = MP3OutputBuffer;

    /*****************************************************\
        Initialize Encoder
    \*****************************************************/
    #ifdef __MI_USE__ // Used for multiple instance
        sMP3E_handler = sMP3CreateEnc(&enc_struct);
    #else
        iReturnVal    = sMP3CreateEnc(&enc_struct);
        if(iReturnVal == SMP3E_ERR)
            LOGE_MP3_ENC_OMX("EncoderInit Error\n");
        else if(iReturnVal == SMP3E_ERR_MEMALLOC)
            LOGE_MP3_ENC_OMX("malloc error\n");
        else if(iReturnVal == SMP3E_ERR_MEM0ALLOC)
            LOGE_MP3_ENC_OMX("0 malloc Error\n");
        if(iReturnVal)
            exit(1);
    #endif

    *aInputFrameLength      = InBuffsize * 2; //As the input frame lenght is InBuffsize read in short
    *aMaxNumberOutputFrames = iMaxNumOutputFramesPerBuffer;

    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Mp3EncInit Out \n");
    return OMX_TRUE;
}


void OmxMp3Encoder::Mp3EncDeinit()
{
    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Mp3EncDeinit In \n");

    if ( MP3InputBuffer != NULL )
    {
        free(MP3InputBuffer);
    }

    if ( MP3OutputBuffer != NULL )
    {
        free(MP3OutputBuffer);
    }

#ifdef __MI_USE__
    sMP3DeleteEnc(sMP3E_handler, &enc_struct);
#else
    sMP3DeleteEnc(&enc_struct);
#endif

    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Mp3EncDeinit Out \n");
}


OMX_BOOL OmxMp3Encoder::Mp3EncodeAudio(OMX_U8*    aOutputBuffer,
                                       OMX_U32*   aOutputLength,
                                       OMX_U8*    aInBuffer,
                                       OMX_U32    aInBufSize,
                                       OMX_TICKS  aInTimeStamp,
                                       OMX_TICKS* aOutTimeStamp)
{

    LOGE_MP3_ENC_OMX( "OmxMp3Encoder Mp3EncodeAudio In \n");

    int32 InputFrameNum = 0; 
    int32 nEncodedBytes = 0;

    //Calculate the number of input frames to be encoded
    InputFrameNum = aInBufSize / iOneInputFrameLength;

    // required for next frame timestamp
    iStartTime           = (iNextStartTime >= aInTimeStamp  ? iNextStartTime : aInTimeStamp);
    iStopTime            = iStartTime + MP3_FRAME_LENGTH_IN_TIMESTAMP * InputFrameNum;
    iNextStartTime       = iStopTime; // for the next encoding

    //Encoding of Multiple frames at one time
    for(int i = 0; i < InputFrameNum; i++)
    {
        memcpy((void *)enc_struct.p_in,(void *)(aInBuffer+(i*iOneInputFrameLength)),iOneInputFrameLength);
        enc_struct.n_read    = (iOneInputFrameLength / 2);

        /*****************************************************\
                Encode Frame
        \*****************************************************/

    #ifdef __MI_USE__ // Used for multiple instance
        nEncodedBytes = sMP3EncFrame(sMP3E_handler, &enc_struct);
    #else
        nEncodedBytes = sMP3EncFrame(&enc_struct);
    #endif

        if( nEncodedBytes == SMP3E_ERR)
        {
            LOGE_MP3_ENC_OMX("Out OmxMp3Encoder::Mp3EncodeAudio Error");
            return OMX_FALSE;
        }

        memcpy((void *)aOutputBuffer,(void *)enc_struct.p_out,nEncodedBytes);
        // Then incrementing aOutputBuffer for next output frame
        aOutputBuffer    = aOutputBuffer + nEncodedBytes;
        // Total output buffer length in each call to loop
        *aOutputLength    += nEncodedBytes;
    }

    //Set the output buffer timestamp equal to the input buffer start time.
    *aOutTimeStamp        = iStartTime;
    LOGE_MP3_ENC_OMX("OmxMp3Encoder Mp3EncodeAudio Out \n");
    return OMX_TRUE;
}
