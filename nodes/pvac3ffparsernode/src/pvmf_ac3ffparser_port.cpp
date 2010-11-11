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
#include "pvmf_ac3ffparser_port.h"
#include "pvmf_ac3ffparser_node.h"
#include "pvmf_ac3ffparser_defs.h"
#include "pv_mime_string_utils.h"

PVMFAC3FFParserOutPort::PVMFAC3FFParserOutPort(int32 aTag, PVMFNodeInterface* aNode)
        : PvmfPortBaseImpl(aTag, aNode, "Ac3FFParOut(Audio)")
{
    iAC3ParserNode = OSCL_STATIC_CAST(PVMFAC3FFParserNode*, aNode);
    Construct();
    /*	PvmiCapabilityAndConfig::Construct(
    		PVMF_AC3FFPARSER_PORT_OUTPUT_FORMATS,
    		PVMF_AC3FFPARSER_PORT_OUTPUT_FORMATS_VALTYPE);
    		*/
}
void PVMFAC3FFParserOutPort::Construct()
{
    iLogger = PVLogger::GetLoggerObject("PVMFAC3ParserOutPort");
    oscl_memset(&iStats, 0, sizeof(PvmfPortBaseImplStats));
    iNumFramesGenerated = 0;
    iNumFramesConsumed = 0;
}


PVMFAC3FFParserOutPort::~PVMFAC3FFParserOutPort()
{
    Disconnect();
    ClearMsgQueues();
}


bool PVMFAC3FFParserOutPort::IsFormatSupported(PVMFFormatType aFmt)
{
    return (aFmt == PVMF_MIME_AC3);
}

void PVMFAC3FFParserOutPort::FormatUpdated()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO
                    , (0, "PVMFAC3FFParserOutPort::FormatUpdated "));
}

PVMFStatus PVMFAC3FFParserOutPort::Connect(PVMFPortInterface* aPort)
{
    PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::Connect: aPort=0x%x", aPort));

    if (!aPort)
    {
        PVMF_AC3PARSERNODE_LOGERROR((0, "PVMFAC3ParserOutPort::Connect: Error - Connecting to invalid port"));
        return PVMFErrArgument;
    }

    if (iConnectedPort)
    {
        PVMF_AC3PARSERNODE_LOGERROR((0, "PVMFAC3ParserOutPort::Connect: Error - Already connected"));
        return PVMFFailure;
    }

    PvmiCapabilityAndConfig *config;

    PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::Connect: Attempting query interface"));
    aPort->QueryInterface(PVMI_CAPABILITY_AND_CONFIG_PVUUID,
                          (OsclAny*&)config);

    if (config != NULL)
    {
        /*
        PVMF_AC3PARSERNODE_LOGINFO((0,"PVMFAC3ParserOutPort::Connect: Attempting format spec info key %x", config));
        if (!(pvmiSetPortFormatSpecificInfoSync(config, PVMF_FORMAT_SPECIFIC_INFO_KEY)))
        {
            PVMF_AC3PARSERNODE_LOGERROR((0, "PVMFAC3ParserOutPort::Connect: Error - Unable To Send Format Specific Info To Peer"));
            return PVMFFailure;
        }
        */

        PVMF_AC3PARSERNODE_LOGINFO((0,"PVMFAC3ParserOutPort::Connect: Attempting format max num media msgs key "));
        if (!(pvmiSetPortFormatSpecificInfoSync(config, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY)))
        {
            PVMF_AC3PARSERNODE_LOGERROR((0, "PVMFAC3ParserOutPort::Connect: Error - Unable To Send Max Num Media Msg Key To Peer"));
            return PVMFFailure;
        }
    }

    /*
     * Automatically connect the peer.
     */
    PVMF_AC3PARSERNODE_LOGINFO((0,"PVMFAC3ParserOutPort::Connect: Attempting peer connect"));
    if (aPort->PeerConnect(this) != PVMFSuccess)
    {
        PVMF_AC3PARSERNODE_LOGERROR((0, "PVMFAC3ParserOutPort::Connect: Error - Peer Connect failed"));
        return PVMFFailure;
    }

    iConnectedPort = aPort;

    PortActivity(PVMF_PORT_ACTIVITY_CONNECT);
    return PVMFSuccess;
}

