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
#ifndef PVMF_G711FFPARSER_NODE_H_INCLUDED
#define PVMF_G711FFPARSER_NODE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif
#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif
#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif
#ifndef PVMF_MEDIA_DATA_H_INCLUDED
#include "pvmf_media_data.h"
#endif
#ifndef OSCL_MEM_MEMPOOL_H_INCLUDED
#include "oscl_mem_mempool.h"
#endif
#ifndef PVMF_MEMPOOL_H_INCLUDED
#include "pvmf_mempool.h"
#endif
#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#include "pvmf_simple_media_buffer.h"
#endif
#ifndef PVMF_DATA_SOURCE_INIT_EXTENSION_H_INCLUDED
#include "pvmf_data_source_init_extension.h"
#endif
#ifndef PVMF_TRACK_SELECTION_EXTENSION_H_INCLUDED
#include "pvmf_track_selection_extension.h"
#endif
#ifndef PVMF_MEDIA_PRESENTATION_INFO_H_INCLUDED
#include "pvmf_media_presentation_info.h"
#endif
#ifndef PVMF_NODE_UTILS_H_INCLUDED
#include "pvmf_node_utils.h"
#endif
#ifndef G711FILEPARSER_H_INCLUDED
#include "g711fileparser.h"
#endif
#ifndef PVMF_META_DATA_EXTENSION_H_INCLUDED
#include "pvmf_meta_data_extension.h"
#endif
#ifndef PVMF_DATA_SOURCE_PLAYBACK_CONTROL_H_INCLUDED
#include "pvmf_data_source_playback_control.h"
#endif
#ifndef PVMF_LOCAL_DATA_SOURCE_H_INCLUDED
#include "pvmf_local_data_source.h"
#endif

#ifndef PVMF_FORMAT_PROGDOWNLOAD_SUPPORT_EXTENSION_H_INCLUDED
#include "pvmf_format_progdownload_support_extension.h"
#endif

#ifndef PVMF_DOWNLOAD_PROGRESS_EXTENSION_H
#include "pvmf_download_progress_interface.h"
#endif

#ifndef PVMI_DATASTREAMUSER_INTERFACE_H_INCLUDED
#include "pvmi_datastreamuser_interface.h"
#endif

#ifndef PVMF_G711FFPARSER_PORT_H_INCLUDED
#include "pvmf_g711ffparser_port.h"
#endif

#ifndef PVMF_CPMPLUGIN_LICENSE_INTERFACE_H_INCLUDED
#include "pvmf_cpmplugin_license_interface.h"
#endif

#define PVMF_G711_PARSER_NODE_NEW(auditCB,T,params,ptr)\
{\
	ptr = OSCL_NEW(T,params);\
}

#define PVMF_G711_PARSER_NODE_DELETE(auditCB,T,ptr)\
{\
	OSCL_DELETE(ptr);\
}

#define PVMF_G711_PARSER_NODE_TEMPLATED_DELETE(auditCB, T, Tsimple, ptr)\
{\
	OSCL_DELETE(ptr);\
}

#define PV_G711_PARSER_NODE_ARRAY_NEW(auditCB, T, count, ptr)\
{\
	ptr = OSCL_ARRAY_NEW(T, count);\
}

#define PV_G711_PARSER_NODE_ARRAY_DELETE(auditCB, ptr)\
{\
	OSCL_ARRAY_DELETE(ptr);\
}


typedef PVMFGenericNodeCommand<OsclMemAllocator> PVMFG711FFNodeCommandBase;

/* Default vector reserve size */
#define PVMF_G711_PARSER_NODE_VECTOR_RESERVE 10
/* Starting value for command IDs  */
#define PVMF_G711_PARSER_NODE_COMMAND_ID_START 9000

