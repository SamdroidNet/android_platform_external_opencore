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
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                 E V R C    F I L E    P A R S E R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


/**
 *  @file g711fileparser.cpp
 *  @brief This file contains the implementation of the raw GSM-G711 file parser.
 */

//----------------------------------------------------------------------------
// INCLUDES
//----------------------------------------------------------------------------
#include "g711fileparser.h"

// Use default DLL entry point for Symbian
#include "oscl_dll.h"
OSCL_DLL_ENTRY_POINT_DEFAULT()

//! specially used in ResetPlayback(), re-position the file pointer
int32 g711bitstreamObject::reset(int32 filePos)
{
    iFrame_type = 0;
    iBytesRead  = iInitFilePos + filePos; // set the initial value
    iBytesProcessed = iBytesRead;
    if (ipG711File)
    {
        ipG711File->Seek(iInitFilePos + filePos, Oscl_File::SEEKSET);
    }
    iPos = g711bitstreamObject::MAIN_BUFF_SIZE + g711bitstreamObject::SECOND_BUFF_SIZE;
    return refill();
}

//! read data from bitstream, this is the only function to read data from file
int32 g711bitstreamObject::refill()
{
    PVMF_G711PARSER_LOGDEBUG((0, "Refill In ipos=%d, iBytesRead=%d, iBytesProcessed=%d, iActualSize=%d, iFileSize=%d", iPos, iBytesRead, iBytesProcessed, iActual_size, iFileSize));

    if (iBytesRead > 0 && iFileSize > 0 && iBytesRead >= iFileSize)
    {
        // if number of bytes read so far exceed the file size,
        // then first update the file size (PDL case).
        if (!UpdateFileSize()) return g711bitstreamObject::MISC_ERROR;

        //At this point we're within 32 bytes of the end of data.
        //Quit reading data but don't return EOF until all data is processed.
        if (iBytesProcessed < iBytesRead)
        {
            return g711bitstreamObject::EVERYTHING_OK;
        }
        else
        {
            //there is no more data to read.
            if (iBytesRead >= iFileSize || iBytesProcessed >= iFileSize)
                return g711bitstreamObject::DATA_INSUFFICIENT;
        }
    }

    if (!ipG711File)
    {
        return g711bitstreamObject::MISC_ERROR;
    }

    // Get file size at the very first time
    if (iFileSize == 0)
    {
        if (ipG711File->Seek(0, Oscl_File::SEEKEND))
        {
            return g711bitstreamObject::MISC_ERROR;
        }

        iFileSize = ipG711File->Tell();

        if (iFileSize <= 0)
        {
            return g711bitstreamObject::MISC_ERROR;
        }

        if (ipG711File->Seek(0, Oscl_File::SEEKSET))
        {
            return g711bitstreamObject::MISC_ERROR;
        }

        // first-time read, set the initial value of iPos
        iPos = g711bitstreamObject::SECOND_BUFF_SIZE;
        iBytesProcessed = 0;
    }
    // we are currently positioned at the end of the data buffer.
    else if (iPos == g711bitstreamObject::MAIN_BUFF_SIZE + g711bitstreamObject::SECOND_BUFF_SIZE)
    {
        // reset iPos and refill from the beginning of the buffer.
        iPos = g711bitstreamObject::SECOND_BUFF_SIZE;
    }

    else if (iPos >= iActual_size)
    {
        int32 len = 0;
        // move the remaining stuff to the beginning of iBuffer
        if (iActual_size + g711bitstreamObject::SECOND_BUFF_SIZE > iPos)
        {
            // we are currently positioned within SECOND_BUFF_SIZE bytes from the end of the buffer.
            len = iActual_size + g711bitstreamObject::SECOND_BUFF_SIZE - iPos;
        }
        else
        {
            // no leftover data.
            len = 0;
        }

        oscl_memcpy(&iBuffer[g711bitstreamObject::SECOND_BUFF_SIZE-len], &iBuffer[iPos], len);
        iPos = g711bitstreamObject::SECOND_BUFF_SIZE - len;

        // update the file size for the PDL scenario where more data has been downloaded
        // into the file but the file size has not been updated yet.
        if (iBytesRead + iMax_size > iFileSize)
        {
            if (!UpdateFileSize()) return g711bitstreamObject::MISC_ERROR;
        }
    }

    // read data
    if ((iActual_size = ipG711File->Read(&iBuffer[g711bitstreamObject::SECOND_BUFF_SIZE], 1, iMax_size)) == 0)
    {
        return g711bitstreamObject::READ_ERROR;
    }

    iBytesRead += iActual_size;

    PVMF_G711PARSER_LOGDEBUG((0, "Refill Out ipos=%d, iBytesRead=%d, iBytesProcessed=%d, iActualSize=%d, iFileSize=%d", iPos, iBytesRead, iBytesProcessed, iActual_size, iFileSize));

    return g711bitstreamObject::EVERYTHING_OK;
}

