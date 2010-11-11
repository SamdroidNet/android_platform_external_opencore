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
 *  @file ac3fileparser.h
 *  @brief This file defines the AC3 file parser.
 */

#ifndef AC3FILEPARSER_H_INCLUDED
#define AC3FILEPARSER_H_INCLUDED

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

#define PVMF_AC3PARSER_LOGDIAGNOSTICS(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG,iDiagnosticLogger,PVLOGMSG_INFO,m);
#define PVMF_AC3PARSER_LOGERROR(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_REL,iLogger,PVLOGMSG_ERR,m);
#define PVMF_AC3PARSER_LOGINFO(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_REL,iLogger,PVLOGMSG_INFO,m);

#define NO_POSSIBLE_MODES 16

//----------------------------------------------------------------------------
// CONSTANTS
//----------------------------------------------------------------------------
#define AC3_MAX_NUM_PACKED_INPUT_BYTES  1920*2 /* Max number of packed input bytes     */
#define AC3_MAX_NUM_FRAMES_PER_BUFF     2 /* Max number of frames queued per call */
#define NUM_BITS_PER_BYTE            8 /* There are 8 bits in a byte           */
#if 0
#define TIME_STAMP_PER_FRAME        20 //160 /* Time stamp value per frame of AMR */
#endif

typedef struct
{
    int32 iBitrate;
    int32 iSamplingRate;
    int32 iDuration;
    int32 iFileSize;
    int32 iTimescale;
} TPVAc3FileInfo;

//----------------------------------------------------------------------------
// FORWARD CLASS DECLARATIONS
//----------------------------------------------------------------------------


/**
 *  @brief The ac3bitstreamObject Class is the class used by the AMR parser to
 *  manipulate the bitstream read from the file.
 */
