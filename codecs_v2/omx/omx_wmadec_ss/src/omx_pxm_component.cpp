/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#include "oscl_base.h"
#include "pv_omxdefs.h"
#include "omx_pxm_component.h"

#if PROXY_INTERFACE
#include "omx_proxy_interface.h"
#endif


// Use default DLL entry point
#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

#define OMX_HALFRANGE_THRESHOLD 0x7FFFFFFF

OSCL_DLL_ENTRY_POINT_DEFAULT()

// This function is called by OMX_GetHandle and it creates an instance of the pxm component AO
OSCL_EXPORT_REF OMX_ERRORTYPE Wma_ssOmxComponentFactory(OMX_OUT OMX_HANDLETYPE* pHandle, OMX_IN  OMX_PTR pAppData, OMX_PTR pProxy, OMX_STRING aOmxLibName, OMX_PTR &aOmxLib, OMX_PTR aOsclUuid, OMX_U32 &aRefCount)
{
	LOGE_PXM_OMX("In Wma_ssOmxComponentFactory");
	OSCL_UNUSED_ARG(aOmxLibName);
    OSCL_UNUSED_ARG(aOmxLib);
    OSCL_UNUSED_ARG(aOsclUuid);
    OSCL_UNUSED_ARG(aRefCount);

    OpenmaxPxmAO* pOpenmaxAOType;
    OMX_ERRORTYPE Status;


    // move InitPxmOmxComponentFields content to actual constructor

    pOpenmaxAOType = (OpenmaxPxmAO*) OSCL_NEW(OpenmaxPxmAO, ());

    if (NULL == pOpenmaxAOType)
    {
        return OMX_ErrorInsufficientResources;
    }

    //Call the construct component to initialize OMX types
    Status = pOpenmaxAOType->ConstructComponent(pAppData, pProxy);

    *pHandle = pOpenmaxAOType->GetOmxHandle();

    LOGE_PXM_OMX("Out Wma_ssOmxComponentFactory");
    return Status;
    ///////////////////////////////////////////////////////////////////////////////////////
}

// This function is called by OMX_FreeHandle when component AO needs to be destroyed
OSCL_EXPORT_REF OMX_ERRORTYPE Wma_ssOmxComponentDestructor(OMX_IN OMX_HANDLETYPE pHandle, OMX_PTR &aOmxLib, OMX_PTR aOsclUuid, OMX_U32 &aRefCount)
{
	LOGE_PXM_OMX("In Wma_ssOmxComponentDestructor");
	OSCL_UNUSED_ARG(aOmxLib);
    OSCL_UNUSED_ARG(aOsclUuid);
    OSCL_UNUSED_ARG(aRefCount);


    // get pointer to component AO
    OpenmaxPxmAO* pOpenmaxAOType = (OpenmaxPxmAO*)((OMX_COMPONENTTYPE*)pHandle)->pComponentPrivate;

    // clean up decoder, OMX component stuff
    pOpenmaxAOType->DestroyComponent();

    // destroy the AO class
    OSCL_DELETE(pOpenmaxAOType);

    LOGE_PXM_OMX("Out Wma_ssOmxComponentDestructor");
    return OMX_ErrorNone;
}

#if DYNAMIC_LOAD_OMX_WMA_SS_COMPONENT
class Wma_ssOmxSharedLibraryInterface: public OsclSharedLibraryInterface,
            public OmxSharedLibraryInterface
{
    public:
        static Wma_ssOmxSharedLibraryInterface *Instance()
        {
            static Wma_ssOmxSharedLibraryInterface omxinterface;
            return &omxinterface;
        };

        OsclAny *QueryOmxComponentInterface(const OsclUuid& aOmxTypeId, const OsclUuid& aInterfaceId)
        {
            if (PV_OMX_WMA_SSDEC_UUID == aOmxTypeId)
            {
                if (PV_OMX_CREATE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(&Wma_ssOmxComponentFactory));
                }
                else if (PV_OMX_DESTROY_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(&Wma_ssOmxComponentDestructor));
                }
            }
            return NULL;
        };
        OsclAny *SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == PV_OMX_SHARED_INTERFACE)
            {
                return OSCL_STATIC_CAST(OmxSharedLibraryInterface*, this);
            }
            return NULL;
        };

    private:
        Wma_ssOmxSharedLibraryInterface() {};
};

// function to obtain the interface object from the shared library
extern "C"
{
    OSCL_EXPORT_REF OsclAny* PVGetInterface()
    {
        return Wma_ssOmxSharedLibraryInterface::Instance();
    }
}

#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

