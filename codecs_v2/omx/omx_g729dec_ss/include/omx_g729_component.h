
/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 * -------------------------------------------------------------------
 */
/**
	@file src/base/g729_component.h

	OpenMax base_component component. This component does not perform any multimedia
	processing.	It is used as a base_component for new components development.

*/

#ifndef OMX_G729_COMPONENT_H_INCLUDED
#define OMX_G729_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef G729_DEC_H_INCLUDED
#include "g729_dec.h"
#endif

#ifndef G729_TIMESTAMP_H_INCLUDED
#include "g729_timestamp.h"
#endif

#define INPUT_BUFFER_SIZE_G729 10
#define OUTPUT_BUFFER_SIZE_G729 160

#define NUMBER_INPUT_BUFFER_G729  10
#define NUMBER_OUTPUT_BUFFER_G729  9


class OpenmaxG729AO : public OmxComponentAudio
{
    public:

        OpenmaxG729AO();
        ~OpenmaxG729AO();

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
        OmxG729Decoder* 	 ipG729Dec;
        G729TimeStampCalc iCurrentFrameTS;
};

#endif // OMX_G729_COMPONENT_H_INCLUDED
