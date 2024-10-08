/*
  ==============================================================================

    PitchShifterEffect.cpp
    Created: 4 Sep 2024 2:28:45pm
    Author:  barto

  ==============================================================================
*/

#include "PitchShifterEffect.h"
using namespace std::complex_literals;

PitchShifterEffect::PitchShifterEffect()
    :m_pitchFFT(fftOrder),
    lastSamplesBuffer{},
    gInputBuffer{},
    gOutputBuffer{},
    unwrappedBuffer{},
    fftUnwrappedBuffer{},
    postFftUnwrappedBuffer{},
    gSynthesisWindowBuffer{},
    gAnalysisWindowBuffer{},
    latance(0),
    lastSamplesWritePointer(0),
    lastSamplesReadPointer(0),
    gOutputBufferReadPointer(0),
    gInputBufferPointer(0),
    gBufferSize(1000),
    gHopCounter(0),
    gOutputBufferWritePointer(0),
    gScaleFactor(0.5),
    M_PI(juce::MathConstants<float>::pi),
    M_PI2(juce::MathConstants<float>::twoPi),
    gPitchShift(0.5),
    gHopSize(gFftSize / 8),
    gCachedInputBufferPointer(0)
{
    VectorResize();
    HannWindow();
    ClearVectors();
}

PitchShifterEffect::~PitchShifterEffect()
{
}

void PitchShifterEffect::VectorResize()
{
    latance = 572;
    gInputBuffer.resize(gBufferSize);
    gOutputBuffer.resize(gBufferSize);
    unwrappedBuffer.resize(gFftSize);
    gAnalysisWindowBuffer.resize(gFftSize);
    gSynthesisWindowBuffer.resize(gFftSize);
    fftUnwrappedBuffer.resize(gFftSize);
    postFftUnwrappedBuffer.resize(gFftSize);
    lastSamplesBuffer.resize(gBufferSize);
    gOutputBufferWritePointer = gFftSize + 2 * gHopSize;
    lastSamplesWritePointer = latance;
}

void PitchShifterEffect::HannWindow()
{
    for (int n = 0; n < gFftSize; n++)
    {
        gAnalysisWindowBuffer[n] = 0.5f * (1.0f - cosf(2.0 * M_PI * n / (float)(gFftSize - 1)));
        gSynthesisWindowBuffer[n] = gAnalysisWindowBuffer[n];
    }
}

void PitchShifterEffect::ClearVectors()
{
    for (int n = 0; n < gFftSize; n++) {
        unwrappedBuffer[n].real(0), unwrappedBuffer[n].imag(0);
        fftUnwrappedBuffer[n].real(0), fftUnwrappedBuffer[n].imag(0);
        postFftUnwrappedBuffer[n].real(0), postFftUnwrappedBuffer[n].imag(0);
    }

    gInputBuffer.clear();
    gOutputBuffer.clear();
    lastSamplesBuffer.clear();
}

double PitchShifterEffect::GetMagnitude(std::complex<float> complexNumber)
{
    return std::sqrt(std::pow(complexNumber.real(), 2) + std::pow(complexNumber.imag(), 2));
}

float PitchShifterEffect::WrapPhase(float phaseIn)
{
    if (phaseIn >= 0)
        return fmodf(phaseIn + M_PI, 2.0 * M_PI) - M_PI;
    else
        return fmodf(phaseIn - M_PI, -2.0 * M_PI) + M_PI;
}

