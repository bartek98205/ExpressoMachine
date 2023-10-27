/*
  ==============================================================================

    Blend.h
    Created: 26 Oct 2023 12:24:57pm
    Author:  barto

  ==============================================================================
*/
#pragma once
#include <cmath>
#include <map>
#include <string.h>
#include "RMS.h"

class BlendClass : protected RMSClass
{
public:



    BlendClass()
    {}

    float dryWetRatio(float recentRms, float minBlendValue, float targetBlendValue)
    {
        float ratio = ((targetBlendValue - minBlendValue) / (RMSClass::maxRMS - RMSClass::minRMS)) * (recentRms - RMSClass::minRMS) + minBlendValue;

        ratio = std::abs(ratio);
        return ratio;
    }

};
