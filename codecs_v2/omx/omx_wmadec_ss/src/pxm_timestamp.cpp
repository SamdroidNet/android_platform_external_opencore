/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * -------------------------------------------------------------------
 */

#include "pxm_timestamp.h"


//Initialize the parameters
void PxmTimeStampCalc::SetParameters(uint32 aFreq, uint32 aSamples)
{
    if (0 != aFreq)
    {
        iSamplingFreq = aFreq;
    }

    iSamplesPerFrame = aSamples;
}


//Set the current timestamp equal to the input buffer timestamp
void PxmTimeStampCalc::SetFromInputTimestamp(uint32 aValue)
{
    iCurrentTs = aValue;
    iCurrentSamples = 0;
}


void PxmTimeStampCalc::UpdateTimestamp(uint32 aValue)
{
    iCurrentSamples += aValue;
}

//Convert current samples into the output timestamp
uint32 PxmTimeStampCalc::GetConvertedTs()
{
    uint32 Value = iCurrentTs;

    //Formula used: TS in ms = (samples * 1000/sampling_freq)
    //Rounding added (add 0.5 to the result), extra check for divide by zero
    if (0 != iSamplingFreq)
    {
        Value = iCurrentTs + (iCurrentSamples * 1000 + (iSamplingFreq / 2)) / iSamplingFreq;
    }

    iCurrentTs = Value;
    iCurrentSamples = 0;

    return (Value);
}

/* Do not update iCurrentTs value, just calculate & return the current timestamp */
uint32 PxmTimeStampCalc::GetCurrentTimestamp()
{
    uint32 Value = iCurrentTs;

    if (0 != iSamplingFreq)
    {
        Value = iCurrentTs + (iCurrentSamples * 1000 + (iSamplingFreq / 2)) / iSamplingFreq;
    }

    return (Value);
}

//Calculate the duration in ms of single frame
uint32 PxmTimeStampCalc::GetFrameDuration()
{
    uint32 Value = 0;

    if (0 != iSamplingFreq)
    {
        Value = (iSamplesPerFrame * 1000 + (iSamplingFreq / 2)) / iSamplingFreq;
    }

    return (Value);
}
