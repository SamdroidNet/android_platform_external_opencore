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
#ifndef PVMF_AC3FFPARSER_EVENTS_H_INCLUDED
#define PVMF_AC3FFPARSER_EVENTS_H_INCLUDED

/**
 UUID for PV AC3 FF parser node error and information event type codes
 **/
#define PVMFAC3ParserNodeEventTypesUUID PVUuid(0xdeadbeef,0xdead,0xbeef,0x93,0x9e,0x1e,0x00,0xc8,0xed,0xf5,0xf1)
/**
 * An enumeration of error types from PV AC3 FF parser node
 **/
typedef enum
{
    /**
     When AC3 FF reports error READ_BITRATE_MUTUAL_EXCLUSION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrBitRateMutualExclusionObjectFailed = 1024,

    /**
     When AC3 FF reports error READ_BITRATE_RECORD_FAILED
    **/
    PVMFAC3FFParserErrBitRateRecordReadFailed,

    /**
     When AC3 FF reports error READ_CODEC_ENTRY_FAILED
    **/
    PVMFAC3FFParserErrCodecEntryReadFailed,

    /**
     When AC3 FF reports error READ_CODEC_LIST_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrCodecListObjectReadFailed,

    /**
     When AC3 FF reports error READ_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrContentDescriptionObjectReadFailed,

    /**
     When AC3 FF reports error READ_CONTENT_DESCRIPTOR_FAILED
    **/
    PVMFAC3FFParserErrContentDescriptorReadFailed,

    /**
     When AC3 FF reports error READ_DATA_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrDataObjectReadFailed,

    /**
     When AC3 FF reports error READ_DATA_PACKET_FAILED
    **/
    PVMFAC3FFParserErrDataPacketReadFailed,

    /**
     When AC3 FF reports error INCORRECT_ERROR_CORRECTION_DATA_TYPE
    **/
    PVMFAC3FFParserErrIncorrectErrorCorrectionDataType,

    /**
     When AC3 FF reports error OPAQUE_DATA_NOT_SUPPORTED
    **/
    PVMFAC3FFParserErrOpaqueDataNotSupported,

    /**
     When AC3 FF reports error READ_DATA_PACKET_PAYLOAD_FAILED
    **/
    PVMFAC3FFParserErrDataPacketPayloadReadFailed,

    /**
     When AC3 FF reports error ZERO_OR_NEGATIVE_SIZE
    **/
    PVMFAC3FFParserErrZeroOrNegativeSize,

    /**
     When AC3 FF reports error READ_ERROR_CORRECTION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrErrorCorrectionObjectReadFailed,

    /**
     When AC3 FF reports error READ_EXTENDED_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrExtendedContentDescriptionObjectReadFailed,

    /**
     When AC3 FF reports error READ_FILE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrFilePropertiesObjectReadFailed,

    /**
     When AC3 FF reports error INVALID_FILE_PROPERTIES_OBJECT_SIZE
    **/
    PVMFAC3FFParserErrInvalidFilePropertiesObjectSize,

    /**
     When AC3 FF reports error INVALID_DATA_PACKET_COUNT
    **/
    PVMFAC3FFParserErrInvalidDataPacketCount,

    /**
     When AC3 FF reports error INVALID_PACKET_SIZE
    **/
    PVMFAC3FFParserErrInvalidDataPacketSize,

    /**
     When AC3 FF reports error READ_HEADER_EXTENSION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrHeaderExtensionObjectReadFailed,

    /**
     When AC3 FF reports error RES_VAL_IN_HEADER_EXTENSION_OBJ_INCORRECT
    **/
    PVMFAC3FFParserErrReservedValueInHeaderExtensionObjectIncorrect,

    /**
     When AC3 FF reports error READ_HEADER_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrHeaderObjectReadFailed,

    /**
     When AC3 FF reports error MANDATORY_HEADER_OBJECTS_MISSING
    **/
    PVMFAC3FFParserErrMandatoryHeaderObjectsMissing,

    /**
     When AC3 FF reports error NO_STREAM_OBJECTS_IN_FILE
    **/
    PVMFAC3FFParserErrNoStreamObjectsInFile,

    /**
     When AC3 FF reports error RES_VALUE_IN_HDR_OBJECT_INCORRECT
    **/
    PVMFAC3FFParserErrReservedValueInHeaderObjectIncorrect,

    /**
     When AC3 FF reports error DUPLICATE_OBJECTS
    **/
    PVMFAC3FFParserErrDuplicateObjects,

    /**
     When AC3 FF reports error ZERO_OR_NEGATIVE_OBJECT_SIZE
    **/
    PVMFAC3FFParserErrZeroOrNegativeObjectSize,

    /**
     When AC3 FF reports error READ_SCRIPT_COMMAND_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrScriptCommandObjectReadFailed,

    /**
     When AC3 FF reports error READ_PADDING_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrPaddingObjectReadFailed,

    /**
     When AC3 FF reports error READ_MARKER_FAILED
    **/
    PVMFAC3FFParserErrMarkerReadFailed,

    /**
     When AC3 FF reports error READ_MARKER_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrMarkerObjectReadFailed,

    /**
     When AC3 FF reports error READ_STREAM_BITRATE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrStreamBitRatePropertiesObjectReadFailed,

    /**
     When AC3 FF reports error READ_STREAM_PROPERTIES_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrStreamPropertiesObjectReadFailed,

    /**
     When AC3 FF reports error INVALID_STREAM_PROPERTIES_OBJECT_SIZE
    **/
    PVMFAC3FFParserErrInvalidStreamPropertiesObjectSize,

    /**
     When AC3 FF reports error INVALID_STREAM_NUMBER
    **/
    PVMFAC3FFParserErrInvalidStreamNumber,

    /**
     When AC3 FF reports error READ_SIMPLE_INDEX_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrSimpleIndexObjectReadFailed,

    /**
     When AC3 FF reports error READ_INDEX_ENTRY_FAILED
    **/
    PVMFAC3FFParserErrIndexEntryReadFailed,

    /**
     When AC3 FF reports error NO_MEDIA_STREAMS
    **/
    PVMFAC3FFParserErrNoMediaStreams,

    /**
     When AC3 FF reports error READ_UNKNOWN_OBJECT
    **/
    PVMFAC3FFParserErrReadUnknownObject,

    /**
     When AC3 FF reports error ASF_FILE_OPEN_FAILED
    **/
    PVMFAC3FFParserErrFileOpenFailed,

    /**
     When AC3 FF reports error ASF_SAMPLE_INCOMPLETE
    **/
    PVMFAC3FFParserErrIncompleteASFSample,

    /**
     When AC3 FF reports error PARSE_TYPE_SPECIFIC_DATA_FAILED
    **/
    PVMFAC3FFParserErrParseTypeSpecificDataFailed,

    /**
     When AC3 FF reports error END_OF_MEDIA_PACKETS
    **/
    PVMFAC3FFParserErrEndOfMediaPackets,

    /**
     When AC3 FF reports error READ_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrContentEncryptionObjectReadFailed,

    /**
     When AC3 FF reports error READ_EXTENDED_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrExtendedContentEncryptionObjectReadFailed,

    /**
     When AC3 FF reports error READ_INDEX_SPECIFIER_FAILED
    **/
    PVMFAC3FFParserErrIndexSpecifierReadFailed,

    /**
     When AC3 FF reports error READ_INDEX_BLOCK_FAILED
    **/
    PVMFAC3FFParserErrIndexBlockReadFailed,

    /**
     When AC3 FF reports error READ_INDEX_OBJECT_FAILED
    **/
    PVMFAC3FFParserErrIndexObjectReadFailed,

    PVMFAC3FFParserErrUnableToOpenFile,
    PVMFAC3FFParserErrUnableToRecognizeFile,
    PVMFAC3FFParserErrUnableToCreateASFFileClass,
    PVMFAC3FFParserErrTrackMediaMsgAllocatorCreationFailed,
    PVMFAC3FFParserErrUnableToPopulateTrackInfoList,
    PVMFAC3FFParserErrInitMetaDataFailed,

    /**
     Placeholder for the last PV AC3 FF parser error event
     **/
    PVMFAC3FFParserErrLast = 8191
} PVMFAC3FFParserErrorEventType;

/**
 * An enumeration of informational event types from PV ASF FF parser node
 **/
typedef enum
{
    /**
     Placeholder for the last PV ASF FF parser informational event
     **/
    PVMFAC3FFParserInfoLast = 10000

} PVMFAC3FFParserInformationalEventType;

#endif // PVMF_ASFFFPARSER_EVENTS_H_INCLUDED


