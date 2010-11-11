
/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 * -------------------------------------------------------------------
 */
/**
	@file src/base/g711_component.h

	OpenMax base_component component. This component does not perform any multimedia
	processing.	It is used as a base_component for new components development.

*/

#ifndef OMX_G711_COMPONENT_H_INCLUDED
#define OMX_G711_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef G711_DEC_H_INCLUDED
#include "g711_dec.h"
#endif

#ifndef G711_TIMESTAMP_H_INCLUDED
#include "g711_timestamp.h"
#endif

#define INPUT_BUFFER_SIZE_G711 160
#define OUTPUT_BUFFER_SIZE_G711 320

#define NUMBER_INPUT_BUFFER_G711  10
#define NUMBER_OUTPUT_BUFFER_G711  9


class OpenmaxG711AO : public OmxComponentAudio
{
    public:

        OpenmaxG711AO();
        ~OpenmaxG711AO();

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
        OmxG711Decoder* 	 ipG711Dec;
        G711TimeStampCalc iCurrentFrameTS;
};

#endif // OMX_G711_COMPONENT_H_INCLUDED