PVMFStatus PVMFAC3FFParserOutPort::getParametersSync(PvmiMIOSession aSession,
        PvmiKeyType aIdentifier,
        PvmiKvp*& aParameters,
        int& num_parameter_elements,
        PvmiCapabilityContext aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aIdentifier);
    OSCL_UNUSED_ARG(aContext);
    PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::getParametersSync: aSession=0x%x, aIdentifier=%s, aParameters=0x%x, num_parameters_elements=%d, aContext=0x%x",
                                aSession, aIdentifier, aParameters, num_parameter_elements, aContext));

    num_parameter_elements = 0;
#if 0
    if (pv_mime_strcmp(aIdentifier, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        if (!pvmiGetPortFormatSpecificInfoSync(PVMF_FORMAT_SPECIFIC_INFO_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }

    else if (pv_mime_strcmp(aIdentifier, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY) == 0)
#else
    if (pv_mime_strcmp(aIdentifier, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY) == 0)
#endif
    {
        if (!pvmiGetPortFormatSpecificInfoSync(PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY, aParameters))
        {
            return PVMFFailure;
        }
    }
    num_parameter_elements = 1;
    return PVMFSuccess;
}


PVMFStatus PVMFAC3FFParserOutPort::releaseParameters(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(num_elements);

    PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::releaseParameters: aSession=0x%x, aParameters=0x%x, num_elements=%d",
                                aSession, aParameters, num_elements));

    /*
    if (pv_mime_strcmp(aParameters->key, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        OsclMemAllocator alloc;
        alloc.deallocate((OsclAny*)(aParameters->key));
        return PVMFSuccess;
    }
    */
    return PVMFErrNotSupported;
}

void PVMFAC3FFParserOutPort::setParametersSync(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements,
        PvmiKvp * & aRet_kvp)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aParameters);
    OSCL_UNUSED_ARG(aRet_kvp);
    OSCL_UNUSED_ARG(num_elements);

    /* PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::setParametersSync: aSession=0x%x, aParameters=0x%x, num_elements=%d, aRet_kvp=0x%x",
                                aSession, aParameters, num_elements, aRet_kvp)); */

}

PVMFStatus PVMFAC3FFParserOutPort::verifyParametersSync(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aParameters);
    OSCL_UNUSED_ARG(num_elements);

    PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::verifyParametersSync: aSession=0x%x, aParameters=0x%x, num_elements=%d",
                                aSession, aParameters, num_elements));

    return PVMFErrNotSupported;
}