enum TPVMFG711ParserNodeCommand
{
    PVMF_G711_PARSER_NODE_QUERYUUID,
    PVMF_G711_PARSER_NODE_QUERYINTERFACE,
    PVMF_G711_PARSER_NODE_REQUESTPORT,
    PVMF_G711_PARSER_NODE_RELEASEPORT,
    PVMF_G711_PARSER_NODE_INIT,
    PVMF_G711_PARSER_NODE_PREPARE,
    PVMF_G711_PARSER_NODE_START,
    PVMF_G711_PARSER_NODE_STOP,
    PVMF_G711_PARSER_NODE_FLUSH,
    PVMF_G711_PARSER_NODE_PAUSE,
    PVMF_G711_PARSER_NODE_RESET,
    PVMF_G711_PARSER_NODE_CANCELALLCOMMANDS,
    PVMF_G711_PARSER_NODE_CANCELCOMMAND,
    PVMF_G711_PARSER_NODE_SET_DATASOURCE_POSITION,
    PVMF_G711_PARSER_NODE_QUERY_DATASOURCE_POSITION,
    PVMF_G711_PARSER_NODE_SET_DATASOURCE_RATE,
    PVMF_G711_PARSER_NODE_GETNODEMETADATAKEYS,
    PVMF_G711_PARSER_NODE_GETNODEMETADATAVALUES,
    PVMF_G711_PARSER_NODE_SET_DATASOURCE_DIRECTION,
    PVMF_G711_PARSER_NODE_GET_LICENSE_W,
    PVMF_G711_PARSER_NODE_GET_LICENSE,
    PVMF_G711_PARSER_NODE_CMD_CANCEL_GET_LICENSE,
    PVMF_G711_PARSER_NODE_COMMAND_LAST
};

#define PVMF_G711_PARSER_NODE_MAX_CPM_METADATA_KEYS 256

class PVMFG711FFNodeCommand : public PVMFG711FFNodeCommandBase
{
    public:

        // Constructor and parser for GetNodeMetadataKeys
        void Construct(PVMFSessionId s, int32 cmd
                       , PVMFMetadataList& aKeyList
                       , uint32 aStartingIndex
                       , int32 aMaxEntries
                       , char* aQueryKey
                       , const OsclAny* aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aKeyList;
            iParam2 = (OsclAny*)aStartingIndex;
            iParam3 = (OsclAny*)aMaxEntries;
            iParam4 = (OsclAny*)aQueryKey;
        }
        void Parse(PVMFMetadataList*& MetaDataListPtr, uint32 &aStartingIndex, int32 &aMaxEntries, char*& aQueryKey)
        {
            MetaDataListPtr = (PVMFMetadataList*)iParam1;
            aStartingIndex = (uint32)iParam2;
            aMaxEntries = (int32)iParam3;
            aQueryKey = (char*)iParam4;
        }

        // Constructor and parser for GetNodeMetadataValue
        void Construct(PVMFSessionId s, int32 cmd, PVMFMetadataList& aKeyList, Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartIndex, int32 aMaxEntries, const OsclAny* aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aKeyList;
            iParam2 = (OsclAny*) & aValueList;
            iParam3 = (OsclAny*)aStartIndex;
            iParam4 = (OsclAny*)aMaxEntries;

        }
        void Parse(PVMFMetadataList* &aKeyList, Oscl_Vector<PvmiKvp, OsclMemAllocator>* &aValueList, uint32 &aStartingIndex, int32 &aMaxEntries)
        {
            aKeyList = (PVMFMetadataList*)iParam1;
            aValueList = (Oscl_Vector<PvmiKvp, OsclMemAllocator>*)iParam2;
            aStartingIndex = (uint32)iParam3;
            aMaxEntries = (int32)iParam4;
        }

        // Constructor and parser for SetDataSourcePosition
        void Construct(PVMFSessionId s, int32 cmd, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT, PVMFTimestamp& aActualMediaDataTS,
                       bool aSeekToSyncPoint, uint32 aStreamID, const OsclAny*aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aTargetNPT;
            iParam2 = (OsclAny*) & aActualNPT;
            iParam3 = (OsclAny*) & aActualMediaDataTS;
            iParam4 = (OsclAny*)aSeekToSyncPoint;
            iParam5 = (OsclAny*)aStreamID;
        }
        void Parse(PVMFTimestamp& aTargetNPT, PVMFTimestamp*& aActualNPT, PVMFTimestamp*& aActualMediaDataTS, bool& aSeekToSyncPoint, uint32& aStreamID)
        {
            aTargetNPT = (PVMFTimestamp)iParam1;
            aActualNPT = (PVMFTimestamp*)iParam2;
            aActualMediaDataTS = (PVMFTimestamp*)iParam3;
            aSeekToSyncPoint = (iParam4) ? true : false;
            aStreamID = (uint32)iParam5;
        }

