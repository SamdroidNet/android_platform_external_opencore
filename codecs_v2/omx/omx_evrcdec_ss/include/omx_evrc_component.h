/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 * -------------------------------------------------------------------
 */
/**
	@file src/base/g711_component.h

	OpenMax base_component component. This component does not perform any multimedia
	processing.	It is used as a base_component for new components development.

*/

#ifndef OMX_EVRC_COMPONENT_H_INCLUDED
#define OMX_EVRC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef EVRC_DEC_H_INCLUDED
#include "evrc_dec.h"
#endif

#ifndef EVRC_TIMESTAMP_H_INCLUDED
#include "evrc_timestamp.h"
#endif

#define INPUT_BUFFER_SIZE_EVRC 24
#define OUTPUT_BUFFER_SIZE_EVRC 320

#define NUMBER_INPUT_BUFFER_EVRC  10
#define NUMBER_OUTPUT_BUFFER_EVRC  9


class OpenmaxEvrcAO : public OmxComponentAudio
{
    public:

        OpenmaxEvrcAO();
        ~OpenmaxEvrcAO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);
        void ProcessData();
        void SyncWithInputTimestamp();
        void ProcessInBufferFlag();

        void ResetComponent();
        OMX_ERRORTYPE GetConfig(
        OMX_IN  OMX_HANDLETYPE hComponent,
        OMX_IN  OMX_INDEXTYPE nIndex,
        OMX_INOUT OMX_PTR pComponentConfigStructure);

    private:

        void CheckForSilenceInsertion();
        void DoSilenceInsertion();
        OMX_TICKS			 iCurrentTimestamp;
        OmxEvrcDecoder* 	 ipEvrcDec;
        EvrcTimeStampCalc iCurrentFrameTS;
};

#endif // OMX_EVRC_COMPONENT_H_INCLUDED
