/*
  ==============================================================================

    Blend.cpp
    Created: 3 Sep 2024 8:40:51pm
    Author:  barto

  ==============================================================================
*/

#include "Blend.h"

Blend::Blend(float maxBlendValue, float minBlendValue, float targetValue) 
    :m_maxBlendValue(maxBlendValue), 
    m_minBlendValue(minBlendValue),
    m_targetBlandValue(targetValue)
{}

float Blend::GetDryWetRatio(float recentRMS, float maxRMSValue, float minRMSValue)
{
    return abs(((m_targetBlandValue - m_minBlendValue) / (maxRMSValue - minRMSValue)) * (recentRMS - minRMSValue) + m_minBlendValue);
}
