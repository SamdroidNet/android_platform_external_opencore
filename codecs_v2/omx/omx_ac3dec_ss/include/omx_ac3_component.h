/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 * -------------------------------------------------------------------
 */
/**
	@file src/base/ac3_component.h

	OpenMax base_component component. This component does not perform any multimedia
	processing.	It is used as a base_component for new components development.

*/

#ifndef OMX_AC3_COMPONENT_H_INCLUDED
#define OMX_AC3_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef AC3_DEC_H_INCLUDED
#include "ac3_dec.h"
#endif

#ifndef AC3_TIMESTAMP_H_INCLUDED
#include "ac3_timestamp.h"
#endif

#if 1
#define INPUT_BUFFER_SIZE_AC3 1024*100
#define OUTPUT_BUFFER_SIZE_AC3 4608

#define NUMBER_INPUT_BUFFER_AC3   5
#define NUMBER_OUTPUT_BUFFER_AC3  4

#else
#define INPUT_BUFFER_SIZE_AC3 4000
#define OUTPUT_BUFFER_SIZE_AC3 4608

#define NUMBER_INPUT_BUFFER_AC3  10
#define NUMBER_OUTPUT_BUFFER_AC3  9
#endif



class OpenmaxAc3AO : public OmxComponentAudio
{
    public:

        OpenmaxAc3AO();
        ~OpenmaxAc3AO();

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

        OmxAc3Decoder* 	 ipAc3Dec;
        Ac3TimeStampCalc iCurrentFrameTS;
};

#endif // OMX_AC3_COMPONENT_H_INCLUDED
