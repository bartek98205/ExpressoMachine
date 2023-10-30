
#pragma once

#include "Blend.h"

class GainClass: protected BlendClass
{
public:
    GainClass()
    {}

   float mod(float n, float d)
    {
        n = fmod(n, d);
        if (n < 0) n += d;
        return n;
    }

   float basicGains(float sample, int typeOfGain, float gainFactor)
   {
        if (typeOfGain == 1)
        {
            return sample = mod(gainFactor * sample + 1, 2) - 1;
        }

        if (typeOfGain == 2)
        {
           return sample = tanh(gainFactor * sample);
        }
   }
};
