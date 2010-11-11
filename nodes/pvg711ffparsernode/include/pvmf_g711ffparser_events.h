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
#ifndef PVMF_G711FFPARSER_EVENTS_H_INCLUDED
#define PVMF_G711FFPARSER_EVENTS_H_INCLUDED

/**
 UUID for PV G711 FF parser node error and information event type codes
 **/
#define PVMFG711ParserNodeEventTypesUUID PVUuid(0xdeadbeef,0xdead,0xbeef,0x93,0x9e,0x1e,0x00,0xc8,0xed,0xf5,0xf3)
/**
 * An enumeration of error types from PV G711 FF parser node
 **/
typedef enum
{
    /**
     When G711 FF reports error READ_BITRATE_MUTUAL_EXCLUSION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrBitRateMutualExclusionObjectFailed = 1024,

    /**
     When G711 FF reports error READ_BITRATE_RECORD_FAILED
    **/
    PVMFG711FFParserErrBitRateRecordReadFailed,

    /**
     When G711 FF reports error READ_CODEC_ENTRY_FAILED
    **/
    PVMFG711FFParserErrCodecEntryReadFailed,

    /**
     When G711 FF reports error READ_CODEC_LIST_OBJECT_FAILED
    **/
    PVMFG711FFParserErrCodecListObjectReadFailed,

    /**
     When G711 FF reports error READ_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrContentDescriptionObjectReadFailed,

    /**
     When G711 FF reports error READ_CONTENT_DESCRIPTOR_FAILED
    **/
    PVMFG711FFParserErrContentDescriptorReadFailed,

    /**
     When G711 FF reports error READ_DATA_OBJECT_FAILED
    **/
    PVMFG711FFParserErrDataObjectReadFailed,

    /**
     When G711 FF reports error READ_DATA_PACKET_FAILED
    **/
    PVMFG711FFParserErrDataPacketReadFailed,

    /**
     When G711 FF reports error INCORRECT_ERROR_CORRECTION_DATA_TYPE
    **/
    PVMFG711FFParserErrIncorrectErrorCorrectionDataType,

    /**
     When G711 FF reports error OPAQUE_DATA_NOT_SUPPORTED
    **/
    PVMFG711FFParserErrOpaqueDataNotSupported,

    /**
     When G711 FF reports error READ_DATA_PACKET_PAYLOAD_FAILED
    **/
    PVMFG711FFParserErrDataPacketPayloadReadFailed,

    /**
     When G711 FF reports error ZERO_OR_NEGATIVE_SIZE
    **/
    PVMFG711FFParserErrZeroOrNegativeSize,

    /**
     When G711 FF reports error READ_ERROR_CORRECTION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrErrorCorrectionObjectReadFailed,

    /**
     When G711 FF reports error READ_EXTENDED_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrExtendedContentDescriptionObjectReadFailed,

    /**
     When G711 FF reports error READ_FILE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFG711FFParserErrFilePropertiesObjectReadFailed,

    /**
     When G711 FF reports error INVALID_FILE_PROPERTIES_OBJECT_SIZE
    **/
    PVMFG711FFParserErrInvalidFilePropertiesObjectSize,

    /**
     When G711 FF reports error INVALID_DATA_PACKET_COUNT
    **/
    PVMFG711FFParserErrInvalidDataPacketCount,

    /**
     When G711 FF reports error INVALID_PACKET_SIZE
    **/
    PVMFG711FFParserErrInvalidDataPacketSize,

    /**
     When G711 FF reports error READ_HEADER_EXTENSION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrHeaderExtensionObjectReadFailed,

    /**
     When G711 FF reports error RES_VAL_IN_HEADER_EXTENSION_OBJ_INCORRECT
    **/
    PVMFG711FFParserErrReservedValueInHeaderExtensionObjectIncorrect,

    /**
     When G711 FF reports error READ_HEADER_OBJECT_FAILED
    **/
    PVMFG711FFParserErrHeaderObjectReadFailed,

    /**
     When G711 FF reports error MANDATORY_HEADER_OBJECTS_MISSING
    **/
    PVMFG711FFParserErrMandatoryHeaderObjectsMissing,

    /**
     When G711 FF reports error NO_STREAM_OBJECTS_IN_FILE
    **/
    PVMFG711FFParserErrNoStreamObjectsInFile,

    /**
     When G711 FF reports error RES_VALUE_IN_HDR_OBJECT_INCORRECT
    **/
    PVMFG711FFParserErrReservedValueInHeaderObjectIncorrect,

    /**
     When G711 FF reports error DUPLICATE_OBJECTS
    **/
    PVMFG711FFParserErrDuplicateObjects,

    /**
     When G711 FF reports error ZERO_OR_NEGATIVE_OBJECT_SIZE
    **/
    PVMFG711FFParserErrZeroOrNegativeObjectSize,

    /**
     When G711 FF reports error READ_SCRIPT_COMMAND_OBJECT_FAILED
    **/
    PVMFG711FFParserErrScriptCommandObjectReadFailed,

    /**
     When G711 FF reports error READ_PADDING_OBJECT_FAILED
    **/
    PVMFG711FFParserErrPaddingObjectReadFailed,

    /**
     When G711 FF reports error READ_MARKER_FAILED
    **/
    PVMFG711FFParserErrMarkerReadFailed,

    /**
     When G711 FF reports error READ_MARKER_OBJECT_FAILED
    **/
    PVMFG711FFParserErrMarkerObjectReadFailed,

    /**
     When G711 FF reports error READ_STREAM_BITRATE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFG711FFParserErrStreamBitRatePropertiesObjectReadFailed,

    /**
     When G711 FF reports error READ_STREAM_PROPERTIES_OBJECT_FAILED
    **/
    PVMFG711FFParserErrStreamPropertiesObjectReadFailed,

    /**
     When G711 FF reports error INVALID_STREAM_PROPERTIES_OBJECT_SIZE
    **/
    PVMFG711FFParserErrInvalidStreamPropertiesObjectSize,

    /**
     When G711 FF reports error INVALID_STREAM_NUMBER
    **/
    PVMFG711FFParserErrInvalidStreamNumber,

    /**
     When G711 FF reports error READ_SIMPLE_INDEX_OBJECT_FAILED
    **/
    PVMFG711FFParserErrSimpleIndexObjectReadFailed,

    /**
     When G711 FF reports error READ_INDEX_ENTRY_FAILED
    **/
    PVMFG711FFParserErrIndexEntryReadFailed,

    /**
     When G711 FF reports error NO_MEDIA_STREAMS
    **/
    PVMFG711FFParserErrNoMediaStreams,

    /**
     When G711 FF reports error READ_UNKNOWN_OBJECT
    **/
    PVMFG711FFParserErrReadUnknownObject,

    /**
     When G711 FF reports error ASF_FILE_OPEN_FAILED
    **/
    PVMFG711FFParserErrFileOpenFailed,

    /**
     When G711 FF reports error ASF_SAMPLE_INCOMPLETE
    **/
    PVMFG711FFParserErrIncompleteASFSample,

    /**
     When G711 FF reports error PARSE_TYPE_SPECIFIC_DATA_FAILED
    **/
    PVMFG711FFParserErrParseTypeSpecificDataFailed,

    /**
     When G711 FF reports error END_OF_MEDIA_PACKETS
    **/
    PVMFG711FFParserErrEndOfMediaPackets,

    /**
     When G711 FF reports error READ_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrContentEncryptionObjectReadFailed,

    /**
     When G711 FF reports error READ_EXTENDED_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFG711FFParserErrExtendedContentEncryptionObjectReadFailed,

    /**
     When G711 FF reports error READ_INDEX_SPECIFIER_FAILED
    **/
    PVMFG711FFParserErrIndexSpecifierReadFailed,

    /**
     When G711 FF reports error READ_INDEX_BLOCK_FAILED
    **/
    PVMFG711FFParserErrIndexBlockReadFailed,

    /**
     When G711 FF reports error READ_INDEX_OBJECT_FAILED
    **/
    PVMFG711FFParserErrIndexObjectReadFailed,

    PVMFG711FFParserErrUnableToOpenFile,
    PVMFG711FFParserErrUnableToRecognizeFile,
    PVMFG711FFParserErrUnableToCreateASFFileClass,
    PVMFG711FFParserErrTrackMediaMsgAllocatorCreationFailed,
    PVMFG711FFParserErrUnableToPopulateTrackInfoList,
    PVMFG711FFParserErrInitMetaDataFailed,

    /**
     Placeholder for the last PV G711 FF parser error event
     **/
    PVMFG711FFParserErrLast = 8191
} PVMFG711FFParserErrorEventType;

/**
 * An enumeration of informational event types from PV ASF FF parser node
 **/
typedef enum
{
    /**
     Placeholder for the last PV ASF FF parser informational event
     **/
    PVMFG711FFParserInfoLast = 10000

} PVMFG711FFParserInformationalEventType;

#endif // PVMF_ASFFFPARSER_EVENTS_H_INCLUDED


