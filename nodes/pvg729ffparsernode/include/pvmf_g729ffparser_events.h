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
#ifndef PVMF_G729FFPARSER_EVENTS_H_INCLUDED
#define PVMF_G729FFPARSER_EVENTS_H_INCLUDED

/**
 UUID for PV G729 FF parser node error and information event type codes
 **/
#define PVMFG729ParserNodeEventTypesUUID PVUuid(0xdeadbeef,0xdead,0xbeef,0x93,0x9e,0x1e,0x00,0xc8,0xed,0xf5,0xf5)

/**
 * An enumeration of error types from PV G729 FF parser node
 **/
typedef enum
{
    /**
     When G729 FF reports error READ_BITRATE_MUTUAL_EXCLUSION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrBitRateMutualExclusionObjectFailed = 1024,

    /**
     When G729 FF reports error READ_BITRATE_RECORD_FAILED
    **/
    PVMFG729FFParserErrBitRateRecordReadFailed,

    /**
     When G729 FF reports error READ_CODEC_ENTRY_FAILED
    **/
    PVMFG729FFParserErrCodecEntryReadFailed,

    /**
     When G729 FF reports error READ_CODEC_LIST_OBJECT_FAILED
    **/
    PVMFG729FFParserErrCodecListObjectReadFailed,

    /**
     When G729 FF reports error READ_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrContentDescriptionObjectReadFailed,

    /**
     When G729 FF reports error READ_CONTENT_DESCRIPTOR_FAILED
    **/
    PVMFG729FFParserErrContentDescriptorReadFailed,

    /**
     When G729 FF reports error READ_DATA_OBJECT_FAILED
    **/
    PVMFG729FFParserErrDataObjectReadFailed,

    /**
     When G729 FF reports error READ_DATA_PACKET_FAILED
    **/
    PVMFG729FFParserErrDataPacketReadFailed,

    /**
     When G729 FF reports error INCORRECT_ERROR_CORRECTION_DATA_TYPE
    **/
    PVMFG729FFParserErrIncorrectErrorCorrectionDataType,

    /**
     When G729 FF reports error OPAQUE_DATA_NOT_SUPPORTED
    **/
    PVMFG729FFParserErrOpaqueDataNotSupported,

    /**
     When G729 FF reports error READ_DATA_PACKET_PAYLOAD_FAILED
    **/
    PVMFG729FFParserErrDataPacketPayloadReadFailed,

    /**
     When G729 FF reports error ZERO_OR_NEGATIVE_SIZE
    **/
    PVMFG729FFParserErrZeroOrNegativeSize,

    /**
     When G729 FF reports error READ_ERROR_CORRECTION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrErrorCorrectionObjectReadFailed,

    /**
     When G729 FF reports error READ_EXTENDED_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrExtendedContentDescriptionObjectReadFailed,

    /**
     When G729 FF reports error READ_FILE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFG729FFParserErrFilePropertiesObjectReadFailed,

    /**
     When G729 FF reports error INVALID_FILE_PROPERTIES_OBJECT_SIZE
    **/
    PVMFG729FFParserErrInvalidFilePropertiesObjectSize,

    /**
     When G729 FF reports error INVALID_DATA_PACKET_COUNT
    **/
    PVMFG729FFParserErrInvalidDataPacketCount,

    /**
     When G729 FF reports error INVALID_PACKET_SIZE
    **/
    PVMFG729FFParserErrInvalidDataPacketSize,

    /**
     When G729 FF reports error READ_HEADER_EXTENSION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrHeaderExtensionObjectReadFailed,

    /**
     When G729 FF reports error RES_VAL_IN_HEADER_EXTENSION_OBJ_INCORRECT
    **/
    PVMFG729FFParserErrReservedValueInHeaderExtensionObjectIncorrect,

    /**
     When G729 FF reports error READ_HEADER_OBJECT_FAILED
    **/
    PVMFG729FFParserErrHeaderObjectReadFailed,

    /**
     When G729 FF reports error MANDATORY_HEADER_OBJECTS_MISSING
    **/
    PVMFG729FFParserErrMandatoryHeaderObjectsMissing,

    /**
     When G729 FF reports error NO_STREAM_OBJECTS_IN_FILE
    **/
    PVMFG729FFParserErrNoStreamObjectsInFile,

    /**
     When G729 FF reports error RES_VALUE_IN_HDR_OBJECT_INCORRECT
    **/
    PVMFG729FFParserErrReservedValueInHeaderObjectIncorrect,

    /**
     When G729 FF reports error DUPLICATE_OBJECTS
    **/
    PVMFG729FFParserErrDuplicateObjects,

    /**
     When G729 FF reports error ZERO_OR_NEGATIVE_OBJECT_SIZE
    **/
    PVMFG729FFParserErrZeroOrNegativeObjectSize,

    /**
     When G729 FF reports error READ_SCRIPT_COMMAND_OBJECT_FAILED
    **/
    PVMFG729FFParserErrScriptCommandObjectReadFailed,

    /**
     When G729 FF reports error READ_PADDING_OBJECT_FAILED
    **/
    PVMFG729FFParserErrPaddingObjectReadFailed,

    /**
     When G729 FF reports error READ_MARKER_FAILED
    **/
    PVMFG729FFParserErrMarkerReadFailed,

    /**
     When G729 FF reports error READ_MARKER_OBJECT_FAILED
    **/
    PVMFG729FFParserErrMarkerObjectReadFailed,

    /**
     When G729 FF reports error READ_STREAM_BITRATE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFG729FFParserErrStreamBitRatePropertiesObjectReadFailed,

    /**
     When G729 FF reports error READ_STREAM_PROPERTIES_OBJECT_FAILED
    **/
    PVMFG729FFParserErrStreamPropertiesObjectReadFailed,

    /**
     When G729 FF reports error INVALID_STREAM_PROPERTIES_OBJECT_SIZE
    **/
    PVMFG729FFParserErrInvalidStreamPropertiesObjectSize,

    /**
     When G729 FF reports error INVALID_STREAM_NUMBER
    **/
    PVMFG729FFParserErrInvalidStreamNumber,

    /**
     When G729 FF reports error READ_SIMPLE_INDEX_OBJECT_FAILED
    **/
    PVMFG729FFParserErrSimpleIndexObjectReadFailed,

    /**
     When G729 FF reports error READ_INDEX_ENTRY_FAILED
    **/
    PVMFG729FFParserErrIndexEntryReadFailed,

    /**
     When G729 FF reports error NO_MEDIA_STREAMS
    **/
    PVMFG729FFParserErrNoMediaStreams,

    /**
     When G729 FF reports error READ_UNKNOWN_OBJECT
    **/
    PVMFG729FFParserErrReadUnknownObject,

    /**
     When G729 FF reports error ASF_FILE_OPEN_FAILED
    **/
    PVMFG729FFParserErrFileOpenFailed,

    /**
     When G729 FF reports error ASF_SAMPLE_INCOMPLETE
    **/
    PVMFG729FFParserErrIncompleteASFSample,

    /**
     When G729 FF reports error PARSE_TYPE_SPECIFIC_DATA_FAILED
    **/
    PVMFG729FFParserErrParseTypeSpecificDataFailed,

    /**
     When G729 FF reports error END_OF_MEDIA_PACKETS
    **/
    PVMFG729FFParserErrEndOfMediaPackets,

    /**
     When G729 FF reports error READ_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrContentEncryptionObjectReadFailed,

    /**
     When G729 FF reports error READ_EXTENDED_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFG729FFParserErrExtendedContentEncryptionObjectReadFailed,

    /**
     When G729 FF reports error READ_INDEX_SPECIFIER_FAILED
    **/
    PVMFG729FFParserErrIndexSpecifierReadFailed,

    /**
     When G729 FF reports error READ_INDEX_BLOCK_FAILED
    **/
    PVMFG729FFParserErrIndexBlockReadFailed,

    /**
     When G729 FF reports error READ_INDEX_OBJECT_FAILED
    **/
    PVMFG729FFParserErrIndexObjectReadFailed,

    PVMFG729FFParserErrUnableToOpenFile,
    PVMFG729FFParserErrUnableToRecognizeFile,
    PVMFG729FFParserErrUnableToCreateASFFileClass,
    PVMFG729FFParserErrTrackMediaMsgAllocatorCreationFailed,
    PVMFG729FFParserErrUnableToPopulateTrackInfoList,
    PVMFG729FFParserErrInitMetaDataFailed,

    /**
     Placeholder for the last PV G729 FF parser error event
     **/
    PVMFG729FFParserErrLast = 8191
} PVMFG729FFParserErrorEventType;

/**
 * An enumeration of informational event types from PV ASF FF parser node
 **/
typedef enum
{
    /**
     Placeholder for the last PV ASF FF parser informational event
     **/
    PVMFG729FFParserInfoLast = 10000

} PVMFG729FFParserInformationalEventType;

#endif // PVMF_ASFFFPARSER_EVENTS_H_INCLUDED