OMX_ERRORTYPE OpenmaxPxmAO::ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy)
{
   LOGE_PXM_OMX("In OpenmaxPxmAO::ConstructComponent");
    ComponentPortType* pInPort, *pOutPort;
    OMX_ERRORTYPE Status;

    iNumPorts = 2;
    iOmxComponent.nSize = sizeof(OMX_COMPONENTTYPE);
    iOmxComponent.pComponentPrivate = (OMX_PTR) this;  // pComponentPrivate points to THIS component AO class
    iOmxComponent.pApplicationPrivate = pAppData; // init the App data

        ipComponentProxy = pProxy;


#if PROXY_INTERFACE
    iPVCapabilityFlags.iIsOMXComponentMultiThreaded = OMX_TRUE;

    iOmxComponent.SendCommand = OpenmaxPxmAO::BaseComponentProxySendCommand;
    iOmxComponent.GetParameter = OpenmaxPxmAO::BaseComponentProxyGetParameter;
    iOmxComponent.SetParameter = OpenmaxPxmAO::BaseComponentProxySetParameter;
    iOmxComponent.GetConfig = OpenmaxPxmAO::BaseComponentProxyGetConfig;
    iOmxComponent.SetConfig = OpenmaxPxmAO::BaseComponentProxySetConfig;
    iOmxComponent.GetExtensionIndex = OpenmaxPxmAO::BaseComponentProxyGetExtensionIndex;
    iOmxComponent.GetState = OpenmaxPxmAO::BaseComponentProxyGetState;
    iOmxComponent.UseBuffer = OpenmaxPxmAO::BaseComponentProxyUseBuffer;
    iOmxComponent.AllocateBuffer = OpenmaxPxmAO::BaseComponentProxyAllocateBuffer;
    iOmxComponent.FreeBuffer = OpenmaxPxmAO::BaseComponentProxyFreeBuffer;
    iOmxComponent.EmptyThisBuffer = OpenmaxPxmAO::BaseComponentProxyEmptyThisBuffer;
    iOmxComponent.FillThisBuffer = OpenmaxPxmAO::BaseComponentProxyFillThisBuffer;

#else
    iPVCapabilityFlags.iIsOMXComponentMultiThreaded = OMX_FALSE;

    iOmxComponent.SendCommand = OpenmaxPxmAO::BaseComponentSendCommand;
    iOmxComponent.GetParameter = OpenmaxPxmAO::BaseComponentGetParameter;
    iOmxComponent.SetParameter = OpenmaxPxmAO::BaseComponentSetParameter;
    iOmxComponent.GetConfig = OpenmaxPxmAO::BaseComponentGetConfig;
    iOmxComponent.SetConfig = OpenmaxPxmAO::BaseComponentSetConfig;
    iOmxComponent.GetExtensionIndex = OpenmaxPxmAO::BaseComponentGetExtensionIndex;
    iOmxComponent.GetState = OpenmaxPxmAO::BaseComponentGetState;
    iOmxComponent.UseBuffer = OpenmaxPxmAO::BaseComponentUseBuffer;
    iOmxComponent.AllocateBuffer = OpenmaxPxmAO::BaseComponentAllocateBuffer;
    iOmxComponent.FreeBuffer = OpenmaxPxmAO::BaseComponentFreeBuffer;
    iOmxComponent.EmptyThisBuffer = OpenmaxPxmAO::BaseComponentEmptyThisBuffer;
    iOmxComponent.FillThisBuffer = OpenmaxPxmAO::BaseComponentFillThisBuffer;
#endif

    iOmxComponent.SetCallbacks = OpenmaxPxmAO::BaseComponentSetCallbacks;
    iOmxComponent.nVersion.s.nVersionMajor = SPECVERSIONMAJOR;
    iOmxComponent.nVersion.s.nVersionMinor = SPECVERSIONMINOR;
    iOmxComponent.nVersion.s.nRevision = SPECREVISION;
    iOmxComponent.nVersion.s.nStep = SPECSTEP;

    // PV capability
    iPVCapabilityFlags.iOMXComponentSupportsExternalInputBufferAlloc = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsExternalOutputBufferAlloc = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsMovableInputBuffers = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsPartialFrames = OMX_FALSE;
    iPVCapabilityFlags.iOMXComponentUsesNALStartCodes = OMX_FALSE;
    iPVCapabilityFlags.iOMXComponentCanHandleIncompleteFrames = OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentUsesFullAVCFrames = OMX_FALSE;


    if (ipAppPriv)
    {
        oscl_free(ipAppPriv);
        ipAppPriv = NULL;
    }

    ipAppPriv = (ComponentPrivateType*) oscl_malloc(sizeof(ComponentPrivateType));
    if (NULL == ipAppPriv)
    {
        return OMX_ErrorInsufficientResources;
    }

    //Construct base class now
    Status = ConstructBaseComponent(pAppData);

    if (OMX_ErrorNone != Status)
    {
        return Status;
    }

    /** Domain specific section for the ports. */
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nPortIndex = OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.eDomain = OMX_PortDomainAudio;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.cMIMEType = (OMX_STRING)"audio/x-ms-wma";
    //ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.cMIMEType = (OMX_STRING)"FORMATUNKNOWN";
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.pNativeRender = 0;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.bFlagErrorConcealment = OMX_FALSE;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.eEncoding = OMX_AUDIO_CodingWMA;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.eDir = OMX_DirInput;
    //Set to a default value, will change later during setparameter call
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferCountActual = NUMBER_INPUT_BUFFER_PXM;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferCountMin = 1;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferSize = INPUT_BUFFER_SIZE_PXM;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.bEnabled = OMX_TRUE;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.bPopulated = OMX_FALSE;


    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nPortIndex = OMX_PORT_OUTPUTPORT_INDEX;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.eDomain = OMX_PortDomainAudio;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.cMIMEType = (OMX_STRING)"raw";
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.pNativeRender = 0;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.bFlagErrorConcealment = OMX_FALSE;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.eEncoding = OMX_AUDIO_CodingPCM;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.eDir = OMX_DirOutput;
    //Set to a default value, will change later during setparameter call
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferCountActual = NUMBER_OUTPUT_BUFFER_PXM;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferCountMin = 1;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferSize = OUTPUT_BUFFER_SIZE_PXM * 6;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.bEnabled = OMX_TRUE;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.bPopulated = OMX_FALSE;

    //Default values for Pxm audio param port
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.nPortIndex = OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.nChannels = 2;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.nBitRate = 0;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.eFormat = OMX_AUDIO_WMAFormat9;//PremCheck
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.eProfile = OMX_AUDIO_WMAProfileL1;//PremCheck
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.nSamplingRate = 44100;//PremCheck
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.nBlockAlign = 0;//PremCheck
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam.nEncodeOptions = 31;//PremCheck

#if 0
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioEqualizerType.nPortIndex = OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioEqualizerType.sBandIndex.nMin = 0;
    //Taken these value from from pxm decoder component
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioEqualizerType.sBandIndex.nValue = (e_equalization) flat;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioEqualizerType.sBandIndex.nMax = (e_equalization) flat_;
#endif

    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nPortIndex = OMX_PORT_OUTPUTPORT_INDEX;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nChannels = 2;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.eNumData = OMX_NumericalDataSigned;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.bInterleaved = OMX_TRUE;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nBitPerSample = 16;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nSamplingRate = 44100;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.ePCMMode = OMX_AUDIO_PCMModeLinear;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.eChannelMapping[0] = OMX_AUDIO_ChannelLF;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.eChannelMapping[1] = OMX_AUDIO_ChannelRF;

    iPortTypesParam.nPorts = 2;
    iPortTypesParam.nStartPortNumber = 0;

    pInPort = (ComponentPortType*) ipPorts[OMX_PORT_INPUTPORT_INDEX];
    pOutPort = (ComponentPortType*) ipPorts[OMX_PORT_OUTPUTPORT_INDEX];

    SetHeader(&pInPort->AudioParam, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
    pInPort->AudioParam.nPortIndex = 0;
    pInPort->AudioParam.nIndex = 0;
    pInPort->AudioParam.eEncoding = OMX_AUDIO_CodingWMA;

    SetHeader(&pOutPort->AudioParam, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
    pOutPort->AudioParam.nPortIndex = 1;
    pOutPort->AudioParam.nIndex = 0;
    pOutPort->AudioParam.eEncoding = OMX_AUDIO_CodingPCM;

    iOutputFrameLength = OUTPUT_BUFFER_SIZE_PXM;

    if (ipPxmDec)
    {
        OSCL_DELETE(ipPxmDec);
        ipPxmDec = NULL;
    }

    ipPxmDec = OSCL_NEW(OmxPxmDecoder, ());
    if (ipPxmDec == NULL)
    {
        return OMX_ErrorInsufficientResources;
    }

    //oscl_memset(ipPxmDec, 0, sizeof(OmxPxmDecoder));

    iSamplesPerFrame = DEFAULT_SAMPLES_PER_FRAME_PXM;
    iOutputMilliSecPerFrame = iCurrentFrameTS.GetFrameDuration();

#if PROXY_INTERFACE

    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSendCommand = BaseComponentSendCommand;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetParameter = BaseComponentGetParameter;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSetParameter = BaseComponentSetParameter;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetConfig = BaseComponentGetConfig;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSetConfig = BaseComponentSetConfig;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetExtensionIndex = BaseComponentGetExtensionIndex;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetState = BaseComponentGetState;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentUseBuffer = BaseComponentUseBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentAllocateBuffer = BaseComponentAllocateBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentFreeBuffer = BaseComponentFreeBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentEmptyThisBuffer = BaseComponentEmptyThisBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentFillThisBuffer = BaseComponentFillThisBuffer;

#endif
	LOGE_PXM_OMX("Out OpenmaxPxmAO::ConstructComponent");
    return OMX_ErrorNone;
}

/** This function is called by the omx core when the component
	* is disposed by the IL client with a call to FreeHandle().
	*/

OMX_ERRORTYPE OpenmaxPxmAO::DestroyComponent()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::DestroyComponent");
    if (iIsInit != OMX_FALSE)
    {
        ComponentDeInit();
    }

    //Destroy the base class now
    DestroyBaseComponent();

    if (ipPxmDec)
    {
        OSCL_DELETE(ipPxmDec);
        ipPxmDec = NULL;
    }

    if (ipAppPriv)
    {
        ipAppPriv->CompHandle = NULL;

        oscl_free(ipAppPriv);
        ipAppPriv = NULL;
    }

    LOGE_PXM_OMX("Out OpenmaxPxmAO::DestroyComponent");
    return OMX_ErrorNone;
}

void OpenmaxPxmAO::SyncWithInputTimestamp()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::SyncWithInputTimestamp");
    //Do not check for silence insertion if the clip is repositioned
    if (OMX_FALSE == iRepositionFlag)
    {
        CheckForSilenceInsertion();
    }

    /* Set the current timestamp equal to input buffer timestamp in case of
     * a) All input frames
     * b) First frame after repositioning */
    if (OMX_FALSE == iSilenceInsertionInProgress || OMX_TRUE == iRepositionFlag)
    {
        // Set the current timestamp equal to input buffer timestamp
        iCurrentFrameTS.SetFromInputTimestamp(iFrameTimestamp);

        //Reset the flag back to false, once timestamp has been updated from input frame
        if (OMX_TRUE == iRepositionFlag)
        {
            iRepositionFlag = OMX_FALSE;
        }
    }
    LOGE_PXM_OMX("Out OpenmaxPxmAO::SyncWithInputTimestamp");
}

