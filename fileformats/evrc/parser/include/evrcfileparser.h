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

//                 E V R C   F I L E    P A R S E R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


/**
 *  @file evrcfileparser.h
 *  @brief This file defines the raw GSM-EVRC file parser.
 */

#ifndef EVRCFILEPARSER_H_INCLUDED
#define EVRCFILEPARSER_H_INCLUDED

//----------------------------------------------------------------------------
// INCLUDES
//----------------------------------------------------------------------------

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef PV_GAU_H
#include "pv_gau.h"
#endif
#ifndef PVFILE_H
#include "pvfile.h"
#endif

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

#define PVMF_EVRCPARSER_LOGDIAGNOSTICS(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG,iDiagnosticLogger,PVLOGMSG_INFO,m);
#define PVMF_EVRCPARSER_LOGERROR(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_REL,iLogger,PVLOGMSG_ERR,m);
#define PVMF_EVRCPARSER_LOGDEBUG(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_REL,iLogger,PVLOGMSG_DEBUG,m);

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------

#define EVRC_MAX_NUM_PACKED_INPUT_BYTES  24 /* Max number of packed input bytes     */
#define MAX_NUM_FRAMES_PER_BUFF     341 /* Max number of frames queued per call */
#define NUM_BITS_PER_BYTE            8 /* There are 8 bits in a byte           */
#define TIME_STAMP_PER_FRAME        20 //160 /* Time stamp value per frame of EVRC */


typedef struct
{
    int32 iBitrate;
    int32 iTimescale;
    int32 iDuration;
    int32 iFileSize;
} TPVEvrcFileInfo;

//----------------------------------------------------------------------------
// FORWARD CLASS DECLARATIONS
//----------------------------------------------------------------------------


/**
 *  @brief The evrcbitstreamObject Class is the class used by the EVRC parser to
 *  manipulate the bitstream read from the file.
 */
class evrcbitstreamObject
{
    public:
        enum
        {
            MAIN_BUFF_SIZE = 8184,
            SECOND_BUFF_SIZE = 24,//assuming max frame size of EVRC as 24

            // error types for GetNextBundledAccessUnits(),
            // the definition is consistent with MP4_ERROR_CODE in iSucceedFail.h
            MISC_ERROR = -2,
            READ_ERROR = -1,
            EVERYTHING_OK = 0,
            END_OF_FILE = 62,
            DATA_INSUFFICIENT = 141
        };

        /**
        * @brief Constructor
        *
        * @param pFile Pointer to file pointer containing bitstream
        * @returns None
        */
        evrcbitstreamObject(PVLogger *aLogger, PVFile* pFile = NULL)
        {
            oscl_memset(this, 0, sizeof(evrcbitstreamObject));
            iLogger = aLogger;
            init(pFile);
            iBuffer = OSCL_ARRAY_NEW(uint8, evrcbitstreamObject::MAIN_BUFF_SIZE + evrcbitstreamObject::SECOND_BUFF_SIZE);
            if (iBuffer)
            {
                iStatus = true;
            }
            else
            {
                iStatus = false;
            }
        }

        /**
        * @brief Destructor
        *
        * @param None
        * @returns None
        */
        ~evrcbitstreamObject()
        {
            /*init();*/
            if (iBuffer)
            {
                OSCL_ARRAY_DELETE(iBuffer);
                iBuffer = NULL;
            }
        }

        /**
        * @brief Returns current bitstream status
        *
        * @param None
        * @returns true=Bitstream instantiated; false=no bitstream
        */
        inline bool get()
        {
            return iStatus;
        }

        /**
        * @brief Re-positions the file pointer. Specially used in ResetPlayback()
        *
        * @param filePos Position in file to move to.
        * @returns None
        */
        int32 reset(int32 filePos = 0);

        /**
        * @brief Retrieves clip information: file size, format(WMF or IF2) and frame_type(bitrate)
        *
        * @param fileSize Size of file
        * @param format EVRC format
        * @param frame_type Frame type
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32  getFileInfo(int32& fileSize, int32& frame_type);

        /**
        * @brief Retrieves one frame data plus frame type, used in getNextBundledAccessUnits().
        *
        * @param frameBuffer Buffer containing frame read
        * @param frame_type Frame type of frame
        * @param bHeaderIncluded Indicates whether to include header or not in buffer
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32  getNextFrame(uint8* frameBuffer, uint8& frame_type);

        /**
        * @brief Undo getNextFrame in case gau buffer overflow occurs when getting next frame
        *
        * @param offset Number of bytes to move back from current position in file
        * @returns None
        */
        void undoGetNextFrame(int32 offset)
        {
            iPos -= offset;
        }

        /**
        * @brief Returns the maximum buffer size of the track
        *
        * @param None
        * @returns Maximum buffer size
        */
        int32 getTrackMaxBufferSizeDB()
        {
            return iMax_size;
        }

    private:

        /**
        * @brief Initialization
        *
        * @param pFile File pointer
        * @returns None
        */
        inline void init(PVFile* pFile = NULL)
        {
            iFileSize = 0;
            iBytesRead = 0;
            iBytesProcessed = 0;
            ipEVRCFile = pFile;
            iActual_size = iMax_size = evrcbitstreamObject::MAIN_BUFF_SIZE;
            iPos = evrcbitstreamObject::MAIN_BUFF_SIZE + evrcbitstreamObject::SECOND_BUFF_SIZE;
            iFrame_type = 0;
            iInitFilePos = 0;

            if (ipEVRCFile)
            {
                ipEVRCFile->Seek(0, Oscl_File::SEEKSET);
            }
        }

