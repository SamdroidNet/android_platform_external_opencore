/* ----------------------------------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name    : OMX MP3 Encoder.
 * @File Name              : omx_mp3enc_component.h
 * @File Description       : OpenMax encoder component file.
 * @Author                 : Sudhir Vyas, Renuka V
 * @Created Date           : 05-06-2009
 * @Modification History
 * Version:            Date:             By:                Change:
 * -------------------------------------------------------------------------------------------------
 */

#ifndef OMX_MP3ENC_COMPONENT_H_INCLUDED
#define OMX_MP3ENC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef MP3_ENC_H_INCLUDED
#include "mp3_enc.h"
#endif

// Set according to encoder parameters

#define INPUT_BUFFER_SIZE_MP3_ENC       MP3_ONE_INPUTFRAME_DEFAULT_LENGTH * MAX_NUM_OUTPUT_FRAMES_PER_BUFFER           
#define OUTPUT_BUFFER_SIZE_MP3_ENC      MAX_MP3_FRAME_SIZE *MAX_NUM_OUTPUT_FRAMES_PER_BUFFER             

#define NUMBER_INPUT_BUFFER_MP3_ENC     5
#define NUMBER_OUTPUT_BUFFER_MP3_ENC    2
#define MIN_NUMBER_BUFFER_MP3_ENC       1


#define omx_min(a, b)  ((a) <= (b) ? (a) : (b));

class OmxComponentMp3EncoderAO : public OmxComponentAudio
{
    public:

        OmxComponentMp3EncoderAO();
        ~OmxComponentMp3EncoderAO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);
        void ProcessData();
        void ProcessInBufferFlag();
        void SyncWithInputTimestamp();

    private:

        OMX_U32          iInputFrameLength;
        OMX_U32          iMaxNumberOutputFrames;
        OMX_U32          iActualNumberOutputFrames;

        OMX_TICKS        iCurrentTimestamp;
        OmxMp3Encoder*   ipMp3Enc;
};
#endif // OMX_MP3ENC_COMPONENT_H_INCLUDED
