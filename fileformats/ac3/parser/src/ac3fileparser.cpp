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
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                 A C 3    F I L E    P A R S E R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


/**
 *  @file ac3fileparser.cpp
 *  @brief This file contains the implementation of the raw AC3 file parser.
 */

//----------------------------------------------------------------------------
// INCLUDES
//----------------------------------------------------------------------------
#include "ac3fileparser.h"

#include "oscl_dll.h"
#include <utils/Log.h>
OSCL_DLL_ENTRY_POINT_DEFAULT()

#define AC3_DECODE_FSCOD(_byte) (((_byte) & 0xC0) >> 6)
#define AC3_DECODE_FRAMESIZECOD(_byte) ((_byte) & 0x3F)

#define AC3_TBL_SAMPLING_RATE(_fscod) (AC3SamplingRateTbl[(_fscod)])
#define AC3_TBL_BIT_RATE(_framesizecod) (AC3BitRateTbl[(_framesizecod)])
#define AC3_TBL_FRAME_SIZE(_framesizecod,_fs) (AC3FrameSizeTbl[(_framesizecod)][(_fs)] * 2)
//#define __FILE_SIZE_SCAN_ONE_FRAME__
//#define __PRING_LOG__


/* Table containing possible sampling rates
 * in AC3 file. Sampling rates in Hz */
static const uint32 AC3SamplingRateTbl[4] = {
                            48000,        /* for fscod = 00b */
                            44100,        /* for fscod = 01b */
                            32000,        /* for fscod = 11b */
                            0xFFFFFFFF    /* invalid */
                        };

/* Bit rates in kbps */
static const uint32 AC3BitRateTbl[38] = {
                                             32,
                                             32,
                                             40,
                                             40,
                                             48,
                                             48,
                                             56,
                                             56,
                                             64,
                                             64,
                                             80,
                                             80,
                                             96,
                                             96,
                                            112,
                                            112,
                                            128,
                                            128,
                                            160,
                                            160,
                                            192,
                                            192,
                                            224,
                                            224,
                                            256,
                                            256,
                                            320,
                                            320,
                                            384,
                                            384,
                                            448,
                                            448,
                                            512,
                                            512,
                                            576,
                                            576,
                                            640,
                                            640
                        };

/* Frame sizes in words; 1 word = 2 bytes */
static const uint32 AC3FrameSizeTbl[38][3] = {
                             {    96,         69,         64 },
                             {    96,         70,         64 },
                             {   120,         87,         80 },
                             {   120,         88,         80 },
                             {   144,        104,         96 },
                             {   144,        105,         96 },
                             {   168,        121,        112 },
                             {   168,        122,        112 },
                             {   192,        139,        128 },
                             {   192,        140,        128 },
                             {   240,        174,        160 },
                             {   240,        175,        160 },
                             {   288,        208,        192 },
                             {   288,        209,        192 },
                             {   336,        243,        224 },
                             {   336,        244,        224 },
                             {   384,        278,        256 },
                             {   384,        279,        256 },
                             {   480,        348,        320 },
                             {   480,        349,        320 },
                             {   576,        417,        384 },
                             {   576,        418,        384 },
                             {   672,        487,        448 },
                             {   672,        488,        448 },
                             {   768,        557,        512 },
                             {   768,        558,        512 },
                             {   960,        696,        640 },
                             {   960,        697,        640 },
                             {  1152,        835,        768 },
                             {  1152,        836,        768 },
                             {  1344,        975,        896 },
                             {  1344,        976,        896 },
                             {  1536,       1114,       1024 },
                             {  1536,       1115,       1024 },
                             {  1728,       1253,       1152 },
                             {  1728,       1254,       1152 },
                             {  1920,       1393,       1280 },
                             {  1920,       1394,       1280 }
                        };


//! specially used in ResetPlayback(), re-position the file pointer
int32 ac3bitstreamObject::reset(int32 filePos)
{
    iFileBytesRead  = filePos; // set the initial value
    iFileBytesProcessed = iFileBytesRead;
    if (ipAC3File)
    {
        ipAC3File->Seek(filePos, Oscl_File::SEEKSET);
    }
    iBuffPos = 0;
    iBuffValidDataSize = 0;
    return refill();
}

