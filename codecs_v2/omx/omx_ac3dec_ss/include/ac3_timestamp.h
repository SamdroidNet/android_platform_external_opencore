/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */
#ifndef AC3_TIMESTAMP_H_INCLUDED
#define AC3_TIMESTAMP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define DEFAULT_SAMPLING_FREQ_AC3 48000
#define DEFAULT_SAMPLES_PER_FRAME_AC3 1152

class Ac3TimeStampCalc
{
    public:

        Ac3TimeStampCalc()
        {
            iSamplingFreq = DEFAULT_SAMPLING_FREQ_AC3;
            iCurrentTs = 0;
            iCurrentSamples = 0;
            iSamplesPerFrame = DEFAULT_SAMPLES_PER_FRAME_AC3;
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

#endif	//#ifndef AC3_TIMESTAMP_H_INCLUDED