        // Constructor and parser for QueryDataSourcePosition
        void Construct(PVMFSessionId s, int32 cmd, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT,	bool aSeekToSyncPoint, const OsclAny*aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aTargetNPT;
            iParam2 = (OsclAny*) & aActualNPT;
            iParam3 = (OsclAny*)aSeekToSyncPoint;
            iParam4 = NULL;
            iParam5 = NULL;
        }
        void Parse(PVMFTimestamp& aTargetNPT, PVMFTimestamp*& aActualNPT, bool& aSeekToSyncPoint)
        {
            aTargetNPT = (PVMFTimestamp)iParam1;
            aActualNPT = (PVMFTimestamp*)iParam2;
            aSeekToSyncPoint = (iParam3) ? true : false;
        }

        // Constructor and parser for SetDataSourceRate
        void Construct(PVMFSessionId s, int32 cmd, int32 aRate, PVMFTimebase* aTimebase, const OsclAny*aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*)aRate;
            iParam2 = (OsclAny*)aTimebase;
            iParam3 = NULL;
            iParam4 = NULL;
            iParam5 = NULL;
        }
        void Parse(int32& aRate, PVMFTimebase*& aTimebase)
        {
            aRate = (int32)iParam1;
            aTimebase = (PVMFTimebase*)iParam2;
        }

        /* need to overlaod the base Destroy routine to cleanup metadata key. */
        void Destroy()
        {
            PVMFGenericNodeCommand<PVMFG711ParserNodeAllocator>::Destroy();
            switch (iCmd)
            {
                case PVMF_G711_PARSER_NODE_GETNODEMETADATAKEYS:
                    if (iParam4)
                    {
                        /* cleanup the allocated string */
                        Oscl_TAlloc<OSCL_HeapString<PVMFG711ParserNodeAllocator>, PVMFG711ParserNodeAllocator> str;
                        str.destruct_and_dealloc(iParam4);
                    }
                    break;
                default:
                    break;
            }
        }
        /* need to overlaod the base Copy routine to copy metadata key. */
        void Copy(const PVMFGenericNodeCommand<PVMFG711ParserNodeAllocator>& aCmd)
        {
            PVMFGenericNodeCommand<PVMFG711ParserNodeAllocator>::Copy(aCmd);
            switch (aCmd.iCmd)
            {
                case PVMF_G711_PARSER_NODE_GETNODEMETADATAKEYS:
                    if (aCmd.iParam4)
                    {
                        /* copy the allocated string */
                        OSCL_HeapString<PVMFG711ParserNodeAllocator>* aStr =
                            (OSCL_HeapString<PVMFG711ParserNodeAllocator>*)aCmd.iParam4;
                        Oscl_TAlloc<OSCL_HeapString<PVMFG711ParserNodeAllocator>, PVMFG711ParserNodeAllocator> str;
                        iParam4 = str.alloc_and_construct(*aStr);
                    }
                    break;
                default:
                    break;
            }
        }

        /* Constructor and parser for GetLicenseW */
        void Construct(PVMFSessionId s,
                       int32 cmd,
                       OSCL_wString& aContentName,
                       OsclAny* aLicenseData,
                       uint32 aDataSize,
                       int32 aTimeoutMsec,
                       OsclAny* aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aContentName;
            iParam2 = (OsclAny*)aLicenseData;
            iParam3 = (OsclAny*)aDataSize;
            iParam4 = (OsclAny*)aTimeoutMsec;
            iParam5 = NULL;
        }
        void Parse(OSCL_wString*& aContentName,
                   OsclAny*& aLicenseData,
                   uint32& aDataSize,
                   int32& aTimeoutMsec)
        {
            aContentName = (OSCL_wString*)iParam1;
            aLicenseData = (PVMFTimestamp*)iParam2;
            aDataSize = (uint32)iParam3;
            aTimeoutMsec = (int32)iParam4;
        }

        /* Constructor and parser for GetLicense */
        void Construct(PVMFSessionId s,
                       int32 cmd,
                       OSCL_String& aContentName,
                       OsclAny* aLicenseData,
                       uint32 aDataSize,
                       int32 aTimeoutMsec,
                       OsclAny* aContext)
        {
            PVMFG711FFNodeCommandBase::Construct(s, cmd, aContext);
            iParam1 = (OsclAny*) & aContentName;
            iParam2 = (OsclAny*)aLicenseData;
            iParam3 = (OsclAny*)aDataSize;
            iParam4 = (OsclAny*)aTimeoutMsec;
            iParam5 = NULL;
        }
        void Parse(OSCL_String*& aContentName,
                   OsclAny*& aLicenseData,
                   uint32& aDataSize,
                   int32& aTimeoutMsec)
        {
            aContentName = (OSCL_String*)iParam1;
            aLicenseData = (PVMFTimestamp*)iParam2;
            aDataSize = (uint32)iParam3;
            aTimeoutMsec = (int32)iParam4;
        }
};