//! read data from bitstream, this is the only function to read data from file
int32 ac3bitstreamObject::refill()
{
    uint8 *pWriteLocation = NULL;

    int32 retValue = 0;

    if (iFileBytesRead > 0 && iFileSize > 0 && iFileBytesRead >= iFileSize)
    {
        //At this point we're at least within 1 frame from the end of data.
        //Quit reading data but don't return EOF until all data is processed.
        if (iFileBytesProcessed < iFileBytesRead)
        {
            return ac3bitstreamObject::EVERYTHING_OK;
        }
        else
        {
            return ac3bitstreamObject::END_OF_FILE;
        }
    }

    if (!ipAC3File)
    {
        return ac3bitstreamObject::MISC_ERROR;
    }

    // Get file size at the very first time
    if (iFileSize == 0)
    {
        if (ipAC3File->Seek(0, Oscl_File::SEEKEND))
        {
            return ac3bitstreamObject::MISC_ERROR;
        }

        iFileSize = ipAC3File->Tell();

        if (iFileSize <= 0)
        {
            return ac3bitstreamObject::MISC_ERROR;
        }

        if (ipAC3File->Seek(0, Oscl_File::SEEKSET))
        {
            return ac3bitstreamObject::MISC_ERROR;
        }

        // first-time read, set the initial value of iBuffPos
        iBuffPos = 0;
        iBuffValidDataSize = 0;
        iFileBytesProcessed = 0;
        pWriteLocation = &iBuffer[iBuffPos];
    }
    else if (iBuffPos >= iBuffValidDataSize)
    {
        iBuffPos = 0;
        iBuffValidDataSize = 0;
        pWriteLocation = &iBuffer[iBuffPos];
    }

    else if (iBuffPos < iBuffValidDataSize)
    {
        // move the remaining stuff to the beginning of iBuffer
        if ((iBuffValidDataSize - iBuffPos) > iBuffPos)
        {
            // memory content will be overlapped
#ifdef __PRING_LOG__
            LOGE("AC3FILEPARSER refill: iBuffPos = %d, iBuffValidDatasize = %d, overlap condition", iBuffPos,iBuffValidDataSize);
#endif
            return ac3bitstreamObject::MISC_ERROR;
        }
        int32 len = iBuffValidDataSize - iBuffPos;
        oscl_memcpy(&iBuffer[0], &iBuffer[iBuffPos], len);
        pWriteLocation = &iBuffer[len];
        iBuffPos = 0;
        iBuffValidDataSize = len;
    }

    // read data
#ifdef __PRING_LOG__
    LOGE ("AC3FILEPARSER Size to read: %d", (iBuffAllocSize - iBuffValidDataSize));
#endif
    retValue = ipAC3File->Read (pWriteLocation, 1, (iBuffAllocSize - iBuffValidDataSize));
    if (retValue <= 0)
    {
        /* PVMF_AC3PARSER_LOGERROR ((0, "CAC3FileParser::refill- File read failed")); */
#ifdef __PRING_LOG__
        LOGE ("AC3FILEPARSER failed to read from file; Size to read: %d", (iBuffAllocSize - iBuffValidDataSize));
#endif
        return ac3bitstreamObject::READ_ERROR;
    }

    iFileBytesRead += retValue;
    iBuffValidDataSize += retValue;

    return ac3bitstreamObject::EVERYTHING_OK;
}

