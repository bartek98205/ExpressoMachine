/*
  ==============================================================================

    PitchDetector.cpp
    Created: 3 Sep 2024 9:57:16pm
    Author:  barto

  ==============================================================================
*/

#include "PitchDetector.h"

PitchDetector::PitchDetector() :
    detectFft(detectFftOrder),
    idxOfMaxMagnitude(0),
    currentMagnitude(0),
    max(0),
    bufferSize(960),
    mySampleRate(96000),
    preMagnitudeBuffer{},
    magnitudeBuffer{},
    firstHarmonic(880.0f),
    secondHarmonic(2200.0f)
{
    ResizeVectors();
}


void PitchDetector::ResizeVectors()
{
    preMagnitudeBuffer.resize(detectFftSize);
    magnitudeBuffer.resize(halfDetectFftSize);
}


void PitchDetector::ClearMagnitudeBuffer(std::vector <float>& myBuffer)
{
    std::fill(myBuffer.begin(), myBuffer.end(), 0.0f);
}

void PitchDetector::PushSamplesIntoBuffer(std::vector<float>& myBuffer, const float* channelData, int bufferSize)
{
    std::copy(channelData, channelData + bufferSize, myBuffer.begin());
}

void PitchDetector::PerformForwardFFT(std::vector<float>& myBuffer)
{
    detectFft.performRealOnlyForwardTransform(myBuffer.data(), false);
}

float PitchDetector::CalculateMagnitude(float real, float imag)
{
    return std::sqrt(std::pow(real, 2) + std::pow(imag, 2));
}

void PitchDetector::GetHarmonics(int rowOfFirstHarmonic, int rowOfSecondHarmonic, float* channelData, double toUseSampleRate)
{
    firstHarmonic = ProcessPitchDetection(channelData, toUseSampleRate) * rowOfFirstHarmonic;
    secondHarmonic = ProcessPitchDetection(channelData, toUseSampleRate) * rowOfSecondHarmonic;
}

void PitchDetector::GetMagnitudeBuffer(std::vector<float>& myBuffer, std::vector<float>& bufferToFill)
{
    int count = 0;
    const int stepSize = 2;

    for (auto it = myBuffer.begin(); it != myBuffer.end(); it += stepSize)
    {
        bufferToFill[count] = CalculateMagnitude(*it, *(it + 1));
        count++;
    }
}

void PitchDetector::GetMaxMagnitude(std::vector<float>& magnitudeBuffer)
{
    max = 0;
    idxOfMaxMagnitude = 0;
    currentMagnitude = 0;

    for (int s = 1; s < halfDetectFftSize; s++)
    {
        currentMagnitude = magnitudeBuffer[s];

        if (currentMagnitude > max)
        {
            idxOfMaxMagnitude = s;
            max = currentMagnitude;
        }
    }
}

float PitchDetector::GetFrequency(double sampleRate)
{
    return float(idxOfMaxMagnitude) * float(sampleRate) / detectFftSize;
}


float PitchDetector::ProcessPitchDetection(float* channelData, double toUseSampleRate)
{
    ClearMagnitudeBuffer(preMagnitudeBuffer);
    PushSamplesIntoBuffer(preMagnitudeBuffer, channelData, bufferSize);
    PerformForwardFFT(preMagnitudeBuffer);
    GetMagnitudeBuffer(preMagnitudeBuffer, magnitudeBuffer);
    GetMaxMagnitude(magnitudeBuffer);

    return GetFrequency(toUseSampleRate);
}
