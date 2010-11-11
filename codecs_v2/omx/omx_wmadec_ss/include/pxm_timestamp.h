/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#ifndef PXM_TIMESTAMP_H_INCLUDED
#define PXM_TIMESTAMP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define DEFAULT_SAMPLING_FREQ_PXM 48000
#define DEFAULT_SAMPLES_PER_FRAME_PXM 1152

class PxmTimeStampCalc
{
    public:

        PxmTimeStampCalc()
        {
            iSamplingFreq = DEFAULT_SAMPLING_FREQ_PXM;
            iCurrentTs = 0;
            iCurrentSamples = 0;
            iSamplesPerFrame = DEFAULT_SAMPLES_PER_FRAME_PXM;
        };

        void SetParameters(uint32 aFreq, uint32 aSamples);

        void SetFromInputTimestamp(uint32 aValue);

        void UpdateTimestamp(uint32 aValue);

        uint32 GetConvertedTs();

        uint32 GetCurrentTimestamp();

        uint32 GetFrameDuration();

    private:
        uint32 iSamplingFreq;
        uint32 iCurrentTs;
        uint32 iCurrentSamples;
        uint32 iSamplesPerFrame;
};

#endif	//#ifndef PXM_TIMESTAMP_H_INCLUDED