//! most important function to get one frame data plus frame type, used in getNextBundledAccessUnits()
int32 g711bitstreamObject::getNextFrame(uint8* frameBuffer, uint8& frame_type)
{
    PVMF_G711PARSER_LOGDEBUG((0, "GetNextFrame In ipos=%d, iBytesRead=%d, iBytesProcessed=%d, iActualSize=%d, iFileSize=%d", iPos, iBytesRead, iBytesProcessed, iActual_size, iFileSize));
    if (!frameBuffer)
    {
        return g711bitstreamObject::MISC_ERROR;
    }

    int32 ret_value = g711bitstreamObject::EVERYTHING_OK;

    // Need to refill?
    if (iFileSize == 0 || iPos >= iActual_size)
    {
        ret_value = refill();
        if (ret_value)
        {
            return ret_value;
        }
    }

    int32 frame_size = 160;
    uint8 *pBuffer = &iBuffer[iPos];

	if (iPos + 1 >= iActual_size)
	{
		ret_value = refill();
		if (ret_value)
		{
			return ret_value;
		}
	}
	
	pBuffer = &iBuffer[iPos];
	//Need to check if this size varies according to the framerate
	frame_size = 160 ; //IetfDecInputBytes[(uint16)frame_type];

    if (frame_size > 0)
    {
		oscl_memcpy(frameBuffer, &pBuffer[0], frame_size);   // With frame header
    }
    iPos += frame_size;
    iBytesProcessed += frame_size;
    return ret_value;
}

//! parse the IETF bitstream header: "#!G711" 
int32 g711bitstreamObject::parseG711Header()
{
    int32 returnValue = reset();
    if (returnValue == g711bitstreamObject::EVERYTHING_OK)
    {

        // first time read, we don't use iSecond_buffer
        uint8 *pBuffer = &iBuffer[iPos];
        if (iActual_size >= 6 &&
                pBuffer[0] == '#' &&
                pBuffer[1] == '!' &&
                pBuffer[2] == 'G' &&
                pBuffer[3] == '7' &&
                pBuffer[4] == '1' &&
				pBuffer[5] == '1')
        {
			iInitFilePos = 7;			
			iPos += iInitFilePos;
			iBytesProcessed += iInitFilePos;
		}
    }

    return returnValue;
}

//! get clip information: file size and frame_type(bitrate)
int32 g711bitstreamObject::getFileInfo(int32& fileSize, int32& frame_type)
{
    fileSize = 0;
    int32 ret_value = g711bitstreamObject::EVERYTHING_OK;
    if (iFileSize == 0)
    {
        ret_value = parseG711Header();
        if (ret_value)
        {
            return ret_value;
        }
    }
    fileSize = iFileSize;
    frame_type = iFrame_type;
    return ret_value;
}

//! get the updated file size
bool g711bitstreamObject::UpdateFileSize()
{
    if (ipG711File != NULL)
    {
        uint32 aRemBytes = 0;
        if (ipG711File->GetRemainingBytes(aRemBytes))
        {
            uint32 currPos = (uint32)(ipG711File->Tell());
            iFileSize = currPos + aRemBytes;
            return true;
        }
    }
    return false;
}