        /**
        * @brief Reads data from bitstream, this is the only function to read data from file
        *
        * @param None
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32 refill();

        /**
        * @brief Parses the EVRC bitstream header: "$!EVRC"
        *
        * @param None
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32 parseEVRCHeader();


		/**
		* @brief Gets the updated file size
		*
		* @param None
		* @returns Result of operation: true/false.
		*/
		bool UpdateFileSize();




    private:
        int32 iPos;             // pointer for iBuffer[]
        int32 iActual_size;     // number of bytes read from a file once <= max_size
        int32 iMax_size;        // max_size = bitstreamStruc::MAIN_BUFF_SIZE
        int32 iBytesRead;       // (cumulative) number of bytes read from a file so far
        int32 iBytesProcessed;
        int32 iFileSize;        // file size of the ipEVRCFile
        uint32 iInitFilePos;    // For IETF bitstream, iInitFilePos = IETF header size
        int32 iFrame_type;      // frame type got from the very first frame

        uint8 *iBuffer;
        PVFile* ipEVRCFile; // bitstream file
        bool iStatus;
        PVLogger *iLogger;

};

/**
 *  @brief The CEVRCFileParser Class is the class that will construct and maintain
 *  all the necessary data structures to be able to render a valid EVRC file
 *  to disk.
 *
 *  This class supports the following EVRC file format specs: IF2, WMF, ETS
 */
class PVMFCPMPluginAccessInterfaceFactory;
class CEVRCFileParser
{
    public:
        typedef OsclMemAllocator alloc_type;

        /**
        * @brief Constructor
        *
        * @param None
        * @returns None
        */
        OSCL_IMPORT_REF  CEVRCFileParser();

        /**
        * @brief Destructor
        *
        * @param None
        * @returns None
        */
        OSCL_IMPORT_REF ~CEVRCFileParser();

        /**
        * @brief Opens the specified clip and performs initialization of the parser
        *
        * @param aClip Filename to parse
        * @param aInitParsingEnable Indicates whether to setup random positioning (true)
        * or not (false)
        * @param aFileSession Pointer to opened file server session. Used when opening
        * and reading the file on certain operating systems.
        * @returns true if the init succeeds, else false.
        */
        OSCL_IMPORT_REF bool InitEVRCFile(OSCL_wString& aClip, bool aInitParsingEnable = true, Oscl_FileServer* aFileSession = NULL, PVMFCPMPluginAccessInterfaceFactory*aCPM = NULL, OsclFileHandle*aHandle = NULL, uint32 countToClaculateRDATimeInterval = 1);

        /**
        * @brief Retrieves information about the clip such as bit rate, sampling frequency, etc.
        *
        * @param aInfo Storage for information retrieved
        * @returns True if successful, False otherwise.
        */
        OSCL_IMPORT_REF bool RetrieveFileInfo(TPVEvrcFileInfo& aInfo);

        /**
        * @brief Resets the parser variables to allow start of playback at a new position
        *
        * @param aStartTime Time where to start playback from
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        OSCL_IMPORT_REF int32  ResetPlayback(int32 aStartTime = 0);

        /**
        * @brief Returns the actual starting timestamp for a specified start time
        *
        * @param aStartTime Time where to start playback from
        * @returns Timestamp corresponding to the actual start position
        */
        OSCL_IMPORT_REF uint32 SeekPointFromTimestamp(uint32 aStartTime = 0);

        /**
        * @brief Attempts to read in the number of EVRC frames specified by aNumSamples.
        * It formats the read data to WMF bit order and stores it in the GAU structure.
        *
        * @param aNumSamples Requested number of frames to be read from file
        * @param aGau Frame information structure of type GAU
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        OSCL_IMPORT_REF int32  GetNextBundledAccessUnits(uint32 *aNumSamples, GAU *aGau);

        /**
        * @brief Returns the value of the next timestamp that will be
        *    returned in a call to GetNextBundledAccessUnits.
        *
        * @param aTimestamp returned value of next timestamp
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        OSCL_IMPORT_REF int32  PeekNextTimestamp(uint32 *aTimestamp);

        /**
        * @brief Returns the frame type of the current EVRC frame
        *
        * @param aFrameIndex Index to frame
        * @returns Frame type
        */
        OSCL_IMPORT_REF uint8  GetFrameTypeInCurrentBundledAccessUnits(uint32 aFrameIndex);

        /**
        * @brief Returns the maximum buffer size of the track
        *
        * @param None
        * @returns Buffer size
        */
        OSCL_IMPORT_REF int32  getTrackMaxBufferSizeDB();

        OSCL_IMPORT_REF uint8*  getCodecSpecificInfo();

    private:
        PVFile			iEVRCFile;
        int32           iEVRCDuration;
        int32           iEVRCFileSize;
        int32           iTotalNumFramesRead;
        bool            iEndOfFileReached;
        evrcbitstreamObject *ipBSO;
        Oscl_Vector<int32, alloc_type> iRPTable; // table containing sync indexes for repositioning

        /* Decoder input buffer for 1 raw encoded speech frame (IETF or IF2) */
        uint8 iEVRCFrameBuffer[EVRC_MAX_NUM_PACKED_INPUT_BYTES];
        uint8 iEVRCFrameHeaderBuffer[MAX_NUM_FRAMES_PER_BUFF];
        uint32 iRandomAccessTimeInterval;
        uint32 iCountToClaculateRDATimeInterval;
        bool CalculateDuration(bool aInitParsingEnable, uint32 countToClaculateRDATimeInterval);
        PVLogger*     iLogger;
        PVLogger*     iDiagnosticLogger;

};

#endif //EVRCFILEPARSER_H_INCLUDED

