/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name	: EVRC Encoder
 * @File Name			: omx_evrcenc_component.cpp
 * @File Description	: OpenMax component file for evrc encoder.
 * @Author				: Sudhir Vyas				 
 * @Created Date		: 16-06-2009		     
 * @Modification History
 * Version:				Date:				By:				Change:		
 * -------------------------------------------------------------------
 */
#include "omx_evrcenc_component.h"

#if PROXY_INTERFACE
#include "omx_proxy_interface.h"
#endif

// This function is called by OMX_GetHandle and it creates an instance of the evrc component AO
OMX_ERRORTYPE EvrcEncOmxComponentFactory(OMX_OUT OMX_HANDLETYPE* pHandle, OMX_IN  OMX_PTR pAppData, OMX_PTR pProxy, OMX_STRING aOmxLibName, OMX_PTR &aOmxLib, OMX_PTR aOsclUuid, OMX_U32 &aRefCount)
{
    OSCL_UNUSED_ARG(aOmxLibName);
    OSCL_UNUSED_ARG(aOmxLib);
    OSCL_UNUSED_ARG(aOsclUuid);
    OSCL_UNUSED_ARG(aRefCount);

    OmxComponentEvrcEncoderAO* pOpenmaxAOType = NULL;
    OMX_ERRORTYPE Status = OMX_ErrorNone;

    // move InitEvrcOmxComponentFields content to actual constructor

    pOpenmaxAOType = (OmxComponentEvrcEncoderAO*) OSCL_NEW(OmxComponentEvrcEncoderAO, ());

    if (NULL == pOpenmaxAOType)
    {
        return OMX_ErrorInsufficientResources;
    }

    //Call the construct component to initialize OMX types
    Status = pOpenmaxAOType->ConstructComponent(pAppData, pProxy);

    *pHandle = pOpenmaxAOType->GetOmxHandle();

    return Status;
}

// This function is called by OMX_FreeHandle when component AO needs to be destroyed
OMX_ERRORTYPE EvrcEncOmxComponentDestructor(OMX_IN OMX_HANDLETYPE pHandle, OMX_PTR &aOmxLib, OMX_PTR aOsclUuid, OMX_U32 &aRefCount)
{
    OSCL_UNUSED_ARG(aOmxLib);
    OSCL_UNUSED_ARG(aOsclUuid);
    OSCL_UNUSED_ARG(aRefCount);

    // get pointer to component AO
    OmxComponentEvrcEncoderAO* pOpenmaxAOType = (OmxComponentEvrcEncoderAO*)((OMX_COMPONENTTYPE*)pHandle)->pComponentPrivate;

    // clean up encoder, OMX component stuff
    pOpenmaxAOType->DestroyComponent();

    // destroy the AO class
    OSCL_DELETE(pOpenmaxAOType);

    return OMX_ErrorNone;
}

#if DYNAMIC_LOAD_OMX_EVRCENC_COMPONENT

class EvrcEncOmxSharedLibraryInterface:  public OsclSharedLibraryInterface,
            public OmxSharedLibraryInterface