//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::CG711FileParser
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
//  Constructor for CG711FileParser class
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF CG711FileParser::CG711FileParser(void)
{
    iG711Duration        = -1;
    iTotalNumFramesRead = 0;
    iEndOfFileReached   = false;
    iRandomAccessTimeInterval = 0;
    iCountToClaculateRDATimeInterval = 0;
    iLogger = PVLogger::GetLoggerObject("pvg711_parser");
    iDiagnosticLogger = PVLogger::GetLoggerObject("playerdiagnostics.pvg711_parser");

    ipBSO = NULL;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::~CG711FileParser
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
//  Destructor for CG711FileParser class
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF CG711FileParser::~CG711FileParser(void)
{
    iG711File.Close();
    OSCL_DELETE(ipBSO);
    ipBSO = NULL;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::InitG711File
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    iClip = pointer to the G711 file name to be played of type TPtrC
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
//  This function opens the G711 file, checks for G711 format type, calculates
//  the track duration, and sets the G711 bitrate value.
//
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF bool CG711FileParser::InitG711File(OSCL_wString& aClip, bool aInitParsingEnable, Oscl_FileServer* aFileSession, PVMFCPMPluginAccessInterfaceFactory*aCPM, OsclFileHandle*aHandle, uint32 countToClaculateRDATimeInterval)
{
    iG711File.SetCPM(aCPM);
    iG711File.SetFileHandle(aHandle);

    // Open the file (aClip)
    if (iG711File.Open(aClip.get_cstr(), (Oscl_File::MODE_READ | Oscl_File::MODE_BINARY), *aFileSession) != 0)
    {
        PVMF_G711PARSER_LOGERROR((0, "CG711FileParser::InitG711File- File Open failed"));
        return false;
    }

    // create ipBSO
    ipBSO = OSCL_NEW(g711bitstreamObject, (iLogger, &iG711File));
    if (!ipBSO)
    {
        return false;
    }
    if (!ipBSO->get())
    {
        return false; // make sure the memory allocation is going well
    }

    // get file info
    int32 frameTypeIndex = 0;
    if (ipBSO->getFileInfo(iG711FileSize, frameTypeIndex))
    {
        PVMF_G711PARSER_LOGERROR((0, "CG711FileParser::InitG711File- getFileInfo failed "));
        return false;
    }
	// Determine file duration and set up random positioning table if needed
    CalculateDuration(aInitParsingEnable, countToClaculateRDATimeInterval);
    return true;
}

bool CG711FileParser::CalculateDuration(bool aInitParsingEnable, uint32 countToClaculateRDATimeInterval)
{
    iCountToClaculateRDATimeInterval = countToClaculateRDATimeInterval;
    uint32 FrameCount = iCountToClaculateRDATimeInterval;
    iRandomAccessTimeInterval = countToClaculateRDATimeInterval * TIME_STAMP_PER_FRAME;
	
    if (aInitParsingEnable)
    {
        // Go through each frame to calculate G711 file duration.
        int32 status = g711bitstreamObject::EVERYTHING_OK;
        uint8 frame_type = 0;
        iG711Duration = 0;
        int32 error = 0;
        int32 filePos = 0;

        OSCL_TRY(error, iRPTable.push_back(filePos));
        OSCL_FIRST_CATCH_ANY(error, return false);

        while (status == g711bitstreamObject::EVERYTHING_OK)
        {
            // get the next frame

            status = ipBSO->getNextFrame(iG711FrameBuffer, frame_type); // NO IETF frame header

            if (status == g711bitstreamObject::EVERYTHING_OK)
            {
                // calculate the number of frames // BX
                iG711Duration += TIME_STAMP_PER_FRAME;

                // set up the table for randow positioning
                int32 frame_length = 160;
                filePos += frame_length;

                error = 0;
                if (!FrameCount)
                {
                    OSCL_TRY(error, iRPTable.push_back(filePos));
                    OSCL_FIRST_CATCH_ANY(error, return false);
                    FrameCount = countToClaculateRDATimeInterval;
                }
            }

            else if (status == g711bitstreamObject::END_OF_FILE)
            {
                break;
            }

            else
            {
                // error happens!
                PVMF_G711PARSER_LOGERROR((0, "CG711FileParser::getNextFrame Fails Error Code %d", status));
                if (ipBSO->reset())
                {
                    return false;
                }

                return false;
            }
            FrameCount--;
        }

        ResetPlayback(0);
    }
    return true;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::RetrieveG711FileInfo
//----------------------------------------------------------------------------
// INPUT AND OUTPUT DEFINITIONS
//
//  Inputs:
//    aMedia     = pointer to CPVMedia class
//    aTrackId   = pointer to the ID specific to the current G711 track
//    aTimescale = pointer to the sampling frequency value, for G711 it is 8000 Hz.
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
//  This function opens the G711 file, checks for G711 format type, calculates
//  the track duration, and sets the G711 bitrate value.
//
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF bool CG711FileParser::RetrieveFileInfo(TPVG711FileInfo& aInfo)
{
    aInfo.iTimescale = 1000;
    aInfo.iDuration = iG711Duration;
    aInfo.iFileSize = iG711FileSize;
    PVMF_G711PARSER_LOGDIAGNOSTICS((0, "CG711FileParser::RetrieveFileInfo- duration = %d,filesize = %d", iG711Duration, iG711FileSize));

    return true;
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::ResetPlayback
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
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF int32 CG711FileParser::ResetPlayback(int32 aStartTime)
{
	// get file size info, //iG711File.Size(fileSize)
    int32 result;
    if (iG711FileSize <= 0)
    {
        int32 frameTypeIndex;
        if (ipBSO->getFileInfo(iG711FileSize, frameTypeIndex))
        {
            PVMF_G711PARSER_LOGERROR((0, "CG711FileParser::Reset Playback Failed"));
            return g711bitstreamObject::MISC_ERROR;
        }
    }

    iEndOfFileReached = false;
    // initialize "iTotalNumFramesRead"
    // note: +1 means we choose the next frame(ts>=aStartTime)
    iTotalNumFramesRead = aStartTime / TIME_STAMP_PER_FRAME + (aStartTime > 0) * 1;

    uint32 tblIdx = aStartTime / (iRandomAccessTimeInterval);// +(aStartTime>0)*1;
    iTotalNumFramesRead = tblIdx * iCountToClaculateRDATimeInterval;

    PVMF_G711PARSER_LOGDIAGNOSTICS((0, "CG711FileParser::resetplayback - TotalNumFramesRead=%d", iTotalNumFramesRead));
    // set new file position
    int32 newPosition = 0;
    if (iTotalNumFramesRead > 0)
    {
        // At the first time, don't do reset
        if (iG711Duration != 0 && iRPTable.size() == 0)
        {
            newPosition = (iG711FileSize * aStartTime) / iG711Duration;
            PVMF_G711PARSER_LOGDIAGNOSTICS((0, "CG711FileParser::resetplayback - newPosition=%d", newPosition));
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
    result = ipBSO->reset(newPosition);
    if (newPosition >= 0 && result)
    {
        PVMF_G711PARSER_LOGERROR((0, "G711BitstreamObject::refill- Misc Error"));
        return result;
    }
    iEndOfFileReached = false;

    return g711bitstreamObject::EVERYTHING_OK;
}


//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::SeekPointFromTimestamp
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
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF uint32 CG711FileParser::SeekPointFromTimestamp(uint32 aStartTime)
{
    // get file size info, //iG711File.Size(fileSize)
    if (iG711FileSize <= 0)
    {
        int32 frameTypeIndex;
        if (ipBSO->getFileInfo(iG711FileSize, frameTypeIndex))
        {
            return 0;
        }
    }

    // Determine the frame number corresponding to timestamp
    // note: +1 means we choose the next frame(ts>=aStartTime)
    uint32 startframenum = aStartTime / TIME_STAMP_PER_FRAME + (aStartTime > 0) * 1;

    // Correct the frame number if necessary
    if (startframenum > 0)
    {
        if (iG711Duration != 0 && iRPTable.size() <= 0)
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

    return (startframenum*TIME_STAMP_PER_FRAME);
}

//----------------------------------------------------------------------------
// FUNCTION NAME: CG711FileParser::GetNextBundledAccessUnits
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
//  This function attempts to read in the number of G711 frames specified by
//  aNumSamples. It formats the read data to WMF bit order and stores it in
//  the GAU structure.
//
//----------------------------------------------------------------------------
// REQUIREMENTS
//
//----------------------------------------------------------------------------
// REFERENCES
//
//------------------------------------------------------------------------------
// CAUTION
//
//------------------------------------------------------------------------------
OSCL_EXPORT_REF int32 CG711FileParser::GetNextBundledAccessUnits(uint32 *aNumSamples, GAU *aGau)
{
    // Check the requested number of frames is not greater than the max supported
    if (*aNumSamples > MAX_NUM_FRAMES_PER_BUFF)
    {
        PVMF_G711PARSER_LOGERROR((0, "CG711FileParser::GetNextBundledAccessUnits Failed - requested number of frames is greater than the max supported"));
        return g711bitstreamObject::MISC_ERROR;
    }

    int32 returnValue = g711bitstreamObject::EVERYTHING_OK;

    if (iEndOfFileReached)
    {
        *aNumSamples = 0;
        return g711bitstreamObject::END_OF_FILE;
    }

    uint8* pTempGau = (uint8 *) aGau->buf.fragments[0].ptr;
    uint32 gauBufferSize = aGau->buf.fragments[0].len;
    uint32 i, bytesReadInGau = 0, numSamplesRead = 0;

    for (i = 0; i < *aNumSamples && !iEndOfFileReached; i++)
    {
        // get the next frame
        returnValue = ipBSO->getNextFrame(iG711FrameBuffer, iG711FrameHeaderBuffer[i]);
        if (returnValue == g711bitstreamObject::END_OF_FILE)
        {
            iEndOfFileReached = true;
            break;
        }
        else if (returnValue == g711bitstreamObject::EVERYTHING_OK)
        {
        }
        else if (returnValue == g711bitstreamObject::DATA_INSUFFICIENT)
		{
			*aNumSamples = 0;
		    return returnValue;
        }
        else
        {   // error happens!!
            *aNumSamples = 0;
            return g711bitstreamObject::READ_ERROR;
        }

        int32 frame_size = 160;
        
        // Check whether the gau buffer will be overflow
        if (bytesReadInGau + frame_size >= gauBufferSize)
        {
            // Undo the read
            ipBSO->undoGetNextFrame(frame_size);
            break;
        }

        if (frame_size > 0)
        {
            oscl_memcpy(pTempGau, iG711FrameBuffer, frame_size);

            pTempGau += frame_size;
            bytesReadInGau += frame_size;
        }
        aGau->info[i].len = frame_size;
        aGau->info[i].ts  = (iTotalNumFramesRead + (numSamplesRead++)) * TIME_STAMP_PER_FRAME;

    } // end of: for(i = 0; i < *aNumSamples && !iEndOfFileReached; i++)

    aGau->info[0].ts = iTotalNumFramesRead * TIME_STAMP_PER_FRAME;

    *aNumSamples = numSamplesRead;
    iTotalNumFramesRead += numSamplesRead;

    //We may have reached EOF but also found some samples.
    //don't return EOF until there are no samples left.
    if (returnValue == g711bitstreamObject::END_OF_FILE
            && numSamplesRead > 0)
        return g711bitstreamObject::EVERYTHING_OK;

    return returnValue;
}

OSCL_EXPORT_REF int32 CG711FileParser::PeekNextTimestamp(uint32 *aTimestamp)
{

    *aTimestamp = iTotalNumFramesRead * TIME_STAMP_PER_FRAME;

    return g711bitstreamObject::EVERYTHING_OK;
}


OSCL_EXPORT_REF uint8 CG711FileParser::GetFrameTypeInCurrentBundledAccessUnits(uint32 frameIndex)
{
    if (frameIndex >= MAX_NUM_FRAMES_PER_BUFF)
    {
        return 0;
    }
    return iG711FrameHeaderBuffer[frameIndex];
}

OSCL_EXPORT_REF int32 CG711FileParser::getTrackMaxBufferSizeDB()
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


OSCL_EXPORT_REF uint8* CG711FileParser::getCodecSpecificInfo()
{
    ipBSO->getNextFrame(iG711FrameBuffer, iG711FrameHeaderBuffer[1]);
    return 	iG711FrameHeaderBuffer;
}


