/*
  ==============================================================================

    Blend.h
    Created: 3 Sep 2024 8:40:51pm
    Author:  barto

  ==============================================================================
*/

#pragma once
#include "RMS.h"

/**
* @class Blend
     * @brief This class is responsible for calculating
     * the ratio between a clean, unmodified sound and a modified
     * version of that sound based on RMS values.
 */


class Blend 
{
    /**
        * @brief Constructor to initialize Blend class with specified maximum and minimum blend values.
        *
        * @param maxBlandValue The maximum value used for blending calculations.
        * @param minBlandValue The minimum value used for blending calculations.
        * @param targetValue The new target blend value to set. This value determines the
        *  maximum level of blending that can occur between the clean and
        *  modified signals.
    */

    Blend(float maxBlendValue, float minBlendValue, float targetValue) {};

    ~Blend() {};

    /**
         * @brief Calculate the dry/wet ratio based on recent RMS and given bounds.
         *
         *  * @param recentRMS The RMS value of the most recent audio buffer.
         * @param maxRMSValue The maximum RMS value observed.
         * @param minRMSValue The minimum RMS value observed.
         * @return A float representing the dry/wet ratio, where a higher value indicates more
         *         of the wet (modified) signal.
     */
    float GetDryWetRatio(float recentRMS, float maxRMSValue, float minRMSValue);

private:
    float m_minBlendValue;
    float m_maxBlendValue;
    float m_targetBlandValue;
};