void OpenmaxPxmAO::ResetComponent()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::ResetComponent");
    // reset decoder
    if (ipPxmDec)
    {
        ipPxmDec->ResetDecoder();

    	//Set this length to zero for flushing the current input buffer
    	ipPxmDec->iInputUsedLength = 0;
    	//ipPxmDec->iInitFlag = 0;
	}

	iTempOutLen = 0;
    LOGE_PXM_OMX("Out OpenmaxPxmAO::ResetComponent");
}


void OpenmaxPxmAO::ProcessInBufferFlag()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::ProcessInBufferFlag");
    iIsInputBufferEnded = OMX_FALSE;
    LOGE_PXM_OMX("Out OpenmaxPxmAO::ProcessInBufferFlag");
}

void OpenmaxPxmAO::ProcessData()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::ProcessData");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData IN"));

    QueueType* pInputQueue  = ipPorts[OMX_PORT_INPUTPORT_INDEX]->pBufferQueue;
    QueueType* pOutputQueue = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->pBufferQueue;

    ComponentPortType* pInPort = (ComponentPortType*) ipPorts[OMX_PORT_INPUTPORT_INDEX];
    ComponentPortType* pOutPort = ipPorts[OMX_PORT_OUTPUTPORT_INDEX];
    OMX_COMPONENTTYPE* pHandle = &iOmxComponent;

    OMX_U8*	pOutBuffer;
    OMX_U32	OutputLength;
    OMX_S32 DecodeReturn;
    OMX_BOOL ResizeNeeded = OMX_FALSE;


    OMX_U32 TempInputBufferSize = (2 * sizeof(uint8) * (ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferSize));

    if ((!iIsInputBufferEnded) || iEndofStream)
    {
        if (OMX_TRUE == iSilenceInsertionInProgress)
        {
            DoSilenceInsertion();
            //If the flag is still true, come back to this routine again
            if (OMX_TRUE == iSilenceInsertionInProgress)
            {
                return;
            }
        }

        //Check whether prev output bufer has been released or not
        if (OMX_TRUE == iNewOutBufRequired)
        {
            //Check whether a new output buffer is available or not
            if (0 == (GetQueueNumElem(pOutputQueue)))
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData OUT output buffer unavailable"));
                return;
            }

            ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
            if (NULL == ipOutputBuffer)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData Error, Output Buffer Dequeue returned NULL, OUT"));
                return;
            }
            ipOutputBuffer->nFilledLen = 0;
            iNewOutBufRequired = OMX_FALSE;

            //Set the current timestamp to the output buffer timestamp
            ipOutputBuffer->nTimeStamp = iCurrentFrameTS.GetConvertedTs();

            // Copy the output buffer that was stored locally before dynamic port reconfiguration
            // in the new omx buffer received.
            if (OMX_TRUE == iSendOutBufferAfterPortReconfigFlag)
            {
                if ((ipTempOutBufferForPortReconfig)
                        && (iSizeOutBufferForPortReconfig <= ipOutputBuffer->nAllocLen))
                {
                    oscl_memcpy(ipOutputBuffer->pBuffer, ipTempOutBufferForPortReconfig, iSizeOutBufferForPortReconfig);
                    ipOutputBuffer->nFilledLen = iSizeOutBufferForPortReconfig;
                    ipOutputBuffer->nTimeStamp = iTimestampOutBufferForPortReconfig;
                }

                iSendOutBufferAfterPortReconfigFlag = OMX_FALSE;

                //Send the output buffer back only when it has become full
                if ((ipOutputBuffer->nAllocLen - ipOutputBuffer->nFilledLen) < iOutputFrameLength)
                {
                    ReturnOutputBuffer(ipOutputBuffer, pOutPort);
                }

                //Free the temp output buffer
                if (ipTempOutBufferForPortReconfig)
                {
                    oscl_free(ipTempOutBufferForPortReconfig);
                    ipTempOutBufferForPortReconfig = NULL;
                    iSizeOutBufferForPortReconfig = 0;
                }

                //Dequeue new output buffer if required to continue decoding the next frame
                if (OMX_TRUE == iNewOutBufRequired)
                {
                    if (0 == (GetQueueNumElem(pOutputQueue)))
                    {
                        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData OUT, output buffer unavailable"));
                        return;
                    }

                    ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
                    if (NULL == ipOutputBuffer)
                    {
                        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData Error, Output Buffer Dequeue returned NULL, OUT"));
                        return;
                    }

                    ipOutputBuffer->nFilledLen = 0;
                    iNewOutBufRequired = OMX_FALSE;

                    ipOutputBuffer->nTimeStamp = iCurrentFrameTS.GetConvertedTs();
                }
            }
        }

        /* Code for the marking buffer. Takes care of the OMX_CommandMarkBuffer
         * command and hMarkTargetComponent as given by the specifications
         */
        if (ipMark != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent = ipMark->hMarkTargetComponent;
            ipOutputBuffer->pMarkData = ipMark->pMarkData;
            ipMark = NULL;
        }

        if (ipTargetComponent != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent = ipTargetComponent;
            ipOutputBuffer->pMarkData = iTargetMarkData;
            ipTargetComponent = NULL;

        }
        //Mark buffer code ends here

        pOutBuffer = &ipOutputBuffer->pBuffer[ipOutputBuffer->nFilledLen];
        OutputLength = 0;

        /* Copy the left-over data from last input buffer that is stored in temporary
         * buffer to the next incoming buffer.
         */
        if (iTempInputBufferLength > 0 &&
                ((iInputCurrLength + iTempInputBufferLength) < TempInputBufferSize))
        {
            oscl_memcpy(&ipTempInputBuffer[iTempInputBufferLength], ipFrameDecodeBuffer, iInputCurrLength);
            iInputCurrLength += iTempInputBufferLength;
            iTempInputBufferLength = 0;
            ipFrameDecodeBuffer = ipTempInputBuffer;
        }

        LOGE_PXM_OMX("Before ipPxmDec->PxmDecodeFrame");
        //Output buffer is passed as a short pointer
        DecodeReturn = ipPxmDec->PxmDecodeFrame((OMX_S16*) pOutBuffer,
                                                (OMX_U32*) & OutputLength,
                                                &(ipFrameDecodeBuffer),
                                                &iInputCurrLength,
                                                &iFrameCount,
                                                &(ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode),
                                                &(ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioWmaParam),
                                                iEndOfFrameFlag,
                                                &ResizeNeeded);

		LOGE_PXM_OMX("After ipPxmDec->PxmDecodeFrame");

        if (ResizeNeeded == OMX_TRUE)
        {
            if (0 != OutputLength)
            {
                iOutputFrameLength = OutputLength * 2;

				iTempOutLen = OutputLength;

                //Update the timestamp
                iSamplesPerFrame = OutputLength / ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nChannels;

                iCurrentFrameTS.SetParameters(ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nSamplingRate, iSamplesPerFrame);
                iOutputMilliSecPerFrame = iCurrentFrameTS.GetFrameDuration();

				LOGE_PXM_OMX(" Info :: after resizing Output milli secs pers frame is %d", iOutputMilliSecPerFrame);

            }

            // set the flag to disable further processing until Client reacts to this
            //	by doing dynamic port reconfiguration
            iResizePending = OMX_TRUE;

            /* Do not return the output buffer generated yet, store it locally
             * and wait for the dynamic port reconfig to complete */
            if ((NULL == ipTempOutBufferForPortReconfig))
            {
                ipTempOutBufferForPortReconfig = (OMX_U8*) oscl_malloc(sizeof(uint8) * OutputLength * 2);
                if (NULL == ipTempOutBufferForPortReconfig)
                {
                    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData error, insufficient resources"));
                    return;
                }
            }

            //Copy the omx output buffer to the temporary internal buffer
            oscl_memcpy(ipTempOutBufferForPortReconfig, pOutBuffer, OutputLength * 2);
            iSizeOutBufferForPortReconfig = OutputLength * 2;

            //Set the current timestamp to the output buffer timestamp for the first output frame
            //Later it will be done at the time of dequeue
            iTimestampOutBufferForPortReconfig = iCurrentFrameTS.GetConvertedTs();

            iCurrentFrameTS.UpdateTimestamp(iSamplesPerFrame);
            //Make this length 0 so that no output buffer is returned by the component
            OutputLength = 0;

            // send port settings changed event
            OMX_COMPONENTTYPE* pHandle = (OMX_COMPONENTTYPE*) ipAppPriv->CompHandle;

            (*(ipCallbacks->EventHandler))
            (pHandle,
             iCallbackData,
             OMX_EventPortSettingsChanged, //The command was completed
             OMX_PORT_OUTPUTPORT_INDEX,
             0,
             NULL);
        }

        ipOutputBuffer->nFilledLen += OutputLength * 2;
        //offset not required in our case, set it to zero
        ipOutputBuffer->nOffset = 0;

        if ((OutputLength > 0) && (iTempOutLen < OutputLength))
        {

            iTempOutLen = OutputLength;
			iOutputFrameLength = OutputLength * 2;
			
			//Update the timestamp
			iSamplesPerFrame = OutputLength / ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nChannels;
			
			iCurrentFrameTS.SetParameters(ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioPcmMode.nSamplingRate, iSamplesPerFrame);
			iOutputMilliSecPerFrame = iCurrentFrameTS.GetFrameDuration();

			//iCurrentFrameTS.UpdateTimestamp(iSamplesPerFrame);
        } 

        if (OutputLength > 0)
        {
            iCurrentFrameTS.UpdateTimestamp(iSamplesPerFrame);
        }

        /* If EOS flag has come from the client & there are no more
         * input buffers to decode, send the callback to the client
         */
        if (OMX_TRUE == iEndofStream)
        {
            /* Changed the condition here. If EOS has come and decoder can't decode
             * the buffer, do not wait for buffer length to become zero and return
             * the callback as the current buffer may contain partial frame and
             * no other data is going to come now
             */
            if (PXMDEC_SUCCESS != DecodeReturn)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData EOS callback send"));

                (*(ipCallbacks->EventHandler))
                (pHandle,
                 iCallbackData,
                 OMX_EventBufferFlag,
                 1,
                 OMX_BUFFERFLAG_EOS,
                 NULL);

                iEndofStream = OMX_FALSE;

                ipOutputBuffer->nFlags |= OMX_BUFFERFLAG_EOS;

                ReturnOutputBuffer(ipOutputBuffer, pOutPort);
                ipOutputBuffer = NULL;

                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData OUT"));

                return;
            }
        }


        if (PXMDEC_SUCCESS == DecodeReturn)
        {
	        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "ipInputBuffer = %d",ipInputBuffer));
	        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "iInputCurrLength = %d",iInputCurrLength));
            ipInputBuffer->nFilledLen = iInputCurrLength;
        }
        else if (PXMDEC_INCOMPLETE_FRAME == DecodeReturn)
        {
            /* If decoder returns MP4AUDEC_INCOMPLETE_FRAME,
             * this indicates the input buffer contains less than a frame data
             * Copy it to a temp buffer to be used in next decode call
             */
            oscl_memcpy(ipTempInputBuffer, ipFrameDecodeBuffer, iInputCurrLength);
            iTempInputBufferLength = iInputCurrLength;
            ipInputBuffer->nFilledLen = 0;
            iInputCurrLength = 0;
        }
        else
        {
            //bitstream error, discard the current data as it can't be decoded further
            ipInputBuffer->nFilledLen = 0;
            iInputCurrLength = 0;

            //Report it to the client via a callback
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData ErrorStreamCorrupt callback send"));

            (*(ipCallbacks->EventHandler))
            (pHandle,
             iCallbackData,
             OMX_EventError,
             OMX_ErrorStreamCorrupt,
             0,
             NULL);

        }

        //Return the input buffer if it has been consumed fully by the decoder
        if (0 == ipInputBuffer->nFilledLen)
        {
            ReturnInputBuffer(ipInputBuffer, pInPort);
            ipInputBuffer = NULL;
            iIsInputBufferEnded = OMX_TRUE;
            iInputCurrLength = 0;
        }

        //Send the output buffer back when it has become full
        //if ((ipOutputBuffer->nAllocLen - ipOutputBuffer->nFilledLen) < (iOutputFrameLength))
		if (ipOutputBuffer->nFilledLen)
        {
            LOGE_PXM_OMX(" Info :: output buffer filled len is %d ", ipOutputBuffer->nFilledLen);
            ReturnOutputBuffer(ipOutputBuffer, pOutPort);
            ipOutputBuffer = NULL;
        }

        /* If there is some more processing left with current buffers, re-schedule the AO
         * Do not go for more than one round of processing at a time.
         * This may block the AO longer than required.
         */
        if (((iInputCurrLength != 0 || GetQueueNumElem(pInputQueue) > 0)
                && (GetQueueNumElem(pOutputQueue) > 0) && (ResizeNeeded == OMX_FALSE))
                || (OMX_TRUE == iEndofStream))
        {
            RunIfNotReady();
        }
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ProcessData OUT"));
    LOGE_PXM_OMX("Out OpenmaxPxmAO::ProcessData");
    return;
}

