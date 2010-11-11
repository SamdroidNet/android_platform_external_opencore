/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 * -------------------------------------------------------------------
 */
/**
	@file src/base/pxm_component.h

	OpenMax base_component component. This component does not perform any multimedia
	processing.	It is used as a base_component for new components development.

*/

#ifndef OMX_PXM_COMPONENT_H_INCLUDED
#define OMX_PXM_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef PXM_DEC_H_INCLUDED
#include "pxm_dec.h"
#endif

#ifndef PXM_TIMESTAMP_H_INCLUDED
#include "pxm_timestamp.h"
#endif

#if 1
#define INPUT_BUFFER_SIZE_PXM 4000*10
#define OUTPUT_BUFFER_SIZE_PXM 4608*4

#define NUMBER_INPUT_BUFFER_PXM  4
#define NUMBER_OUTPUT_BUFFER_PXM  3

#else
#define INPUT_BUFFER_SIZE_PXM 4000*2
#define OUTPUT_BUFFER_SIZE_PXM 4608*4

#define NUMBER_INPUT_BUFFER_PXM  10
#define NUMBER_OUTPUT_BUFFER_PXM  9
#endif


class OpenmaxPxmAO : public OmxComponentAudio
{
    public:

        OpenmaxPxmAO();
        ~OpenmaxPxmAO();

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

        OmxPxmDecoder* 	 ipPxmDec;
		OMX_S32 iTempOutLen;
        PxmTimeStampCalc iCurrentFrameTS;
};

#endif // OMX_PXM_COMPONENT_H_INCLUDED
