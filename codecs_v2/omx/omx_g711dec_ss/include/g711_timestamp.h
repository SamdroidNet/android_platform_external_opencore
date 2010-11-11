/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

 #ifndef G711_TIMESTAMP_H_INCLUDED
#define G711_TIMESTAMP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define DEFAULT_SAMPLING_FREQ_G711 8000
#define DEFAULT_SAMPLES_PER_FRAME_G711 160

class G711TimeStampCalc
{
    public:

        G711TimeStampCalc()
        {
            iSamplingFreq = DEFAULT_SAMPLING_FREQ_G711;
            iCurrentTs = 0;
            iCurrentSamples = 0;
            iSamplesPerFrame = DEFAULT_SAMPLES_PER_FRAME_G711;
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

#endif	//#ifndef G711_TIMESTAMP_H_INCLUDED