{
    public:
        static EvrcEncOmxSharedLibraryInterface *Instance()
        {
            static EvrcEncOmxSharedLibraryInterface omxinterface;
            return &omxinterface;
        };

        OsclAny *QueryOmxComponentInterface(const OsclUuid& aOmxTypeId, const OsclUuid& aInterfaceId)
        {
            if (PV_OMX_EVRCENC_UUID == aOmxTypeId)
            {
                if (PV_OMX_CREATE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(&EvrcEncOmxComponentFactory));
                }
                else if (PV_OMX_DESTROY_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(&EvrcEncOmxComponentDestructor));
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
        EvrcEncOmxSharedLibraryInterface() {};
};

// function to obtain the interface object from the shared library
extern "C"
{
    OSCL_EXPORT_REF OsclAny* PVGetInterface()
    {
        return EvrcEncOmxSharedLibraryInterface::Instance();
    }
}

#endif

OMX_ERRORTYPE OmxComponentEvrcEncoderAO::ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy)
{
    ComponentPortType*	pInPort		= NULL;
	ComponentPortType*	pOutPort	= NULL;
    OMX_ERRORTYPE		Status		= OMX_ErrorNone;

    iNumPorts = 2;
    iCompressedFormatPortNum = OMX_PORT_OUTPUTPORT_INDEX;
    iOmxComponent.nSize = sizeof(OMX_COMPONENTTYPE);
    iOmxComponent.pComponentPrivate = (OMX_PTR) this;  // pComponentPrivate points to THIS component AO class
    ipComponentProxy = pProxy;
    iOmxComponent.pApplicationPrivate = pAppData; // init the App data


#if PROXY_INTERFACE
    iPVCapabilityFlags.iIsOMXComponentMultiThreaded = OMX_TRUE;

    iOmxComponent.SendCommand			= OmxComponentEvrcEncoderAO::BaseComponentProxySendCommand;
    iOmxComponent.GetParameter			= OmxComponentEvrcEncoderAO::BaseComponentProxyGetParameter;
    iOmxComponent.SetParameter			= OmxComponentEvrcEncoderAO::BaseComponentProxySetParameter;
    iOmxComponent.GetConfig				= OmxComponentEvrcEncoderAO::BaseComponentProxyGetConfig;
    iOmxComponent.SetConfig				= OmxComponentEvrcEncoderAO::BaseComponentProxySetConfig;
    iOmxComponent.GetExtensionIndex		= OmxComponentEvrcEncoderAO::BaseComponentProxyGetExtensionIndex;
    iOmxComponent.GetState				= OmxComponentEvrcEncoderAO::BaseComponentProxyGetState;
    iOmxComponent.UseBuffer				= OmxComponentEvrcEncoderAO::BaseComponentProxyUseBuffer;
    iOmxComponent.AllocateBuffer		= OmxComponentEvrcEncoderAO::BaseComponentProxyAllocateBuffer;
    iOmxComponent.FreeBuffer			= OmxComponentEvrcEncoderAO::BaseComponentProxyFreeBuffer;
    iOmxComponent.EmptyThisBuffer		= OmxComponentEvrcEncoderAO::BaseComponentProxyEmptyThisBuffer;
    iOmxComponent.FillThisBuffer		= OmxComponentEvrcEncoderAO::BaseComponentProxyFillThisBuffer;

#else
    iPVCapabilityFlags.iIsOMXComponentMultiThreaded = OMX_FALSE;

    iOmxComponent.SendCommand			= OmxComponentEvrcEncoderAO::BaseComponentSendCommand;
    iOmxComponent.GetParameter			= OmxComponentEvrcEncoderAO::BaseComponentGetParameter;
    iOmxComponent.SetParameter			= OmxComponentEvrcEncoderAO::BaseComponentSetParameter;
    iOmxComponent.GetConfig				= OmxComponentEvrcEncoderAO::BaseComponentGetConfig;
    iOmxComponent.SetConfig				= OmxComponentEvrcEncoderAO::BaseComponentSetConfig;
    iOmxComponent.GetExtensionIndex		= OmxComponentEvrcEncoderAO::BaseComponentGetExtensionIndex;
    iOmxComponent.GetState				= OmxComponentEvrcEncoderAO::BaseComponentGetState;
    iOmxComponent.UseBuffer				= OmxComponentEvrcEncoderAO::BaseComponentUseBuffer;
    iOmxComponent.AllocateBuffer		= OmxComponentEvrcEncoderAO::BaseComponentAllocateBuffer;
    iOmxComponent.FreeBuffer			= OmxComponentEvrcEncoderAO::BaseComponentFreeBuffer;
    iOmxComponent.EmptyThisBuffer		= OmxComponentEvrcEncoderAO::BaseComponentEmptyThisBuffer;
    iOmxComponent.FillThisBuffer		= OmxComponentEvrcEncoderAO::BaseComponentFillThisBuffer;
#endif

    iOmxComponent.SetCallbacks = OmxComponentEvrcEncoderAO::BaseComponentSetCallbacks;
    iOmxComponent.nVersion.s.nVersionMajor	= SPECVERSIONMAJOR;
    iOmxComponent.nVersion.s.nVersionMinor	= SPECVERSIONMINOR;
    iOmxComponent.nVersion.s.nRevision		= SPECREVISION;
    iOmxComponent.nVersion.s.nStep			= SPECSTEP;

    // PV capability
    iPVCapabilityFlags.iOMXComponentSupportsExternalInputBufferAlloc	= OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsExternalOutputBufferAlloc	= OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsMovableInputBuffers			= OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentSupportsPartialFrames				= OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentUsesNALStartCodes					= OMX_FALSE;
    iPVCapabilityFlags.iOMXComponentCanHandleIncompleteFrames			= OMX_TRUE;
    iPVCapabilityFlags.iOMXComponentUsesFullAVCFrames					= OMX_FALSE;

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

    /* Domain specific section for the ports */
    /* Input port is raw/pcm for EVRC encoder */
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.eDir									= OMX_DirInput;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.eDomain								= OMX_PortDomainAudio;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nPortIndex								= OMX_PORT_INPUTPORT_INDEX;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.eEncoding					= OMX_AUDIO_CodingPCM;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.cMIMEType					= (OMX_STRING)"raw";
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.pNativeRender				= 0;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.format.audio.bFlagErrorConcealment		= OMX_FALSE;
    
    //Set to a default value, will change later during setparameter call
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.bEnabled								= OMX_TRUE;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.bPopulated								= OMX_FALSE;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferSize							= INPUT_BUFFER_SIZE_EVRC_ENC;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferCountMin						= MIN_BUFFERCOUNT;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferCountActual						= NUMBER_INPUT_BUFFER_EVRC_ENC;
    
    /* Output port is evrc format for EVRC encoder */
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.eDir									= OMX_DirOutput;
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.eDomain								= OMX_PortDomainAudio;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nPortIndex							= OMX_PORT_OUTPUTPORT_INDEX;
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.eEncoding				= OMX_AUDIO_CodingEVRC;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.cMIMEType				= (OMX_STRING)"audio/mpeg";
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.pNativeRender			= 0;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.format.audio.bFlagErrorConcealment	= OMX_FALSE;
    
    //Set to a default value, will change later during setparameter call
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.bEnabled								= OMX_TRUE;
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.bPopulated							= OMX_FALSE;
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferSize							= OUTPUT_BUFFER_SIZE_EVRC_ENC;
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferCountMin						= MIN_BUFFERCOUNT;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->PortParam.nBufferCountActual					= NUMBER_OUTPUT_BUFFER_EVRC_ENC;
   
    //Default values for PCM input audio param port
	// changed according to AudioPcmMode structure     
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.eNumData							= OMX_NumericalDataSigned;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.ePCMMode							= OMX_AUDIO_PCMModeLinear;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.nChannels							= NUM_CHANNELS_2;
	ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.nPortIndex							= OMX_PORT_INPUTPORT_INDEX;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.bInterleaved						= OMX_TRUE;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.nBitPerSample						= BITS_PER_SAMPLE_16;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.nSamplingRate						= SAMPLING_RATE8K;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.eChannelMapping[0]					= OMX_AUDIO_ChannelLF;
    ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode.eChannelMapping[1]					= OMX_AUDIO_ChannelRF;

    //Default values for EVRC output audio param port
	// changed according to AudioEvrcParam structure 
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioEvrcParam.nChannels						= NUM_CHANNELS_2;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioEvrcParam.nPortIndex						= OMX_PORT_OUTPUTPORT_INDEX;
    ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioEvrcParam.nMinBitRate						= EVRC_MIN_FRAME_RATE;
	ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioEvrcParam.nMaxBitRate						= EVRC_MAX_FRAME_RATE;	

    iPortTypesParam.nPorts						= NUM_PORTS;
    iPortTypesParam.nStartPortNumber			= 0;

    pInPort = (ComponentPortType*) ipPorts[OMX_PORT_INPUTPORT_INDEX];
    pOutPort = (ComponentPortType*) ipPorts[OMX_PORT_OUTPUTPORT_INDEX];

    SetHeader(&pInPort->AudioParam, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
    pInPort->AudioParam.nPortIndex				= IN_PORTINDEX;
    pInPort->AudioParam.nIndex					= 0;
    pInPort->AudioParam.eEncoding				= OMX_AUDIO_CodingPCM;

    SetHeader(&pOutPort->AudioParam, sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE));
    pOutPort->AudioParam.nPortIndex				= OUT_PORTINDEX;
    pOutPort->AudioParam.nIndex					= 0;
    pOutPort->AudioParam.eEncoding				= OMX_AUDIO_CodingEVRC;

    iInputBufferRemainingBytes					= 0;

    if (ipEvrcEnc)
    {
        OSCL_DELETE(ipEvrcEnc);
        ipEvrcEnc = NULL;
    }

    ipEvrcEnc = OSCL_NEW(OmxEvrcEncoder, ());
    if (NULL == ipEvrcEnc)
    {
        return OMX_ErrorInsufficientResources;
    }


#if PROXY_INTERFACE

    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSendCommand			= BaseComponentSendCommand;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetParameter		= BaseComponentGetParameter;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSetParameter		= BaseComponentSetParameter;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetConfig			= BaseComponentGetConfig;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentSetConfig			= BaseComponentSetConfig;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetExtensionIndex	= BaseComponentGetExtensionIndex;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentGetState			= BaseComponentGetState;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentUseBuffer			= BaseComponentUseBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentAllocateBuffer		= BaseComponentAllocateBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentFreeBuffer			= BaseComponentFreeBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentEmptyThisBuffer		= BaseComponentEmptyThisBuffer;
    ((ProxyApplication_OMX*)ipComponentProxy)->ComponentFillThisBuffer		= BaseComponentFillThisBuffer;

#endif
    return OMX_ErrorNone;
}