void OpenmaxPxmAO::ComponentGetRolesOfComponent(OMX_STRING* aRoleString)
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::ComponentGetRolesOfComponent");
    *aRoleString = (OMX_STRING)"audio_decoder.pxm";
    LOGE_PXM_OMX("Out OpenmaxPxmAO::ComponentGetRolesOfComponent");
}

//Active object constructor
OpenmaxPxmAO::OpenmaxPxmAO()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::OpenmaxPxmAO");
    ipPxmDec = NULL;

    if (!IsAdded())
    {
        AddToScheduler();
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : constructed"));
    LOGE_PXM_OMX("Out OpenmaxPxmAO::OpenmaxPxmAO");
}


//Active object destructor
OpenmaxPxmAO::~OpenmaxPxmAO()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::~OpenmaxPxmAO");
    if (IsAdded())
    {
        RemoveFromScheduler();
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : destructed"));
    LOGE_PXM_OMX("Out OpenmaxPxmAO::~OpenmaxPxmAO");
}

/** The Initialization function
 */
OMX_ERRORTYPE OpenmaxPxmAO::ComponentInit()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::ComponentInit");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ComponentInit IN"));

    OMX_BOOL Status = OMX_FALSE;
	iTempOutLen = 0;

    if (OMX_TRUE == iIsInit)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ComponentInit error incorrect operation"));
        LOGE_PXM_OMX("Out OpenmaxPxmAO::ComponentInit1");
        return OMX_ErrorIncorrectStateOperation;
    }
    iIsInit = OMX_TRUE;

    //Pxm lib init
    if (!iCodecReady)
    {
        //Status = ipPxmDec->PxmDecInit(&(ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioEqualizerType));
        Status = ipPxmDec->PxmDecInit();
        iCodecReady = OMX_TRUE;
    }

    iInputCurrLength = 0;
    //Used in dynamic port reconfiguration
    iFrameCount = 0;

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ComponentInit OUT"));

    if (OMX_TRUE == Status)
    {
	    LOGE_PXM_OMX("Out OpenmaxPxmAO::ComponentInit2");
        return OMX_ErrorNone;
    }
    else
    {
	    LOGE_PXM_OMX("Out OpenmaxPxmAO::ComponentInit3");
        return OMX_ErrorInvalidComponent;
    }
}