int32 ac3bitstreamObject::getFileSize()
{
	return iFileSize;
}
//! most important function to get one frame data plus frame type, used in getNextBundledAccessUnits()
int32 ac3bitstreamObject::getNextFrame(uint8* frameBuffer, int32& frame_len, int32& frame_duration_ms)
{
    int32 iCurrFrameSamplingRate = 0;
    int32 iCurrFrameBitRate = 0;
    int32 iCurrFrameSize = 0;
    bool bNeedToRefill = false;
    int32 ret_value = ac3bitstreamObject::EVERYTHING_OK;
    int32 frame_size = 0;
    uint8 *pBuffer = &iBuffer[iBuffPos];


    if (!frameBuffer)
    {
#ifdef __PRING_LOG__
        LOGE ("AC3FILEPARSER Error in getNextFrame : invalid argument frameBuffer is null");
#endif
        return ac3bitstreamObject::MISC_ERROR;
    }

    bNeedToRefill = false;
    if (iFileSize == 0 || iBuffPos >= iBuffValidDataSize)
    {
        bNeedToRefill = true;
    }
    else if ((iBuffValidDataSize - iBuffPos) <= 5) {
        bNeedToRefill = true;
    }
    else {

      /* at least 5 bytes are available to parse header */
        ret_value = parseAC3Header (pBuffer, iCurrFrameSamplingRate, iCurrFrameBitRate, iCurrFrameSize);
        if (ret_value == ac3bitstreamObject::EVERYTHING_OK) {
            if ((iBuffValidDataSize - iBuffPos) < iFrameSize) {
              /* there is not enough data for even one frame */
              bNeedToRefill = true;
            }
        }
        else {
            LOGE ("AC3FILEPARSER Error in getNextFrame : parseAC3Header failed");
            LOGE ("AC3FILEPARSER AC3Header = %#04x %#04x %#04x %#04x %#04x", pBuffer[0], pBuffer[1], pBuffer[2],
                                                              pBuffer[3], pBuffer[4]);
	     return ret_value;
       }
    }

    if (bNeedToRefill == true) {
        ret_value = refill();
        if (ret_value)
        {
#ifdef __PRING_LOG__
            LOGE ("AC3FILEPARSER Error in getNextFrame : refill failed %d", ret_value);
#endif
            return ret_value;
        }
    }
    ret_value = parseAC3Header (&iBuffer[iBuffPos], iCurrFrameSamplingRate, iCurrFrameBitRate, iCurrFrameSize);
    if (ret_value == ac3bitstreamObject::EVERYTHING_OK) {
        if ((iBuffValidDataSize - iBuffPos) < iFrameSize) {
            return ac3bitstreamObject::MISC_ERROR;
        }
    }
    else
    {
        LOGE ("AC3FILEPARSER Error in getNextFrame : parseAC3Header second call failed");
        LOGE ("AC3FILEPARSER AC3Header = %#04x %#04x %#04x %#04x %#04x", iBuffer[iBuffPos], iBuffer[iBuffPos+1], iBuffer[iBuffPos+2],
                                                          iBuffer[iBuffPos+3], iBuffer[iBuffPos+4]);
        return ac3bitstreamObject::MISC_ERROR;
    }


    if (iCurrFrameSamplingRate == 32000) {
        frame_duration_ms = 48;     /* 48 ms */
    }
    else if (iCurrFrameSamplingRate == 44100) {
        frame_duration_ms = 34830/1000;     /* 34.83 ms */
    }
    else if (iCurrFrameSamplingRate == 48000) {
        frame_duration_ms = 32;     /* 32 ms */
    }
    else {
        LOGE ("AC3FILEPARSER Error in getNextFrame : invalid sampling rate");
        frame_duration_ms = 0;
    }

    frame_len = iCurrFrameSize;

    oscl_memcpy(frameBuffer, &iBuffer[iBuffPos], iCurrFrameSize);
#ifdef __PRING_LOG__
    LOGE("AC3FILEPARSER getNextFrame : iBuffPos = %d, data ptr = %#010x", iBuffPos, &iBuffer[iBuffPos]);
    LOGE("AC3FILEPARSER getNextFrame : frame-size = %d, data = %#04x %#04x %#04x %#04x %#04x", iCurrFrameSize, iBuffer[iBuffPos],
                                                                                  iBuffer[iBuffPos+1],
                                                                                  iBuffer[iBuffPos+2],
                                                                                  iBuffer[iBuffPos+3],
                                                                                  iBuffer[iBuffPos+4]);
#endif
    iBuffPos += iCurrFrameSize;
    iFileBytesProcessed += iCurrFrameSize;


    return ret_value;
}

