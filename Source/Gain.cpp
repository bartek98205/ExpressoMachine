/*
  ==============================================================================

    Gain.cpp
    Created: 3 Sep 2024 9:27:38pm
    Author:  barto

  ==============================================================================
*/

#include "Gain.h"
#include <cmath>

float Gain::Mod(float n, float d)
{
    n = fmod(n, d);
    if (n < 0) n += d;
    return n;
}

float Gain::BasicGains(float sample, TypeOfGain type, int gainFactor)
{
    if (type == hard)
        return sample = mod(gainFactor * sample + 1, 2) - 1;

    if (type == soft)
        return sample = tanh(gainFactor * sample);

}