/** This function is called upon a transition to the idle or invalid state.
 *  Also it is called by the Wma_ssOmxComponentDestructor() function
 */
OMX_ERRORTYPE OpenmaxPxmAO::ComponentDeInit()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::ComponentDeInit");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ComponentDeInit IN"));

    iIsInit = OMX_FALSE;

    if (iCodecReady)
    {
        ipPxmDec->PxmDecDeinit();
        iCodecReady = OMX_FALSE;
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : ComponentDeInit OUT"));

    LOGE_PXM_OMX("Out OpenmaxPxmAO::ComponentDeInit");
    return OMX_ErrorNone;

}

//Check whether silence insertion is required here or not
void OpenmaxPxmAO::CheckForSilenceInsertion()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::CheckForSilenceInsertion");

    OMX_TICKS CurrTimestamp, TimestampGap;
    //Set the flag to false by default
    iSilenceInsertionInProgress = OMX_FALSE;

    CurrTimestamp = iCurrentFrameTS.GetCurrentTimestamp();
    TimestampGap = iFrameTimestamp - CurrTimestamp;

	LOGE_PXM_OMX(" Info :: CurrentTimeStamp= %d, TimeStampGap= %d ", CurrTimestamp, TimestampGap);
	
    if ((TimestampGap > OMX_HALFRANGE_THRESHOLD) || (TimestampGap < iOutputMilliSecPerFrame && iFrameCount > 0))
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : CheckForSilenceInsertion OUT - No need to insert silence"));
        LOGE_PXM_OMX("Out OpenmaxPxmAO::CheckForSilenceInsertion1");
        return;
    }

    //Silence insertion needed, mark the flag to true
    if (iFrameCount > 0)
    {
        iSilenceInsertionInProgress = OMX_TRUE;
        //Determine the number of silence frames to insert
        if (0 != iOutputMilliSecPerFrame)
        {
            iSilenceFramesNeeded = TimestampGap / iOutputMilliSecPerFrame;
        }
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : CheckForSilenceInsertion OUT - Silence Insertion required here"));
    }

    LOGE_PXM_OMX("Out OpenmaxPxmAO::CheckForSilenceInsertion2");
    return;
}