void PitchShifterEffect::PerformPitchShifting(juce::AudioBuffer<float>& bufferToPitch, float* channelDataToPitch, int channelToPitch, float cleanBlend, float gainBlend)
{
    for (int sample = 0; sample < bufferToPitch.getNumSamples(); sample++)
    {
        lastSamplesBuffer[lastSamplesWritePointer] = channelDataToPitch[sample];
        lastSamplesWritePointer++;

        if (lastSamplesWritePointer >= gBufferSize)
        {
            lastSamplesWritePointer = 0;
        }

        float in = channelDataToPitch[sample];
        gInputBuffer[gInputBufferPointer++] = in;

        if (gInputBufferPointer >= gBufferSize)
        {

            gInputBufferPointer = 0;
        }

        float out = gOutputBuffer[gOutputBufferReadPointer];
        gOutputBuffer[gOutputBufferReadPointer] = 0;
        out *= gScaleFactor;

        gOutputBufferReadPointer++;
        if (gOutputBufferReadPointer >= gBufferSize)
            gOutputBufferReadPointer = 0;

        if (++gHopCounter >= gHopSize)
        {
            gHopCounter = 0;
            gCachedInputBufferPointer = gInputBufferPointer;
            static std::vector<float> lastInputPhases(gFftSize);
            static std::vector<float> lastOutputPhases(gFftSize);
            static std::vector<float> analysisMagnitudes(gFftSize / 2 + 1);
            static std::vector<float> analysisFrequencies(gFftSize / 2 + 1);
            static std::vector<float> synthesisMagnitudes(gFftSize / 2 + 1);
            static std::vector<float> synthesisFrequencies(gFftSize / 2 + 1);

            for (int i = 0; i < gFftSize; i++)
            {
                int circularBufferIndex = (gCachedInputBufferPointer + i - gFftSize + gBufferSize) % gBufferSize;
                unwrappedBuffer[i].real(gInputBuffer[circularBufferIndex] * gAnalysisWindowBuffer[i]);
                unwrappedBuffer[i].imag(0);
            }

            m_pitchFFT.perform(unwrappedBuffer.data(), fftUnwrappedBuffer.data(), false);

            for (int n = 0; n <= gFftSize / 2; n++)
            {
                float amplitude = abs(fftUnwrappedBuffer[n]);
                float phase = arg(fftUnwrappedBuffer[n]);
                float magnitude = GetMagnitude(fftUnwrappedBuffer[n]);
                float phaseDiff = phase - lastInputPhases[n];
                float binCentreFrequency = 2.0 * M_PI * (float)n / (float)gFftSize;

                phaseDiff = WrapPhase(phaseDiff - binCentreFrequency * gHopSize);

                float binDeviation = phaseDiff * (float)gFftSize / (float)gHopSize / (2.0 * M_PI);

                analysisFrequencies[n] = (float)n + binDeviation;
                analysisMagnitudes[n] = amplitude;
                lastInputPhases[n] = phase;
            }

            for (int n = 0; n <= gFftSize / 2; n++)
            {
                synthesisMagnitudes[n] = synthesisFrequencies[n] = 0;
            }

            for (int n = 0; n <= gFftSize / 2; n++)
            {
                int newBin = floorf(n * gPitchShift + 0.5);
                if (newBin <= gFftSize / 2) {
                    synthesisMagnitudes[newBin] += analysisMagnitudes[n];
                    synthesisFrequencies[newBin] = analysisFrequencies[n] * gPitchShift;
                }
            }

            for (int n = 0; n <= gFftSize / 2; n++)
            {
                double amplitude = synthesisMagnitudes[n];
                float binDevation = synthesisFrequencies[n] - n;
                float phaseDiff = binDevation * 2.0 * M_PI * (float)gHopSize / (float)gFftSize;
                float binCentreFrequency = 2.0 * M_PI * (float)n / (float)gFftSize;

                phaseDiff += binCentreFrequency * gHopSize;

                double outPhase = WrapPhase(lastOutputPhases[n] + phaseDiff);
                fftUnwrappedBuffer[n] = amplitude * cosf(outPhase) + (amplitude * sinf(outPhase) * 1i);

                if (n > 0 && n < gFftSize / 2)
                {
                    fftUnwrappedBuffer[gFftSize - n].real(fftUnwrappedBuffer[n].real());
                    fftUnwrappedBuffer[gFftSize - n].imag(-(fftUnwrappedBuffer[n].imag()));
                }
                lastOutputPhases[n] = outPhase;
            }

            m_pitchFFT.perform(fftUnwrappedBuffer.data(), postFftUnwrappedBuffer.data(), true);

            for (int n = 0; n < gFftSize; n++)
            {
                int circularBufferIndex = (gCachedInputBufferPointer + n - gFftSize + gBufferSize) % gBufferSize;
                gOutputBuffer[circularBufferIndex] += postFftUnwrappedBuffer[n].real() * gSynthesisWindowBuffer[n];
            }
            gOutputBufferWritePointer = (gOutputBufferWritePointer + gHopSize) % gBufferSize;
        }

        //channelDataToPitch[sample] = myGain.basicGains(((lastSamplesBuffer[lastSamplesReadPointer]) + (out * cleanBlend)), 1, gainBlend);
        channelDataToPitch[sample] = lastSamplesBuffer[lastSamplesReadPointer];

        lastSamplesReadPointer++;
        if (lastSamplesReadPointer >= gBufferSize)
        {
            lastSamplesReadPointer = 0;
        }
    }
}

