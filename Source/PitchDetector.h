/*
  ==============================================================================

    PitchDetector.h
    Created: 3 Sep 2024 9:57:16pm
    Author:  barto

  ==============================================================================
*/

#pragma once
#include <vector>
#include "juce_dsp\juce_dsp.h"

class PitchDetector
{
private:
    juce::dsp::FFT detectFft;
    static constexpr auto detectFftOrder = 12, detectFftSize = 1 << detectFftOrder, halfDetectFftSize = detectFftSize / 2;
    int idxOfMaxMagnitude;
    float currentMagnitude, max, firstHarmonic, secondHarmonic;
    const int bufferSize;
    double mySampleRate;
    std::vector<float> preMagnitudeBuffer;
    std::vector<float> magnitudeBuffer;

public:
    PitchDetector ();

    void ResizeVectors ();

    void ClearMagnitudeBuffer (std::vector <float>& myBuffer);

    void PushSamplesIntoBuffer(std::vector <float>& myBuffer, const float* channelData, int bufferSize);

    void PerformForwardFFT(std::vector <float>& myBuffer);

    float CalculateMagnitude(float real, float imag);

    void GetHarmonics(int rowOfFirstHarmonic, int rowOfSecondHarmonic, float* channelData, double toUseSampleRate);

    void GetMagnitudeBuffer(std::vector <float>& myBuffer, std::vector <float>& bufferToFill);

    void GetMaxMagnitude(std::vector <float>& magnitudeBuffer);

    float GetFrequency(double sampleRate);

    float ProcessPitchDetection(float* channelData, double toUseSampleRate);

};