class ac3bitstreamObject
{
    public:
        enum
        {
            MAIN_BUFF_SIZE = AC3_MAX_NUM_PACKED_INPUT_BYTES*4,
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
        ac3bitstreamObject(PVLogger *aLogger, PVFile* pFile = NULL)
        {
            oscl_memset(this, 0, sizeof(ac3bitstreamObject));
            iLogger = aLogger;
            init(pFile);
            iBuffer = OSCL_ARRAY_NEW(uint8, ac3bitstreamObject::MAIN_BUFF_SIZE);
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
        ~ac3bitstreamObject()
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
        * @param samplingrate AC3 audio sampling rate (in Hz)
        * @param bitrate AC3 encoded audio bitrate (in kbps)
        * @param framesize AC3 frame size (in bytes)
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32  getFileInfo(int32& fileSize, int32& samplingrate, int32& bitrate, int32& framesize);

        /**
        * @brief Retrieves one frame data plus frame type, used in getNextBundledAccessUnits().
        *
        * @param frameBuffer Buffer containing frame read
        * @param frame_len  Length of frame in bytes
        * @param frame_duration_ms Frame duration in milliseconds
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32  getNextFrame(uint8* frameBuffer, int32& frame_len, int32& frame_duration_ms);

        /**
        * @brief Returns the Size of file beeing played.
        *
        */
        int32  getFileSize();

        /**
        * @brief Undo getNextFrame in case gau buffer overflow occurs when getting next frame
        *
        * @param offset Number of bytes to move back from current position in file
        * @returns None
        */
        void undoGetNextFrame(int32 offset)
        {
            iBuffPos -= offset;
        }

        /**
        * @brief Returns the maximum buffer size of the track
        *
        * @param None
        * @returns Maximum buffer size
        */
        int32 getTrackMaxBufferSizeDB()
        {
            return iBuffAllocSize;
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
            iFileBytesRead = 0;
            iFileBytesProcessed = 0;
            ipAC3File = pFile;
            iBuffValidDataSize = iBuffAllocSize = ac3bitstreamObject::MAIN_BUFF_SIZE;
            iBuffPos = 0;

            iSamplingRate = 0;
            iBitRate      = 0;
            iFrameSize    = 0;


            if (ipAC3File)
            {
                ipAC3File->Seek(0, Oscl_File::SEEKSET);
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
        * @brief Parses the AC3 bitstream header (for the first frame) and
        * stores the stream information in context
        *
        * @param None
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
        */
        int32 initAC3StreamParams();
        
        
        /**
        * @brief Gets the updated file size
        *
        * @param None
        * @returns Result of operation: true/false.
        */
        bool UpdateFileSize();

    private:
        int32 iBuffPos;             // pointer for iBuffer[]
        int32 iBuffValidDataSize;     // number of bytes read from a file once <= max_size
        int32 iBuffAllocSize;        // max_size = bitstreamStruc::MAIN_BUFF_SIZE
        int32 iFileBytesRead;       // (cumulative) number of bytes read from a file so far
        int32 iFileBytesProcessed;

        int32 iFileSize;        // file size of the ipAMRFile
        int32 iSamplingRate;    // in Hz
        int32 iBitRate;         // in kbps
        int32 iFrameSize;       // in number of bytes


        uint8 *iBuffer;
        PVFile* ipAC3File; // bitstream file
        bool iStatus;
        PVLogger *iLogger;
/*
        int32 iFrame_type;      // frame type got from the very first frame
        int32 iAmrFormat;       // 0 : WMF ; 1 : IF2 ; >=2 : IETF(AMR, AMR_WB, AMR_MC, AMR_WB_MC)
*/

        /**
         * @brief  Helper function to parse AC3 header
         *
         * @param pBuffer Pointer to buffer containing AC3 byte stream, Should be at least 5 bytes long
         * @param samplingrate Sampling Rate in Hz
         * @param bitrate Bit Rate in kbps
         * @param framesize Frame size in bytes
         *
        * @returns Result of operation: EVERYTHING_OK, READ_FAILED, etc.
         */
        int32  parseAC3Header (uint8 *pBuffer, int32& samplingrate, int32& bitrate, int32& framesize);
};

/**
 *  @brief The CAMRFileParser Class is the class that will construct and maintain
 *  all the necessary data structures to be able to render a valid AMR file
 *  to disk.
 *
 *  This class supports the following AMR file format specs: IF2, WMF, ETS
 */
class PVMFCPMPluginAccessInterfaceFactory;
class CAC3FileParser
{
    public:
        typedef OsclMemAllocator alloc_type;

        /**
        * @brief Constructor
        *
        * @param None
        * @returns None
        */
        OSCL_IMPORT_REF  CAC3FileParser();

        /**
        * @brief Destructor
        *
        * @param None
        * @returns None
        */
        OSCL_IMPORT_REF ~CAC3FileParser();

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
        OSCL_IMPORT_REF bool InitAC3File(OSCL_wString& aClip, bool aInitParsingEnable = true, Oscl_FileServer* aFileSession = NULL, PVMFCPMPluginAccessInterfaceFactory*aCPM = NULL, OsclFileHandle*aHandle = NULL, uint32 countToClaculateRDATimeInterval = 1);

        /**
        * @brief Retrieves information about the clip such as bit rate, sampling frequency, etc.
        *
        * @param aInfo Storage for information retrieved
        * @returns True if successful, False otherwise.
        */
        OSCL_IMPORT_REF bool RetrieveFileInfo(TPVAc3FileInfo& aInfo);

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
        * @brief Attempts to read in the number of AMR frames specified by aNumSamples.
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
        * @brief Returns the frame type of the current AMR frame
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
        int32           iAC3FrameDuration;   /* frame duration in micro seconds */

    private:
        PVFile			    iAC3File;            /* file handle */

        int32           iAC3FileSize;        /* file size in bytes */
        int32           iAC3TrackDuration;   /* trak duration in ms */
        int32           iAC3SamplingRate;    /* sampling freq in Hz */
        int32           iAC3FrameSize;       /* frame size in bytes */
        //int32           iAC3FrameDuration;   /* frame duration in micro seconds */
        int32           iAC3BitRate;         /* bit rate in kbps */

        int32           iTotalNumFramesRead;
        bool            iEndOfFileReached;
        ac3bitstreamObject *ipBSO;
        Oscl_Vector<int32, alloc_type> iRPTable; // table containing sync indexes for repositioning

        PVLogger*     iLogger;
        PVLogger*     iDiagnosticLogger;

        /* Decoder input buffer for one frame */
        uint8 iAC3FrameBuffer[AC3_MAX_NUM_PACKED_INPUT_BYTES];
        /*
        uint8 iAMRFrameHeaderBuffer[MAX_NUM_FRAMES_PER_BUFF];
        */
        uint32 iRandomAccessTimeInterval;
        uint32 iCountToClaculateRDATimeInterval;
        bool CalculateDuration(bool aInitParsingEnable, uint32 countToClaculateRDATimeInterval);
        int32 AC3SamplingRateToFrameDuration (int32 samplingRate);

};

#endif //AMRFILEPARSER_H_INCLUDED

