/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
/**
	@file omx_g729enc_component.h
	OpenMax encoder component file.
*/

#ifndef OMX_G729ENC_COMPONENT_H_INCLUDED
#define OMX_G729ENC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef G729_DEC_H_INCLUDED
#include "g729_enc.h"
#endif


#define INPUT_BUFFER_SIZE_G729_ENC (160 * MAX_NUM_OUTPUT_FRAMES_PER_BUFFER)	//3200

#define OUTPUT_BUFFER_SIZE_G729_ENC (MAX_NUM_OUTPUT_FRAMES_PER_BUFFER * MAX_G729_FRAME_SIZE)	//10 * 160 = 1600

#define NUMBER_INPUT_BUFFER_G729_ENC  5
#define NUMBER_OUTPUT_BUFFER_G729_ENC  2


#define omx_min(a, b)  ((a) <= (b) ? (a) : (b));

class OmxComponentG729EncoderAO : public OmxComponentAudio
{
    public:

        OmxComponentG729EncoderAO();
        ~OmxComponentG729EncoderAO();


        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

        void ProcessData();

        void ProcessInBufferFlag();
        void SyncWithInputTimestamp();

    private:

        OMX_U32			iInputFrameLength;
        OMX_U32			iMaxNumberOutputFrames;
        OMX_U32			iActualNumberOutputFrames;

        OMX_TICKS		iCurrentTimestamp;
        OmxG729Encoder* ipG729Enc;
};

#endif // OMX_G729ENC_COMPONENT_H_INCLUDED