//Currently we are doing zero frame insertion in this routine
void OpenmaxPxmAO::DoSilenceInsertion()
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::DoSilenceInsertion");
    QueueType* pOutputQueue = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->pBufferQueue;
    ComponentPortType* pOutPort = ipPorts[OMX_PORT_OUTPUTPORT_INDEX];
    OMX_U8*	pOutBuffer = NULL;


    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : DoSilenceInsertion IN"));

	LOGE_PXM_OMX(" OpenmaxPxmAO::DoSilenceInsertion :: number of silence frames needed is %d", iSilenceFramesNeeded);
	
    while (iSilenceFramesNeeded > 0)
    {
        //Check whether prev output bufer has been consumed or not
        if (OMX_TRUE == iNewOutBufRequired)
        {
            //Check whether a new output buffer is available or not
            if (0 == (GetQueueNumElem(pOutputQueue)))
            {
                //Resume Silence insertion next time when component will be called
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : DoSilenceInsertion OUT output buffer unavailable"));
                iSilenceInsertionInProgress = OMX_TRUE;
                return;
            }

            ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);
            if (NULL == ipOutputBuffer)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : DoSilenceInsertion Error, Output Buffer Dequeue returned NULL, OUT"));
                iSilenceInsertionInProgress = OMX_TRUE;
                return;
            }

			LOGE_PXM_OMX(" OpenmaxPxmAO::DoSilenceInsertion :: output buffer dequeued ");
	
            ipOutputBuffer->nFilledLen = 0;
            iNewOutBufRequired = OMX_FALSE;

            //Set the current timestamp to the output buffer timestamp
            ipOutputBuffer->nTimeStamp = iCurrentFrameTS.GetConvertedTs();
        }


        pOutBuffer = &ipOutputBuffer->pBuffer[ipOutputBuffer->nFilledLen];
        oscl_memset(pOutBuffer, 0, iOutputFrameLength);

		LOGE_PXM_OMX(" OpenmaxPxmAO::DoSilenceInsertion :: output buffer length before update is %d , outputFrame length is %d", ipOutputBuffer->nFilledLen, iOutputFrameLength);

        ipOutputBuffer->nFilledLen += iOutputFrameLength;
        ipOutputBuffer->nOffset = 0;
        iCurrentFrameTS.UpdateTimestamp(iSamplesPerFrame);
		
		LOGE_PXM_OMX(" OpenmaxPxmAO::DoSilenceInsertion :: output buffer length after update is %d ", ipOutputBuffer->nFilledLen);

        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : DoSilenceInsertion - One frame of zeros inserted"));

        //Send the output buffer back when it has become full
        if ((ipOutputBuffer->nAllocLen - ipOutputBuffer->nFilledLen) < iOutputFrameLength)
        {
            ReturnOutputBuffer(ipOutputBuffer, pOutPort);
            ipOutputBuffer = NULL;
		
			LOGE_PXM_OMX(" OpenmaxPxmAO::DoSilenceInsertion :: returned output buffer ");
        }

        // Decrement the silence frame counter
        --iSilenceFramesNeeded;
    }

    /* Completed Silence insertion successfully, now consider the input buffer already dequeued
     * for decoding & update the timestamps */

    iSilenceInsertionInProgress = OMX_FALSE;
    iCurrentFrameTS.SetFromInputTimestamp(iFrameTimestamp);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OpenmaxPxmAO : DoSilenceInsertion OUT - Done successfully"));

    LOGE_PXM_OMX("Out OpenmaxPxmAO::DoSilenceInsertion");
    return;
}


OMX_ERRORTYPE OpenmaxPxmAO::GetConfig(
    OMX_IN  OMX_HANDLETYPE hComponent,
    OMX_IN  OMX_INDEXTYPE nIndex,
    OMX_INOUT OMX_PTR pComponentConfigStructure)
{
	LOGE_PXM_OMX("In OpenmaxPxmAO::GetConfig");
    OSCL_UNUSED_ARG(hComponent);
    OSCL_UNUSED_ARG(nIndex);
    OSCL_UNUSED_ARG(pComponentConfigStructure);
    LOGE_PXM_OMX("Out OpenmaxPxmAO::GetConfig");
    return OMX_ErrorNotImplemented;
}