typedef PVMFNodeCommandQueue<PVMFG711FFNodeCommand, OsclMemAllocator> PVMFG711FFNodeCmdQ;


/**
* Track/Port information
*/
class MediaClockConverter;
class PVMFG711FFParserNode;


/**
* Container for the CPM object
*/

#ifndef CPM_H_INCLUDED
#include "cpm.h"
#endif

//memory allocator type for this node.
typedef OsclMemAllocator PVMFG711FFNodeAllocator;

/**
* The node class
*/
//typedef PVMFNodeCommandQueue<PVMFG711FFNodeCommand, PVMFG711ParserNodeAllocator> PVMFG711ParserNodeCmdQueue;
class CG711FileParser;
class PVMFG711FFParserOutPort;
class PVLogger;

class PVMFG711FFParserNode :  public OsclTimerObject
            , public PVMFNodeInterface
            , public PVMFDataSourceInitializationExtensionInterface
            , public PVMFTrackSelectionExtensionInterface
            , public PvmfDataSourcePlaybackControlInterface
            , public PVMFMetadataExtensionInterface
            , public PVMFCPMStatusObserver
            , public PVMIDatastreamuserInterface
            , public PvmiDataStreamObserver
            , public PVMFFormatProgDownloadSupportInterface
            , public PVMFCPMPluginLicenseInterface
{
    public:
        PVMFG711FFParserNode(int32 aPriority = OsclActiveObject::EPriorityNominal);
        ~PVMFG711FFParserNode();

        // From PVMFNodeInterface
        PVMFStatus ThreadLogon();
        PVMFStatus ThreadLogoff();
        PVMFStatus GetCapability(PVMFNodeCapability& aNodeCapability);
        PVMFPortIter* GetPorts(const PVMFPortFilter* aFilter = NULL);
        PVMFCommandId QueryUUID(PVMFSessionId, const PvmfMimeString& aMimeType,
                                Oscl_Vector<PVUuid, PVMFG711FFNodeAllocator>& aUuids,
                                bool aExactUuidsOnly = false,
                                const OsclAny* aContext = NULL);

        PVMFCommandId QueryInterface(PVMFSessionId, const PVUuid& aUuid,
                                     PVInterface*& aInterfacePtr,
                                     const OsclAny* aContext = NULL);

        virtual PVMFCommandId RequestPort(PVMFSessionId aSession,
                                          int32 aPortTag,
                                          const PvmfMimeString* aPortConfig = NULL,
                                          const OsclAny* aContext = NULL);

        PVMFCommandId ReleasePort(PVMFSessionId, PVMFPortInterface& aPort, const OsclAny* aContext = NULL);
        PVMFCommandId Init(PVMFSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Start(PVMFSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Stop(PVMFSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Flush(PVMFSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Pause(PVMFSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Reset(PVMFSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Prepare(PVMFSessionId aSession, const OsclAny* aContext = NULL);
        PVMFCommandId CancelAllCommands(PVMFSessionId, const OsclAny* aContextData = NULL);
        PVMFCommandId CancelCommand(PVMFSessionId, PVMFCommandId aCmdId, const OsclAny* aContextData = NULL);
        PVMFStatus QueryInterfaceSync(PVMFSessionId aSession,
                                      const PVUuid& aUuid,
                                      PVInterface*& aInterfacePtr);

        /* From PVInterface */
        void addRef();
        void removeRef();
        bool queryInterface(const PVUuid& uuid, PVInterface *& iface);
        //From PVMFDataSourceInitializationExtensionInterface
        PVMFStatus SetSourceInitializationData(OSCL_wString& aSourceURL, PVMFFormatType& aSourceFormat, OsclAny* aSourceData);
        PVMFStatus SetClientPlayBackClock(PVMFMediaClock* aClientClock);
        PVMFStatus SetEstimatedServerClock(PVMFMediaClock* aClientClock);

        //From PVMFTrackSelectionExtensionInterface
        PVMFStatus GetMediaPresentationInfo(PVMFMediaPresentationInfo& aInfo);
        PVMFStatus SelectTracks(PVMFMediaPresentationInfo& aInfo);

        // From PVMFMetadataExtensionInterface
        uint32 GetNumMetadataKeys(char* aQueryKeyString = NULL);
        uint32 GetNumMetadataValues(PVMFMetadataList& aKeyList);
        PVMFCommandId GetNodeMetadataKeys(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList
                                          , uint32 aStartingKeyIndex, int32 aMaxKeyEntries, char* aQueryKeyString = NULL, const OsclAny* aContextData = NULL);
        PVMFCommandId GetNodeMetadataValues(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList
                                            , Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartingValueIndex, int32 aMaxValueEntries, const OsclAny* aContextData = NULL);
        PVMFStatus ReleaseNodeMetadataKeys(PVMFMetadataList& aKeyList, uint32 aStartingKeyIndex, uint32 aEndKeyIndex);
        PVMFStatus ReleaseNodeMetadataValues(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartingValueIndex, uint32 aEndValueIndex);

        // From PvmfDataSourcePlaybackControlInterface
        PVMFCommandId SetDataSourcePosition(PVMFSessionId aSessionId
                                            , PVMFTimestamp aTargetNPT
                                            , PVMFTimestamp& aActualNPT
                                            , PVMFTimestamp& aActualMediaDataTS
                                            , bool aSeekToSyncPoint = true
                                                                      , uint32 aStreamID = 0
                                                                                           , OsclAny* aContext = NULL);

        PVMFCommandId QueryDataSourcePosition(PVMFSessionId aSessionId
                                              , PVMFTimestamp aTargetNPT
                                              , PVMFTimestamp& aActualNPT
                                              , bool aSeekToSyncPoint = true
                                                                        , OsclAny* aContext = NULL);

        PVMFCommandId QueryDataSourcePosition(PVMFSessionId aSessionId
                                              , PVMFTimestamp aTargetNPT
                                              , PVMFTimestamp& aSeekPointBeforeTargetNPT
                                              , PVMFTimestamp& aSeekPointAfterTargetNPT
                                              , OsclAny* aContext = NULL
                                                                    , bool aSeekToSyncPoint = true);

        PVMFCommandId SetDataSourceRate(PVMFSessionId aSession
                                        , int32 aRate
                                        , PVMFTimebase* aTimebase = NULL
                                                                    , OsclAny* aContext = NULL);

        /* From PVMIDatastreamuserInterface */
        void PassDatastreamFactory(PVMFDataStreamFactory& aFactory,
                                   int32 aFactoryTag,
                                   const PvmfMimeString* aFactoryConfig = NULL);

        void PassDatastreamReadCapacityObserver(PVMFDataStreamReadCapacityObserver* aObserver);

        /* From PVMFFormatProgDownloadSupportInterface */
        int32 convertSizeToTime(uint32 fileSize, uint32& aNPTInMS);
        void setFileSize(const uint32 aFileSize);
        void setDownloadProgressInterface(PVMFDownloadProgressInterface*);
        void playResumeNotification(bool aDownloadComplete);
        void notifyDownloadComplete()
        {
            playResumeNotification(true);
        };

        /* From PvmiDataStreamObserver */
        void DataStreamCommandCompleted(const PVMFCmdResp& aResponse);
        void DataStreamInformationalEvent(const PVMFAsyncEvent& aEvent);
        void DataStreamErrorEvent(const PVMFAsyncEvent& aEvent);

        //from PVMFPortActivityHandler
        void HandlePortActivity(const PVMFPortActivity& aActivity);

        /* From PVMFCPMPluginLicenseInterface */
        PVMFStatus GetLicenseURL(PVMFSessionId aSessionId,
                                 OSCL_wString& aContentName,
                                 OSCL_wString& aLicenseURL)
        {
            OSCL_UNUSED_ARG(aSessionId);
            OSCL_UNUSED_ARG(aContentName);
            OSCL_UNUSED_ARG(aLicenseURL);
            //must use Async method.
            return PVMFErrNotSupported;
        }
        PVMFStatus GetLicenseURL(PVMFSessionId aSessionId,
                                 OSCL_String&  aContentName,
                                 OSCL_String&  aLicenseURL)
        {
            OSCL_UNUSED_ARG(aSessionId);
            OSCL_UNUSED_ARG(aContentName);
            OSCL_UNUSED_ARG(aLicenseURL);
            //must use Async method.
            return PVMFErrNotSupported;
        }

        PVMFCommandId GetLicense(PVMFSessionId aSessionId,
                                 OSCL_wString& aContentName,
                                 OsclAny* aData,
                                 uint32 aDataSize,
                                 int32 aTimeoutMsec,
                                 OsclAny* aContextData) ;

        PVMFCommandId GetLicense(PVMFSessionId aSessionId,
                                 OSCL_String&  aContentName,
                                 OsclAny* aData,
                                 uint32 aDataSize,
                                 int32 aTimeoutMsec,
                                 OsclAny* aContextData);

        PVMFCommandId CancelGetLicense(PVMFSessionId aSessionId
                                       , PVMFCommandId aCmdId
                                       , OsclAny* aContextData);

    private:
        void Construct();
        void Run();
        //Command processing
        PVMFCommandId QueueCommandL(PVMFG711FFNodeCommand& aCmd);

        void MoveCmdToCurrentQueue(PVMFG711FFNodeCommand& aCmd);
        void MoveCmdToCancelQueue(PVMFG711FFNodeCommand& aCmd);
        bool ProcessCommand();
        void CommandComplete(PVMFG711FFNodeCmdQ& aCmdQueue,
                             PVMFG711FFNodeCommand& aCmd,
                             PVMFStatus aStatus,
                             OsclAny* aData = NULL,
                             PVUuid* aEventUUID = NULL,
                             int32* aEventCode = NULL,
                             PVInterface* aExtMsg = NULL);

        void CommandComplete(PVMFG711FFNodeCmdQ&, PVMFG711FFNodeCommand&, PVMFStatus, PVInterface*, OsclAny* aData);
        bool FlushPending();
        //Command handlers.
        void DoQueryUuid(PVMFG711FFNodeCommand&);
        void DoQueryInterface(PVMFG711FFNodeCommand&);

        PVMFStatus DoInit(PVMFG711FFNodeCommand&);
        void CompleteInit();

        void DoPrepare(PVMFG711FFNodeCommand&);
        void DoStart(PVMFG711FFNodeCommand&);
        void DoStop(PVMFG711FFNodeCommand&);
        void DoPause(PVMFG711FFNodeCommand&);
        void DoFlush(PVMFG711FFNodeCommand&);

        void DoCancelAllCommands(PVMFG711FFNodeCommand&);
        void DoCancelCommand(PVMFG711FFNodeCommand&);

        void DoRequestPort(PVMFG711FFNodeCommand&, PVMFPortInterface*&);
        void DoReleasePort(PVMFG711FFNodeCommand&);

        void DoReset(PVMFG711FFNodeCommand&);
        void  CompleteReset();

        // For metadata extention interface
        PVMFStatus DoGetMetadataKeys(PVMFG711FFNodeCommand& aCmd);
        PVMFStatus DoGetMetadataValues(PVMFG711FFNodeCommand& aCmd);
        PVMFStatus InitMetaData();


        void CompleteGetMetaDataValues();
        int32 AddToValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, PvmiKvp& aNewValue);
        PVMFStatus CompleteGetMetadataKeys(PVMFG711FFNodeCommand& aCmd);

        void PushToAvailableMetadataKeysList(const char* aKeystr, char* aOptionalParam = NULL);
        Oscl_Vector<OSCL_HeapString<PVMFG711ParserNodeAllocator>, PVMFG711ParserNodeAllocator> iAvailableMetadataKeys;
        Oscl_Vector<OSCL_HeapString<PVMFG711ParserNodeAllocator>, PVMFG711ParserNodeAllocator> iCPMMetadataKeys;
        uint32 iG711ParserNodeMetadataValueCount;

        // For data source position extension interface
        PVMFStatus DoSetDataSourcePosition(PVMFG711FFNodeCommand& aCmd);
        PVMFStatus DoQueryDataSourcePosition(PVMFG711FFNodeCommand& aCmd);
        PVMFStatus DoSetDataSourceRate(PVMFG711FFNodeCommand& aCmd);

        /* For data source direction extension interface */
        void DoSetDataSourceDirection(PVMFG711FFNodeCommand& aCmd);
        int32 iPlayBackDirection;

        // Event reporting
        void ReportErrorEvent(PVMFEventType aEventType, OsclAny* aEventData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        void ReportInfoEvent(PVMFEventType aEventType, OsclAny* aEventData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        void SetState(TPVMFNodeInterfaceState);

        // Track data processing

        bool RetrieveTrackData(PVG711FFNodeTrackPortInfo& aTrackPortInfo);
        PVMFStatus RetrieveTrackData(PVG711FFNodeTrackPortInfo& aTrackPortInfo, PVMFSharedMediaDataPtr& aMediaDataOut);
        bool SendEndOfTrackCommand(PVG711FFNodeTrackPortInfo& aTrackPortInfo);
        PVMFStatus SendBeginOfMediaStreamCommand(PVG711FFNodeTrackPortInfo* aTrackPortInfo);
        bool CheckAvailabilityForSendingNewTrackData(PVG711FFNodeTrackPortInfo& aTrackPortInfo);
        void ResetAllTracks();
        bool ReleaseAllPorts();
        void CleanupFileSource();

    private: // private member variables

        PVMFG711FFNodeCmdQ iInputCommands;
        PVMFG711FFNodeCmdQ iCurrentCommand;
        PVMFG711FFNodeCmdQ iCancelCommand;

        PVMFG711FFParserOutPort* iOutPort;
        friend class PVMFG711FFParserOutPort;
        void ProcessOutgoingMsg();

        /* Port processing */
        PVMFPortVector<PVMFG711FFParserOutPort, PVMFG711ParserNodeAllocator> iPortVector;
        bool ProcessPortActivity(PVG711FFNodeTrackPortInfo*);
        PVMFStatus RetrieveMediaSample(PVG711FFNodeTrackPortInfo* aTrackInfoPtr,
                                       PVMFSharedMediaDataPtr& aSharedPtr);
        PVMFStatus QueueMediaSample(PVG711FFNodeTrackPortInfo* aTrackInfoPtr);
        PVMFStatus ProcessOutgoingMsg(PVG711FFNodeTrackPortInfo* aTrackInfoPtr);
        bool GetTrackInfo(PVMFPortInterface* aPort,
                          PVG711FFNodeTrackPortInfo*& aTrackInfoPtr);
        bool GetTrackInfo(int32 aTrackID,
                          PVG711FFNodeTrackPortInfo*& aTrackInfoPtr);
        PVMFStatus GenerateAndSendEOSCommand(PVG711FFNodeTrackPortInfo* aTrackInfoPtr);
        bool CheckForPortRescheduling();
        bool CheckForPortActivityQueues();
        int32 PushBackKeyVal(Oscl_Vector<PvmiKvp, OsclMemAllocator>*& aValueListPtr, PvmiKvp &aKeyVal);
        PVMFStatus PushValueToList(Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> &aRefMetadataKeys,
                                   PVMFMetadataList *&aKeyListPtr,
                                   uint32 aLcv);

        /* Progressive download related */
        PVMFStatus CheckForG711HeaderAvailability();
        void PauseAllMediaRetrieval();
        uint64 iG711HeaderSize;
        bool iAutoPaused;
        bool iDownloadComplete;
        PVMFDownloadProgressInterface* iDownloadProgressInterface;
        uint32 iDownloadFileSize;
        PVMIDataStreamSyncInterface* iDataStreamInterface;
        PVMFDataStreamFactory* iDataStreamFactory;
        PVMFDataStreamReadCapacityObserver* iDataStreamReadCapacityObserver;
        PvmiDataStreamSession iDataStreamSessionID;
        PvmiDataStreamCommandId iRequestReadCapacityNotificationID;
        uint32 iLastNPTCalcInConvertSizeToTime;
        uint32 iFileSizeLastConvertedToTime;

        PVMFNodeCapability iCapability;
        PVLogger* iLogger;
        PVLogger* iDataPathLogger;
        PVLogger* iClockLogger;

        OSCL_wHeapString<OsclMemAllocator> iSourceURL;
        PVMFFormatType iSourceFormat;
        Oscl_FileServer iFileServer;
        bool iUseCPMPluginRegistry;
        OsclFileHandle* iFileHandle;
        PVMFLocalDataSource iCPMSourceData;

        CG711FileParser* iG711Parser;
        TPVG711FileInfo iG711FileInfo;
        bool oSourceIsCurrent;

        // stream id
        uint32 iStreamID;
        // flaf for bos
        bool iSendBOS;

        Oscl_Vector<PVG711FFNodeTrackPortInfo, OsclMemAllocator> iSelectedTrackList;

        // Reference counter for extension
        uint32 iExtensionRefCount;


        /* Content Policy Manager related */
        bool iPreviewMode;
        PVMFCPM* iCPM;
        PVMFSessionId iCPMSessionID;
        PVMFCPMContentType iCPMContentType;
        PVMFCPMPluginAccessInterfaceFactory* iCPMContentAccessFactory;
        PVMFMetadataExtensionInterface* iCPMMetaDataExtensionInterface;
        PVMFCPMPluginLicenseInterface* iCPMLicenseInterface;
        PVInterface* iCPMLicenseInterfacePVI;
        PvmiKvp iRequestedUsage;
        PvmiKvp iApprovedUsage;
        PvmiKvp iAuthorizationDataKvp;
        PVMFCPMUsageID iUsageID;
        PVMFCommandId iCPMInitCmdId;
        PVMFCommandId iCPMOpenSessionCmdId;
        PVMFCommandId iCPMRegisterContentCmdId;
        PVMFCommandId iCPMRequestUsageId;
        PVMFCommandId iCPMUsageCompleteCmdId;
        PVMFCommandId iCPMCloseSessionCmdId;
        PVMFCommandId iCPMResetCmdId;
        PVMFCommandId iCPMGetMetaDataKeysCmdId;
        PVMFCommandId iCPMGetMetaDataValuesCmdId;
        PVMFCommandId iCPMGetLicenseInterfaceCmdId;
        PVMFCommandId iCPMGetLicenseCmdId;
        PVMFCommandId iCPMCancelGetLicenseCmdId;

        void InitCPM();
        void OpenCPMSession();
        void CPMRegisterContent();
        bool GetCPMContentAccessFactory();
        bool GetCPMMetaDataExtensionInterface();
        void RequestUsage();
        void SendUsageComplete();
        void CloseCPMSession();
        void ResetCPM();
        void PopulateDRMInfo();
        void GetCPMMetaDataKeys();
        void GetCPMMetaDataValues();
        void GetCPMLicenseInterface();
        PVMFStatus iCPMRequestUsageCommandStatus;
        /* From PVMFCPMStatusObserver */
        void CPMCommandCompleted(const PVMFCmdResp& aResponse);

        PVMFStatus DoGetLicense(PVMFG711FFNodeCommand& aCmd,
                                bool aWideCharVersion = false);
        PVMFStatus DoCancelGetLicense(PVMFG711FFNodeCommand& aCmd);
        void CompleteGetLicense();
        PVMFStatus ParseG711File();
        uint32 iCountToClaculateRDATimeInterval;
};

#endif // PVMF_G711FFPARSER_NODE_H_INCLUDED

