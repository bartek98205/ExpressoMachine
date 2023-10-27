
#pragma once
#include <vector>
#include <numeric>
#include <cmath>

class TestClass
{
public:
	
	double  currentAngle = 0.0, angleDelta = 0.0;
    std::vector <float> sinWave;


    void testSin(int bufferSize, int sampleRate, float freq, float amplitude)
    {
        sinWave.resize(bufferSize);
        auto cyclesPerSample = freq / sampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;

        for (int s = 0; s < sinWave.size(); s++)
        {
            sinWave[s] = (float)std::sin(currentAngle);
            currentAngle += angleDelta;
        }
    }

    void pushSinToBuffer(float* channelData, int bufferSize)
    {

        for (int s = 0; s < bufferSize; s++)
        {
            channelData[s] = sinWave[s];

        }
    }
};