
#pragma once

class RMSClass
{
public:
    float maxRMS;
    float minRMS;

    RMSClass() :
        maxRMS(0.4f),
        minRMS(0.000001f)
    {}

    void setMaxRMS(float recentRMS)
    {
        if (recentRMS > maxRMS)
        {
            maxRMS = recentRMS;
            DBG("MAXRMS");
            DBG(maxRMS);
        }
    }

    void setMinRMS(float recentRMS)
    {
        if (recentRMS < minRMS)
        {
            minRMS = recentRMS;
            DBG("minRMS");
            DBG(minRMS);
        }
    }

    void setBoundsOfRMS(float recentRMS)
    {
        setMinRMS(recentRMS);
        setMaxRMS(recentRMS);

    }

    float my_getRMS(int channel, juce::AudioBuffer<float>& rmsBuffer, int bufferSize)
    {
        return rmsBuffer.getRMSLevel(channel, 0, bufferSize);
    }
};