//! parse the AC3 bitstream header:
int32 ac3bitstreamObject::initAC3StreamParams()
{
    int32 returnValue = reset();
    if (returnValue == ac3bitstreamObject::EVERYTHING_OK)
    {

        // first time read, we don't use iSecond_buffer
        uint8 *pBuffer = &iBuffer[iBuffPos];
        returnValue = parseAC3Header (pBuffer, iSamplingRate, iBitRate, iFrameSize);
        if (returnValue == ac3bitstreamObject::EVERYTHING_OK)
        {
            /* PVMF_AC3PARSER_LOGDIAGNOSTICS((0, "CAC3FileParser::initAC3StreamParams SR=%d, BR=%d, FrS=%d",
                   iSamplingRate, iBitRate, iFrameSize)); */
        }
    }

    return returnValue;
}

//! get clip information: file size, sampling rate, bit rate and frame size
int32 ac3bitstreamObject::getFileInfo(int32& fileSize, int32& samplingrate, int32& bitrate, int32& framesize)
{
    fileSize = 0;
    int32 ret_value = ac3bitstreamObject::EVERYTHING_OK;
    if (iFileSize == 0)
    {
        ret_value = initAC3StreamParams();
        if (ret_value)
        {
            return ret_value;
        }
    }

    fileSize     = iFileSize;
    samplingrate = iSamplingRate;
    bitrate      = iBitRate;
    framesize    = iFrameSize;
    return ret_value;
}