/*	This function is called by the omx core when the component
	is disposed by the IL client with a call to FreeHandle().
	param Component, the component to be disposed.
*/

OMX_ERRORTYPE OmxComponentEvrcEncoderAO::DestroyComponent()
{
    if (iIsInit != OMX_FALSE)
    {
        ComponentDeInit();
    }

    //Destroy the base class now
    DestroyBaseComponent();

    if (ipEvrcEnc)
    {
        OSCL_DELETE(ipEvrcEnc);
        ipEvrcEnc = NULL;
    }

    if (ipAppPriv)
    {
        ipAppPriv->CompHandle = NULL;

        oscl_free(ipAppPriv);
        ipAppPriv = NULL;
    }

    return OMX_ErrorNone;
}



/* This routine will extract the input timestamp from the input buffer */
void OmxComponentEvrcEncoderAO::SyncWithInputTimestamp()
{
    iCurrentTimestamp = iFrameTimestamp;
}


void OmxComponentEvrcEncoderAO::ProcessData()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ProcessData IN"));

    QueueType* pInputQueue  = ipPorts[OMX_PORT_INPUTPORT_INDEX]->pBufferQueue;
    QueueType* pOutputQueue = ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->pBufferQueue;

    ComponentPortType* pInPort  = (ComponentPortType*) ipPorts[OMX_PORT_INPUTPORT_INDEX];
    ComponentPortType* pOutPort = ipPorts[OMX_PORT_OUTPUTPORT_INDEX];
    OMX_COMPONENTTYPE* pHandle  = &iOmxComponent;

    OMX_U8*	 pOutBuffer				= NULL;
    OMX_U32	 OutputLength			= 0;
    OMX_S32  EncodeReturn			= OMX_FALSE;
    OMX_U32  RemainderInputBytes	= 0;
    OMX_TICKS OutputTimeStamp		= 0;

    OMX_U32 TempInputBufferSize = (2 * sizeof(uint8) * (ipPorts[OMX_PORT_INPUTPORT_INDEX]->PortParam.nBufferSize));

    OMX_U32 AllocNumberOutputFrames;
    OMX_U32 ExtraInputBytes = 0;

    if ((!iIsInputBufferEnded) || iEndofStream)
    {
        //Check whether prev output bufer has been released or not
        if (OMX_TRUE == iNewOutBufRequired)
        {
            //Check whether a new output buffer is available or not
            if (0 == (GetQueueNumElem(pOutputQueue)))
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ProcessData OUT output buffer unavailable"));
                return;
            }

            ipOutputBuffer = (OMX_BUFFERHEADERTYPE*) DeQueue(pOutputQueue);

            OSCL_ASSERT(NULL != ipOutputBuffer);
            if (NULL == ipOutputBuffer)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "OmxComponentEvrcEncoderAO : ProcessData ERROR - OUT buffer cannot be dequeued"));

                return;
            }

            ipOutputBuffer->nFilledLen	= 0;
            iNewOutBufRequired			= OMX_FALSE;

            //At the first frame count, detrermine the output parameters
            if (0 == iFrameCount)
            {
                //How many frames can be accomodated in the output buffer
                AllocNumberOutputFrames = ipOutputBuffer->nAllocLen / MAX_EVRC_FRAME_SIZE;
                iActualNumberOutputFrames = omx_min(AllocNumberOutputFrames, iMaxNumberOutputFrames);

                /* Keep the minimum of the two:
                	-frames accomodated and
                	-maximum frames defined by component */
                iOutputFrameLength = iActualNumberOutputFrames * MAX_EVRC_FRAME_SIZE;
            }
        }

        /* Code for the marking buffer. Takes care of the OMX_CommandMarkBuffer
		   command and hMarkTargetComponent as given by the specifications.
         */
        if (ipMark != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent	= ipMark->hMarkTargetComponent;
            ipOutputBuffer->pMarkData				= ipMark->pMarkData;
            ipMark									= NULL;
        }

        if (ipTargetComponent != NULL)
        {
            ipOutputBuffer->hMarkTargetComponent	= ipTargetComponent;
            ipOutputBuffer->pMarkData				= iTargetMarkData;
            ipTargetComponent						= NULL;

        }
        //Mark buffer code ends here


        if ((iTempInputBufferLength > 0) &&
                ((iInputCurrLength + iTempInputBufferLength) <= TempInputBufferSize))
        {
            oscl_memcpy(&ipTempInputBuffer[iTempInputBufferLength], ipFrameDecodeBuffer, iInputCurrLength);
            iInputCurrLength		+= iTempInputBufferLength;
            iTempInputBufferLength	= 0;
            ipFrameDecodeBuffer		= ipTempInputBuffer;
        }


        //If the number of output buffers to be produced from the current iInputCurrLength
        //are more than our requirement, send only the required data for encoding
        if (iInputCurrLength / iInputFrameLength > iActualNumberOutputFrames)
        {
            ExtraInputBytes		= iInputCurrLength - (iInputFrameLength * iActualNumberOutputFrames);
            iInputCurrLength	-= ExtraInputBytes;
        }
        else
        {
            /* Before sending the input buffer to the encoder, ensure that the data is multiple
             * of one evrc input frame length*/
            RemainderInputBytes = iInputCurrLength % iInputFrameLength;
            iInputCurrLength -= RemainderInputBytes;
        }

        pOutBuffer		= &ipOutputBuffer->pBuffer[ipOutputBuffer->nFilledLen];
        OutputLength	= 0;
		 
		// Calling EVRC LSI Encoder function
        EncodeReturn	= ipEvrcEnc->EvrcEncodeAudio(pOutBuffer,
													&OutputLength,
													ipFrameDecodeBuffer,
													iInputCurrLength,
													iCurrentTimestamp,
													&OutputTimeStamp);

        //Attach the timestamp to the output buffer only when we have fetched the new output buffer
        //If we are reusing the same output buffer again, no need to modify the previous timestamp, as it should be of the first frame in that buffer

        if (0 == ipOutputBuffer->nFilledLen)
        {
            ipOutputBuffer->nTimeStamp = OutputTimeStamp;
        }


        ipOutputBuffer->nFilledLen	+= OutputLength;
        //offset not required in our case, set it to zero
        ipOutputBuffer->nOffset		= 0;

        //It has to be incremented atleast one time, so that 'equality with zero' checks added above hold true only once
        iFrameCount++;


        /* If EOS flag has come from the client & there are no more
           input buffers to decode, send the callback to the client
         */
        if (OMX_TRUE == iEndofStream)
        {
            if ((0 == iInputCurrLength) || (OMX_TRUE != EncodeReturn))
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ProcessData EOS callback send"));

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
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ProcessData OUT"));

                return;
            }
        }


        if (OMX_TRUE == EncodeReturn)
        {
            //Do not return the input buffer in case it has more than one frame data to encode
            if (ExtraInputBytes > 0)
            {
                ipFrameDecodeBuffer += iInputCurrLength;
                iInputCurrLength	=  ExtraInputBytes;
            }
            else
            {

                /* If there are some remainder bytes out of the last buffer, copy into a temp buffer
                 * to be used in next decode cycle and return the existing input buffer*/
                if (RemainderInputBytes > 0)
                {
                    oscl_memmove(ipTempInputBuffer, &ipFrameDecodeBuffer[iInputCurrLength], RemainderInputBytes);
                    iTempInputBufferLength = RemainderInputBytes;
                }

                //Input bytes consumed now, return the buffer
                ipInputBuffer->nFilledLen	= 0;
                ReturnInputBuffer(ipInputBuffer, pInPort);
                iIsInputBufferEnded			= OMX_TRUE;
                iInputCurrLength			= 0;

            }
        }
        //In case of error, discard the bitstream and report data corruption error via callback
        else
        {
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ProcessData ErrorStreamCorrupt callback send"));

            ipInputBuffer->nFilledLen = 0;
            ReturnInputBuffer(ipInputBuffer, pInPort);
            iIsInputBufferEnded = OMX_TRUE;
            iInputCurrLength = 0;


            (*(ipCallbacks->EventHandler))
            (pHandle,
             iCallbackData,
             OMX_EventError,
             OMX_ErrorStreamCorrupt,
             0,
             NULL);
        }


        /* Send the output buffer back when it has the following conditions as true:
        1) Output buffer contain desired number of frames, calculated above
        2) Output buffer can no longer hold the desired number of frames that
           we will encode in next encode call
        */
        if ((ipOutputBuffer->nFilledLen >= iOutputFrameLength)
                || (ipOutputBuffer->nAllocLen - ipOutputBuffer->nFilledLen) < iOutputFrameLength)
        {
            //Attach the end of frame flag while sending out the output buffer
            ipOutputBuffer->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
            ReturnOutputBuffer(ipOutputBuffer, pOutPort);
        }


        /* If there is some more processing left with current buffers, re-schedule the AO
         * Do not go for more than one round of processing at a time.
         * This may block the AO longer than required.
         */
        if (((ipInputBuffer->nFilledLen != 0) || (GetQueueNumElem(pInputQueue) > 0))
                && ((GetQueueNumElem(pOutputQueue) > 0) || (OMX_FALSE == iNewOutBufRequired)))
        {
            RunIfNotReady();
        }
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ProcessData OUT"));
    return;
}


