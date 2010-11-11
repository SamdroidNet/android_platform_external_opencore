/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name	: EVRC Encoder
 * @File Name			: omx_evrcenc_component.h
 * @File Description	: OpenMax encoder component header file.
 * @Author				: Sudhir Vyas				 
 * @Created Date		: 16-06-2009		     
 * @Modification History
 * Version:				Date:				By:				Change:		
 * -------------------------------------------------------------------
 */
/**
	@file omx_evrcenc_component.h
	OpenMax encoder component file.
*/

#ifndef OMX_EVRC_COMPONENT_H_INCLUDED
#define OMX_EVRC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef EVRC_ENC_H_INCLUDED
#include "evrc_enc.h"
#endif

// set according to evrc encoder parameters
#define INPUT_BUFFER_SIZE_EVRC_ENC				EVRC_ONE_INPUTFRAME_DEFAULT_LENGTH * MAX_NUM_OUTPUT_FRAMES_PER_BUFFER 
#define OUTPUT_BUFFER_SIZE_EVRC_ENC				MAX_NUM_OUTPUT_FRAMES_PER_BUFFER * MAX_EVRC_FRAME_SIZE
#define NUMBER_INPUT_BUFFER_EVRC_ENC			1
#define NUMBER_OUTPUT_BUFFER_EVRC_ENC			1

#define NUM_PORTS								2

#define IN_PORTINDEX							0
#define OUT_PORTINDEX							1

#define MIN_BUFFERCOUNT							1

#define BITS_PER_SAMPLE_16						16

#define omx_min(a, b)  ((a) <= (b) ? (a) : (b));

class OmxComponentEvrcEncoderAO : public OmxComponentAudio
{
    public:

        OmxComponentEvrcEncoderAO();
        ~OmxComponentEvrcEncoderAO();


        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

        void ProcessData();

        void ProcessInBufferFlag();
        void SyncWithInputTimestamp();

    private:

        OMX_U32			 iInputFrameLength;
        OMX_U32			 iMaxNumberOutputFrames;
        OMX_U32			 iActualNumberOutputFrames;

        OMX_TICKS		 iCurrentTimestamp;
        OmxEvrcEncoder*  ipEvrcEnc;
};

#endif // OMX_EVRCENC_COMPONENT_H_INCLUDED