//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::CAC3FileParser
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    None
//
//  Outputs:
//    None
//
//  Returns:
//    None
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  Constructor for CAC3FileParser class
//------------------------------------------------------------------------------
OSCL_EXPORT_REF CAC3FileParser::CAC3FileParser(void)
{
    iAC3FileSize        = 0;
    iAC3TrackDuration   = -1;
    iAC3SamplingRate    = 0;
    iAC3FrameSize       = 0;
    iAC3FrameDuration   = 0;
    iAC3BitRate         = 0;

    iTotalNumFramesRead = 0;
    iEndOfFileReached   = false;
    iRandomAccessTimeInterval = 0;
    iCountToClaculateRDATimeInterval = 0;
    iLogger = PVLogger::GetLoggerObject("pvac3_parser");
    iDiagnosticLogger = PVLogger::GetLoggerObject("playerdiagnostics.pvac3_parser");

    ipBSO = NULL;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::~CAC3FileParser
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    None
//
//  Outputs:
//    None
//
//  Returns:
//    None
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  Destructor for CAC3FileParser class
//------------------------------------------------------------------------------
OSCL_EXPORT_REF CAC3FileParser::~CAC3FileParser(void)
{
    iAC3File.Close();
    OSCL_DELETE(ipBSO);
    ipBSO = NULL;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::InitAC3File
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    iClip = pointer to the AC3 file name to be played of type TPtrC
//
//  Outputs:
//    None
//
//  Returns:
//    returnValue = true if the init succeeds, else false.
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  This function opens the AC3 file, checks for
//  a. sampling frequency
//  b. frame size
//  c. bitrate
//  d. frame interval (time duration)
//  e. track duration
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF bool CAC3FileParser::InitAC3File(OSCL_wString& aClip, bool aInitParsingEnable, Oscl_FileServer* aFileSession, PVMFCPMPluginAccessInterfaceFactory*aCPM, OsclFileHandle*aHandle, uint32 countToClaculateRDATimeInterval)
{
    iAC3File.SetCPM(aCPM);
    iAC3File.SetFileHandle(aHandle);

    // Open the file (aClip)
    if (iAC3File.Open(aClip.get_cstr(), (Oscl_File::MODE_READ | Oscl_File::MODE_BINARY), *aFileSession) != 0)
    {
        PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::InitAC3File- File Open failed"));
        return false;
    }

    // create ipBSO
    ipBSO = OSCL_NEW(ac3bitstreamObject, (iLogger, &iAC3File));
    if (!ipBSO)
    {
        return false;
    }
    if (!ipBSO->get())
    {
        return false; // make sure the memory allocation is going well
    }

    // get file info
    if (ipBSO->getFileInfo(iAC3FileSize, iAC3SamplingRate, iAC3BitRate, iAC3FrameSize))
    {
        PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::InitAC3File- getFileInfo failed "));
        return false;
    }

    iAC3FrameDuration = AC3SamplingRateToFrameDuration (iAC3SamplingRate);

    // Determine file duration and set up random positioning table if needed
    CalculateDuration(aInitParsingEnable, countToClaculateRDATimeInterval);
    return true;
}

int32 CAC3FileParser::AC3SamplingRateToFrameDuration (int32 samplingRate)
{
    if (samplingRate == 32000) {
        return 48000;     /* 48 ms */
    }
    else if (samplingRate == 44100) {
        return 34830;     /* 34.83 ms */
    }
    else if (samplingRate == 48000) {
        return 32000;     /* 32 ms */
    }
    else {
    }
}

bool CAC3FileParser::CalculateDuration(bool aInitParsingEnable, uint32 countToClaculateRDATimeInterval)
{
    iCountToClaculateRDATimeInterval = countToClaculateRDATimeInterval;
    uint32 FrameCount = iCountToClaculateRDATimeInterval;
    iRandomAccessTimeInterval = countToClaculateRDATimeInterval * (iAC3FrameDuration / 1000);
    int32 frame_duration_ms = 0;
    int32 frame_length = 0;

    PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::CalculateDuration (In)"));
    if (aInitParsingEnable)
    {
        // Go through each frame to calculate AC3 file duration.
        int32 status = ac3bitstreamObject::EVERYTHING_OK;
        uint8 frame_type = 15;
        iAC3TrackDuration = 0;

        int32 error = 0;

        int32 filePos = 0;

        OSCL_TRY(error, iRPTable.push_back(filePos));
        OSCL_FIRST_CATCH_ANY(error, return false);
#ifdef __FILE_SIZE_SCAN_ONE_FRAME__
       status = ipBSO->getNextFrame(iAC3FrameBuffer, frame_length, frame_duration_ms);
       if (status == ac3bitstreamObject::EVERYTHING_OK)
	{
		 // calculate the number of frames // BX
		int iFileSize = 0 ;
       		iFileSize = ipBSO->getFileSize();

		FrameCount = (uint32)(iFileSize/frame_length);
		iAC3TrackDuration = frame_duration_ms*FrameCount;
	}
#else
        while (status == ac3bitstreamObject::EVERYTHING_OK)
        {
            // get the next frame

            status = ipBSO->getNextFrame(iAC3FrameBuffer, frame_length, frame_duration_ms);

            if (status == ac3bitstreamObject::EVERYTHING_OK)
            {
                // calculate the number of frames // BX
                iAC3TrackDuration += frame_duration_ms;
                filePos += frame_length;

                // set up the table for randow positioning
                error = 0;
                if (!FrameCount)
                {
                    OSCL_TRY(error, iRPTable.push_back(filePos));
                    OSCL_FIRST_CATCH_ANY(error, return false);
                    FrameCount = countToClaculateRDATimeInterval;
                }
            }

            else if (status == ac3bitstreamObject::END_OF_FILE)
            {
                break;
            }

            else
            {
                // error happens!
                PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::getNextFrame Fails Error Code %d", status));
                if (ipBSO->reset())
                {
                    PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::CalculateDuration (Out)"));
                    return false;
                }

                return false;
            }
            FrameCount--;
        }
#endif

        ResetPlayback(0);
    }
    PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::CalculateDuration (Out)"));
    return true;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::RetreiveAC3FileInfo
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    aMedia     = pointer to CPVMedia class
//    aTrackId   = pointer to the ID specific to the current AC3 track
//    aTimescale = pointer to the sampling frequency value, for AC3 it is 8000 Hz.
//
//  Outputs:
//    None
//
//  Returns:
//    false if an error happens, else true
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  This function opens the AC3 file, checks for AC3 format type, calculates
//  the track duration, and sets the AC3 bitrate value.
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF bool CAC3FileParser::RetrieveFileInfo(TPVAc3FileInfo& aInfo)
{
    aInfo.iBitrate = iAC3BitRate;
    aInfo.iSamplingRate = iAC3SamplingRate;
    aInfo.iDuration = iAC3TrackDuration;
    aInfo.iFileSize = iAC3FileSize;
    aInfo.iTimescale = 1000; /* TODO: this is a dummy value, need to check the interpretition of this field */
    PVMF_AC3PARSER_LOGDIAGNOSTICS((0, "CAC3FileParser::RetrieveFileInfo- duration = %d, bitrate = %d, filesize = %d, sampling rate = %d", iAC3TrackDuration, iAC3BitRate, iAC3FileSize, iAC3SamplingRate));

    return true;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::ResetPlayback
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    aStartTime = integer value as where to move the playback positioning to.
//
//  Outputs:
//    None
//
//  Returns:
//    0 if success, -1 if failure
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  This function sets the file pointer to the location that aStartTime would
//  point to in the file.
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF int32 CAC3FileParser::ResetPlayback(int32 aStartTime)
{
    // get file size info, //iAC3File.Size(fileSize)
    if (iAC3FileSize <= 0)
    {
        int32 frameTypeIndex;
        if (ipBSO->getFileInfo(iAC3FileSize, iAC3SamplingRate, iAC3BitRate, iAC3FrameSize))
        {
            PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::Reset Playback Failed"));
            return ac3bitstreamObject::MISC_ERROR;
        }
        iAC3FrameDuration = AC3SamplingRateToFrameDuration (iAC3SamplingRate);
    }

    iEndOfFileReached = false;
    // initialize "iTotalNumFramesRead"
    // note: +1 means we choose the next frame(ts>=aStartTime)
    iTotalNumFramesRead = aStartTime / (iAC3FrameDuration / 1000) + (aStartTime > 0) * 1;

    uint32 tblIdx = aStartTime / (iRandomAccessTimeInterval) + (aStartTime>0)*1;
    iTotalNumFramesRead = tblIdx * iCountToClaculateRDATimeInterval;

    PVMF_AC3PARSER_LOGDIAGNOSTICS((0, "CAC3FileParser::resetplayback - TotalNumFramesRead=%d", iTotalNumFramesRead));
    // set new file position
    int32 newPosition = 0;
    if (iTotalNumFramesRead > 0)
    {
        // At the first time, don't do reset
        if (iAC3TrackDuration != 0 && iRPTable.size() == 0)
        {
            newPosition = (iAC3FileSize * aStartTime) / iAC3TrackDuration;
            PVMF_AC3PARSER_LOGDIAGNOSTICS((0, "CAC3FileParser::resetplayback - newPosition=%d", newPosition));
            if (newPosition < 0)
            {
                // if we have no duration information, reset the file position at 0.
                newPosition = 0;
            }
        }
        else if (iRPTable.size() > 0)
        {
            // use the randow positioning table to determine the file position
            if (tblIdx  >= iRPTable.size())
            {
                // Requesting past the end of table so set to (end of table-1)
                // to be at the last sample
                tblIdx = ((int32)iRPTable.size()) - 2;
            }
            newPosition = iRPTable[tblIdx];
        }
    }

    if (newPosition >= 0 && ipBSO->reset(newPosition))
    {
        PVMF_AC3PARSER_LOGERROR((0, "AC3BitstreamObject::refill- Misc Error"));
        return ac3bitstreamObject::MISC_ERROR;
    }
    iEndOfFileReached = false;

    return ac3bitstreamObject::EVERYTHING_OK;
}


int32  ac3bitstreamObject::parseAC3Header (uint8 *pBuffer, int32& samplingrate, int32& bitrate, int32& framesize)
{
    int32 fscod = 0;
    int32 framesizecod = 0;

    if ((pBuffer[0] != 0x0B) || (pBuffer[1] != 0x77)) {
      return ac3bitstreamObject::MISC_ERROR;
    }

    fscod = (int32) AC3_DECODE_FSCOD (pBuffer[4]);
    framesizecod = (int32) AC3_DECODE_FRAMESIZECOD (pBuffer[4]);

    if ((fscod > 2) || (framesizecod > 37)) {
      return ac3bitstreamObject::MISC_ERROR;
    }

    samplingrate = AC3_TBL_SAMPLING_RATE (fscod);
    bitrate       = AC3_TBL_BIT_RATE      (framesizecod);
    if (fscod < 3) {
      if (fscod == 0)
        framesize    = AC3_TBL_FRAME_SIZE    (framesizecod, 2);
      else if (fscod == 2)
        framesize    = AC3_TBL_FRAME_SIZE    (framesizecod, 0);
      else
        framesize    = AC3_TBL_FRAME_SIZE    (framesizecod, 1);
    }

    return ac3bitstreamObject::EVERYTHING_OK;
}



//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::SeekPointFromTimestamp
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    aStartTime = integer value as for the specified start time
//
//  Outputs:
//    None
//
//  Returns:
//    Timestamp in milliseconds of the actual position
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  This function returns the timestamp for an actual position corresponding
//  to the specified start time
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF uint32 CAC3FileParser::SeekPointFromTimestamp(uint32 aStartTime)
{
    // get file size info, //iAC3File.Size(fileSize)
    if (iAC3FileSize <= 0)
    {
        int32 frameTypeIndex;
        if (ipBSO->getFileInfo(iAC3FileSize, iAC3SamplingRate, iAC3BitRate, iAC3FrameSize))
        {
            return 0;
        }
        iAC3FrameDuration = AC3SamplingRateToFrameDuration (iAC3SamplingRate);
    }

    // Determine the frame number corresponding to timestamp
    // note: +1 means we choose the next frame(ts>=aStartTime)
    uint32 startframenum = (aStartTime * 1000 / iAC3FrameDuration) + (aStartTime > 0) * 1;

    // Correct the frame number if necessary
    if (startframenum > 0)
    {
        if (iAC3TrackDuration != 0 && iRPTable.size() <= 0)
        {
            // Duration not known and reposition table not available so go to first frame
            startframenum = 0;
        }
        else if (iRPTable.size() > 0)
        {
            if (startframenum >= iRPTable.size())
            {
                // Requesting past the end of table so set to (end of table-1)
                // to be at the last sample
                startframenum = ((int32)iRPTable.size()) - 2;
            }
        }
    }

    return (startframenum * (iAC3FrameDuration / 1000));
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CAC3FileParser::GetNextBundledAccessUnits
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    aNumSamples = requested number of frames to be read from file
//    aGau        = frame information structure of type GAU
//
//  Outputs:
//    None
//
//  Returns:
//    0 if success, -1 if failure
//
//  Global Variables Used:
//    None
//
//----------------------------------------------------------------------------
// FUNCTION DESCRIPTION
//
//  This function attempts to read in the number of AC3 frames specified by
//  aNumSamples. It formats the read data to WMF bit order and stores it in
//  the GAU structure.
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF int32 CAC3FileParser::GetNextBundledAccessUnits(uint32 *aNumSamples, GAU *aGau)
{
    int32 frame_duration_ms = 0;
    int32 frame_length = 0;
    // Check the requested number of frames is not greater than the max supported
    PVMF_AC3PARSER_LOGINFO((0, "CAC3FileParser::GetNextBundledAccessUnits (In) NumSamples requested = %d", *aNumSamples));
    if (*aNumSamples > AC3_MAX_NUM_FRAMES_PER_BUFF)
    {
        PVMF_AC3PARSER_LOGERROR((0, "CAC3FileParser::GetNextBundledAccessUnits Failed - requested number of frames is greater than the max supported"));
        return ac3bitstreamObject::MISC_ERROR;
    }

    int32 returnValue = ac3bitstreamObject::EVERYTHING_OK;

    if (iEndOfFileReached)
    {
        *aNumSamples = 0;
        return ac3bitstreamObject::END_OF_FILE;
    }

    uint8* pTempGau = (uint8 *) aGau->buf.fragments[0].ptr;
    uint32 gauBufferSize = aGau->buf.fragments[0].len;
    uint32 i, bytesReadInGau = 0, numSamplesRead = 0;

    for (i = 0; i < *aNumSamples && !iEndOfFileReached; i++)
    {
        // get the next frame
        bool bHeaderIncluded = true;
        returnValue = ipBSO->getNextFrame(iAC3FrameBuffer, frame_length, frame_duration_ms);
        if (returnValue == ac3bitstreamObject::END_OF_FILE)
        {
            PVMF_AC3PARSER_LOGINFO((0, "CAC3FileParser::GetNextBundledAccessUnits Got End of File"));
            iEndOfFileReached = true;
            break;
        }
        else if (returnValue == ac3bitstreamObject::EVERYTHING_OK)
        {
        }
        else
        {   // error happens!!
            *aNumSamples = 0;
            return ac3bitstreamObject::READ_ERROR;
        }

        // Now a frame exists in iAC3FrameBuffer, move it to aGau
        // Check whether the gau buffer will be overflow
        if (bytesReadInGau + frame_length >= gauBufferSize)
        {
            // Undo the read
            PVMF_AC3PARSER_LOGINFO((0, "CAC3FileParser::GetNextBundledAccessUnits undoing getnextframe"));
            PVMF_AC3PARSER_LOGINFO((0, "CAC3FileParser::bytesreadingau=%d, framelength=%d,gaubuffersize=%d",bytesReadInGau,
                                                 frame_length, gauBufferSize));
            ipBSO->undoGetNextFrame(frame_length);
            break;
        }

        if (frame_length> 0)
        {
            oscl_memcpy(pTempGau, iAC3FrameBuffer, frame_length);

            pTempGau += frame_length;
            bytesReadInGau += frame_length;
        }
        aGau->info[i].len = frame_length;
        numSamplesRead++;
        aGau->info[i].ts  = (iTotalNumFramesRead + (numSamplesRead)) * (iAC3FrameDuration / 1000);

    } // end of: for(i = 0; i < *aNumSamples && !iEndOfFileReached; i++)


    PVMF_AC3PARSER_LOGINFO((0, "CAC3FileParser::GetNextBundledAccessUnits Calculating timestamp: numSamplesRead=%d, FrameDuration=%d, iTotalNumFramesRead= %d, timestamp=%d", numSamplesRead, iAC3FrameDuration, iTotalNumFramesRead, iTotalNumFramesRead * (iAC3FrameDuration/1000)));
    aGau->info[0].ts = numSamplesRead * (iAC3FrameDuration / 1000);

    *aNumSamples = numSamplesRead;
    iTotalNumFramesRead += numSamplesRead;

    //We may have reached EOF but also found some samples.
    //don't return EOF until there are no samples left.
    if (returnValue == ac3bitstreamObject::END_OF_FILE
            && numSamplesRead > 0)
        return ac3bitstreamObject::EVERYTHING_OK;

    return returnValue;
}

OSCL_EXPORT_REF int32 CAC3FileParser::PeekNextTimestamp(uint32 *aTimestamp)
{

    *aTimestamp = iTotalNumFramesRead * (iAC3FrameDuration / 1000);

    return ac3bitstreamObject::EVERYTHING_OK;
}

OSCL_EXPORT_REF int32 CAC3FileParser::getTrackMaxBufferSizeDB()
{
    if (ipBSO)
    {
        return ipBSO->getTrackMaxBufferSizeDB();
    }
    else
    {
        return -1;
    }
}


OSCL_EXPORT_REF uint8* CAC3FileParser::getCodecSpecificInfo()
{
    return 	NULL;
}