bool
PVMFAC3FFParserOutPort::pvmiSetPortFormatSpecificInfoSync(PvmiCapabilityAndConfig *aPort,
        const char* aFormatValType)
{
    /*
     * Create PvmiKvp for capability settings
     */
    PVAC3FFNodeTrackPortInfo* trackInfoPtr = NULL;
    if (!(iAC3ParserNode->GetTrackInfo((OSCL_STATIC_CAST(PVMFPortInterface*, this)), trackInfoPtr)))
    {
        PVMF_AC3PARSERNODE_LOGERROR ((0, "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: GetTrackInfo failed"));
        return false;
    }
#if 0
    if (pv_mime_strcmp(aFormatValType, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        PVMF_AC3PARSERNODE_LOGINFO((0, 
                "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: checking for PVMF_FORMAT_SPECIFIC_INFO_KEY, trackInfoPtr = %x",trackInfoPtr));
        if (trackInfoPtr->iFormatSpecificConfig.getMemFragSize() > 0)
        {
            OsclMemAllocator alloc;
            PvmiKvp kvp;
            kvp.key = NULL;
            kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
            if (kvp.key == NULL)
            {
                PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: kvp.key is null"));
                return false;
            }
            oscl_strncpy(kvp.key, aFormatValType, kvp.length);

            kvp.value.key_specific_value = (OsclAny*)(trackInfoPtr->iFormatSpecificConfig.getMemFragPtr());
            kvp.capacity = trackInfoPtr->iFormatSpecificConfig.getMemFragSize();
            PvmiKvp* retKvp = NULL; // for return value
            int32 err;
            PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: attempting setParametersSync ptr=%x", aPort));
            OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
            PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: attempting dealloc kvp.key %x", kvp.key));
            /* ignore the error for now */
            alloc.deallocate((OsclAny*)(kvp.key));
        }
        PVMF_AC3PARSERNODE_LOGINFO((0, "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: returning successfully"));
        return true;
    }
    else if (pv_mime_strcmp(aFormatValType, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY) == 0)
#else
    if (pv_mime_strcmp(aFormatValType, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY) == 0)
#endif
    {
        PVMF_AC3PARSERNODE_LOGINFO((0, 
            "PVMFAC3ParserOutPort::pvmiSetPortFormatSpecificInfoSync: checking for PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY"));
        OsclMemAllocator alloc;
        PvmiKvp kvp;
        kvp.key = NULL;
        kvp.length = oscl_strlen(aFormatValType) + 1; // +1 for \0
        kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
        if (kvp.key == NULL)
        {
            return false;
        }
        oscl_strncpy(kvp.key, aFormatValType, kvp.length);

        kvp.value.uint32_value = PVMF_AC3_PARSER_NODE_MAX_NUM_OUTSTANDING_MEDIA_MSGS;
        PvmiKvp* retKvp = NULL; // for return value
        int32 err;
        OSCL_TRY(err, aPort->setParametersSync(NULL, &kvp, 1, retKvp););
        /* ignore the error for now */
        alloc.deallocate((OsclAny*)(kvp.key));
        return true;
    }
    return false;
}

bool
PVMFAC3FFParserOutPort::pvmiGetPortFormatSpecificInfoSync(const char* aFormatValType,
        PvmiKvp*& aKvp)
{
    /*
     * Create PvmiKvp for capability settings
     */
    PVAC3FFNodeTrackPortInfo* trackInfoPtr = NULL;
    if (!(iAC3ParserNode->GetTrackInfo((OSCL_STATIC_CAST(PVMFPortInterface*, this)), trackInfoPtr)))
    {
        return false;
    }
#if 0
    if (pv_mime_strcmp(aFormatValType, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        if (trackInfoPtr->iFormatSpecificConfig.getMemFragSize() > 0)
        {
            OsclMemAllocator alloc;
            aKvp->key = NULL;
            aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
            aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
            if (aKvp->key == NULL)
            {
                return false;
            }
            oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);

            aKvp->value.key_specific_value = (OsclAny*)(trackInfoPtr->iFormatSpecificConfig.getMemFragPtr());
            aKvp->capacity = trackInfoPtr->iFormatSpecificConfig.getMemFragSize();
        }
        return true;
    }
    else if (pv_mime_strcmp(aFormatValType, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY) == 0)
#else
    if (pv_mime_strcmp(aFormatValType, PVMF_DATAPATH_PORT_MAX_NUM_MEDIA_MSGS_KEY) == 0)
#endif
    {
        OsclMemAllocator alloc;
        aKvp->key = NULL;
        aKvp->length = oscl_strlen(aFormatValType) + 1; // +1 for \0
        aKvp->key = (PvmiKeyType)alloc.ALLOCATE(aKvp->length);
        if (aKvp->key == NULL)
        {
            return false;
        }
        oscl_strncpy(aKvp->key, aFormatValType, aKvp->length);
        aKvp->value.uint32_value = PVMF_AC3_PARSER_NODE_MAX_NUM_OUTSTANDING_MEDIA_MSGS;
        return true;
    }
    return false;
}

