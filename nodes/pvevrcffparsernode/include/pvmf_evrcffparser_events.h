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
#ifndef PVMF_EVRCFFPARSER_EVENTS_H_INCLUDED
#define PVMF_EVRCFFPARSER_EVENTS_H_INCLUDED

/**
 UUID for PV EVRC FF parser node error and information event type codes
 **/
#define PVMFEVRCParserNodeEventTypesUUID PVUuid(0xdeadbeef,0xdead,0xbeef,0x93,0x9e,0x1e,0x00,0xc8,0xed,0xf5,0xf2)
/**
 * An enumeration of error types from PV EVRC FF parser node
 **/
typedef enum
{
    /**
     When EVRC FF reports error READ_BITRATE_MUTUAL_EXCLUSION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrBitRateMutualExclusionObjectFailed = 1024,

    /**
     When EVRC FF reports error READ_BITRATE_RECORD_FAILED
    **/
    PVMFEVRCFFParserErrBitRateRecordReadFailed,

    /**
     When EVRC FF reports error READ_CODEC_ENTRY_FAILED
    **/
    PVMFEVRCFFParserErrCodecEntryReadFailed,

    /**
     When EVRC FF reports error READ_CODEC_LIST_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrCodecListObjectReadFailed,

    /**
     When EVRC FF reports error READ_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrContentDescriptionObjectReadFailed,

    /**
     When EVRC FF reports error READ_CONTENT_DESCRIPTOR_FAILED
    **/
    PVMFEVRCFFParserErrContentDescriptorReadFailed,

    /**
     When EVRC FF reports error READ_DATA_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrDataObjectReadFailed,

    /**
     When EVRC FF reports error READ_DATA_PACKET_FAILED
    **/
    PVMFEVRCFFParserErrDataPacketReadFailed,

    /**
     When EVRC FF reports error INCORRECT_ERROR_CORRECTION_DATA_TYPE
    **/
    PVMFEVRCFFParserErrIncorrectErrorCorrectionDataType,

    /**
     When EVRC FF reports error OPAQUE_DATA_NOT_SUPPORTED
    **/
    PVMFEVRCFFParserErrOpaqueDataNotSupported,

    /**
     When EVRC FF reports error READ_DATA_PACKET_PAYLOAD_FAILED
    **/
    PVMFEVRCFFParserErrDataPacketPayloadReadFailed,

    /**
     When EVRC FF reports error ZERO_OR_NEGATIVE_SIZE
    **/
    PVMFEVRCFFParserErrZeroOrNegativeSize,

    /**
     When EVRC FF reports error READ_ERROR_CORRECTION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrErrorCorrectionObjectReadFailed,

    /**
     When EVRC FF reports error READ_EXTENDED_CONTENT_DESCRIPTION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrExtendedContentDescriptionObjectReadFailed,

    /**
     When EVRC FF reports error READ_FILE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrFilePropertiesObjectReadFailed,

    /**
     When EVRC FF reports error INVALID_FILE_PROPERTIES_OBJECT_SIZE
    **/
    PVMFEVRCFFParserErrInvalidFilePropertiesObjectSize,

    /**
     When EVRC FF reports error INVALID_DATA_PACKET_COUNT
    **/
    PVMFEVRCFFParserErrInvalidDataPacketCount,

    /**
     When EVRC FF reports error INVALID_PACKET_SIZE
    **/
    PVMFEVRCFFParserErrInvalidDataPacketSize,

    /**
     When EVRC FF reports error READ_HEADER_EXTENSION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrHeaderExtensionObjectReadFailed,

    /**
     When EVRC FF reports error RES_VAL_IN_HEADER_EXTENSION_OBJ_INCORRECT
    **/
    PVMFEVRCFFParserErrReservedValueInHeaderExtensionObjectIncorrect,

    /**
     When EVRC FF reports error READ_HEADER_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrHeaderObjectReadFailed,

    /**
     When EVRC FF reports error MANDATORY_HEADER_OBJECTS_MISSING
    **/
    PVMFEVRCFFParserErrMandatoryHeaderObjectsMissing,

    /**
     When EVRC FF reports error NO_STREAM_OBJECTS_IN_FILE
    **/
    PVMFEVRCFFParserErrNoStreamObjectsInFile,

    /**
     When EVRC FF reports error RES_VALUE_IN_HDR_OBJECT_INCORRECT
    **/
    PVMFEVRCFFParserErrReservedValueInHeaderObjectIncorrect,

    /**
     When EVRC FF reports error DUPLICATE_OBJECTS
    **/
    PVMFEVRCFFParserErrDuplicateObjects,

    /**
     When EVRC FF reports error ZERO_OR_NEGATIVE_OBJECT_SIZE
    **/
    PVMFEVRCFFParserErrZeroOrNegativeObjectSize,

    /**
     When EVRC FF reports error READ_SCRIPT_COMMAND_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrScriptCommandObjectReadFailed,

    /**
     When EVRC FF reports error READ_PADDING_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrPaddingObjectReadFailed,

    /**
     When EVRC FF reports error READ_MARKER_FAILED
    **/
    PVMFEVRCFFParserErrMarkerReadFailed,

    /**
     When EVRC FF reports error READ_MARKER_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrMarkerObjectReadFailed,

    /**
     When EVRC FF reports error READ_STREAM_BITRATE_PROPERTIES_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrStreamBitRatePropertiesObjectReadFailed,

    /**
     When EVRC FF reports error READ_STREAM_PROPERTIES_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrStreamPropertiesObjectReadFailed,

    /**
     When EVRC FF reports error INVALID_STREAM_PROPERTIES_OBJECT_SIZE
    **/
    PVMFEVRCFFParserErrInvalidStreamPropertiesObjectSize,

    /**
     When EVRC FF reports error INVALID_STREAM_NUMBER
    **/
    PVMFEVRCFFParserErrInvalidStreamNumber,

    /**
     When EVRC FF reports error READ_SIMPLE_INDEX_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrSimpleIndexObjectReadFailed,

    /**
     When EVRC FF reports error READ_INDEX_ENTRY_FAILED
    **/
    PVMFEVRCFFParserErrIndexEntryReadFailed,

    /**
     When EVRC FF reports error NO_MEDIA_STREAMS
    **/
    PVMFEVRCFFParserErrNoMediaStreams,

    /**
     When EVRC FF reports error READ_UNKNOWN_OBJECT
    **/
    PVMFEVRCFFParserErrReadUnknownObject,

    /**
     When EVRC FF reports error ASF_FILE_OPEN_FAILED
    **/
    PVMFEVRCFFParserErrFileOpenFailed,

    /**
     When EVRC FF reports error ASF_SAMPLE_INCOMPLETE
    **/
    PVMFEVRCFFParserErrIncompleteASFSample,

    /**
     When EVRC FF reports error PARSE_TYPE_SPECIFIC_DATA_FAILED
    **/
    PVMFEVRCFFParserErrParseTypeSpecificDataFailed,

    /**
     When EVRC FF reports error END_OF_MEDIA_PACKETS
    **/
    PVMFEVRCFFParserErrEndOfMediaPackets,

    /**
     When EVRC FF reports error READ_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrContentEncryptionObjectReadFailed,

    /**
     When EVRC FF reports error READ_EXTENDED_CONTENT_ENCRYPTION_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrExtendedContentEncryptionObjectReadFailed,

    /**
     When EVRC FF reports error READ_INDEX_SPECIFIER_FAILED
    **/
    PVMFEVRCFFParserErrIndexSpecifierReadFailed,

    /**
     When EVRC FF reports error READ_INDEX_BLOCK_FAILED
    **/
    PVMFEVRCFFParserErrIndexBlockReadFailed,

    /**
     When EVRC FF reports error READ_INDEX_OBJECT_FAILED
    **/
    PVMFEVRCFFParserErrIndexObjectReadFailed,

    PVMFEVRCFFParserErrUnableToOpenFile,
    PVMFEVRCFFParserErrUnableToRecognizeFile,
    PVMFEVRCFFParserErrUnableToCreateASFFileClass,
    PVMFEVRCFFParserErrTrackMediaMsgAllocatorCreationFailed,
    PVMFEVRCFFParserErrUnableToPopulateTrackInfoList,
    PVMFEVRCFFParserErrInitMetaDataFailed,

    /**
     Placeholder for the last PV EVRC FF parser error event
     **/
    PVMFEVRCFFParserErrLast = 8191
} PVMFEVRCFFParserErrorEventType;

/**
 * An enumeration of informational event types from PV ASF FF parser node
 **/
typedef enum
{
    /**
     Placeholder for the last PV ASF FF parser informational event
     **/
    PVMFEVRCFFParserInfoLast = 10000

} PVMFEVRCFFParserInformationalEventType;

#endif // PVMF_ASFFFPARSER_EVENTS_H_INCLUDED