//Not implemented & supported in case of base profile components

void OmxComponentEvrcEncoderAO::ComponentGetRolesOfComponent(OMX_STRING* aRoleString)
{
    *aRoleString = (OMX_STRING)"audio_encoder.evrc";
}


//Component constructor
OmxComponentEvrcEncoderAO::OmxComponentEvrcEncoderAO()
{
    ipEvrcEnc					= NULL;
    iInputFrameLength			= 0;
    iOutputFrameLength			= 0;
    iActualNumberOutputFrames	= 0;
    iMaxNumberOutputFrames		= 0;

    if (!IsAdded())
    {
        AddToScheduler();
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : constructed"));
}


//Active object destructor
OmxComponentEvrcEncoderAO::~OmxComponentEvrcEncoderAO()
{
    if (IsAdded())
    {
        RemoveFromScheduler();
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : destructed"));
}


/* The Initialization function */

OMX_ERRORTYPE OmxComponentEvrcEncoderAO::ComponentInit()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ComponentInit IN"));

    OMX_BOOL Status = OMX_TRUE;

    if (OMX_TRUE == iIsInit)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ComponentInit error incorrect operation"));
        return OMX_ErrorIncorrectStateOperation;
    }
    iIsInit = OMX_TRUE;

    //evrc encoder lib init
    if (!iCodecReady)
    {
        Status = ipEvrcEnc->EvrcEncInit(ipPorts[OMX_PORT_INPUTPORT_INDEX]->AudioPcmMode,
                                      ipPorts[OMX_PORT_OUTPUTPORT_INDEX]->AudioEvrcParam,
                                      &iInputFrameLength, &iMaxNumberOutputFrames);

        iCodecReady = OMX_TRUE;
    }	

    iInputCurrLength = 0;

    //Used in dynamic port reconfiguration
    iFrameCount = 0;

    if (OMX_TRUE == Status)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ComponentInit OUT"));
        return OMX_ErrorNone;
    }
    else
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : Error ComponentInit, OUT"));
        return OMX_ErrorInvalidComponent;
    }
}



/** This function is called upon a transition to the idle or invalid state.
 *  Also it is called by the ComponentDestructor() function
 */
OMX_ERRORTYPE OmxComponentEvrcEncoderAO::ComponentDeInit()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ComponentDeInit IN"));

    iIsInit = OMX_FALSE;

    if (iCodecReady)
    {
        ipEvrcEnc->EvrcEncDeinit();
        iCodecReady = OMX_FALSE;
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "OmxComponentEvrcEncoderAO : ComponentDeInit OUT"));

    return OMX_ErrorNone;

}


/* A component specific routine called from BufferMgmtWithoutMarker */
void OmxComponentEvrcEncoderAO::ProcessInBufferFlag()
{
    iIsInputBufferEnded = OMX_FALSE;
}
