/*
  ==============================================================================

    PitchDetector.h
    Created: 24 Oct 2023 12:05:47pm
    Author:  barto

  ==============================================================================
*/

#pragma once



class PitchDetector
{
public:
    
    juce::dsp::FFT detect_fft;
    static constexpr auto detectFftOrder = 12, detectFftSize = 1 << detectFftOrder, halfDetectFftSize = detectFftSize / 2;
    int idxOfMaxMagnitude;
    float currentMagnitude, max, firstHarmonic, secondHarmonic;
    int bufferSize;
    double my_sampleRate;

    std::vector<float> preMagnitudeBuffer;
    std::vector<float> magnitudeBuffer;


    PitchDetector() :
        detect_fft(detectFftOrder),
        idxOfMaxMagnitude(0),
        currentMagnitude(0),
        max(0),
        bufferSize(960),
        my_sampleRate(96000),
        preMagnitudeBuffer{},
        magnitudeBuffer{},
        firstHarmonic(880.0f),
        secondHarmonic(2200.0f)
    
    {
        resizeVectors();
    }
   
    void resizeVectors()
    {
        preMagnitudeBuffer.resize(detectFftSize);
        magnitudeBuffer.resize(halfDetectFftSize);
    }

    void clearMagnitudeBuffer(std::vector <float>& my_buffer)
    {
        std::fill(my_buffer.begin(), my_buffer.end(), 0.0f);
    }
    
    void pushSamplesIntoBuffer(std::vector <float>& my_buffer, const float* channelData, int bufferSize)
    {
        for (int s = 0; s < bufferSize; s++)
        {
            my_buffer[s] = channelData[s];
        }
    }

    void performForwardFFT(std::vector <float>& my_buffer)
    {
        detect_fft.performRealOnlyForwardTransform(my_buffer.data(), false);
    }

    float calculateMagnitude(float real, float imag)
    {
        return std::sqrt(std::pow(real, 2) + std::pow(imag, 2));
    }

    void getHarmonics(int rowOfFirstHarmonic, int rowOfSecondHarmonic, float* channelData, double toUseSampleRate)
    {
        firstHarmonic = processPitchDetection(channelData, toUseSampleRate) * rowOfFirstHarmonic;
        secondHarmonic = processPitchDetection(channelData, toUseSampleRate) * rowOfSecondHarmonic;
    }

    void getMagnitudeBuffer(std::vector <float>& my_buffer, std::vector <float>& bufferToFill)
    {
        int count = 0;

        for (int s = 0; s < detectFftSize; s += 2)
        {
            bufferToFill[count] = calculateMagnitude(my_buffer[s], my_buffer[s + 1]);
                count++;
        }
    }
    
    void getMaxMagnitude(std::vector <float>& magnitudeBuffer)
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

    float getFrequency(double sampleRate)
    {
        return float(idxOfMaxMagnitude) * float(sampleRate)/ detectFftSize;
    }



    float processPitchDetection(float* channelData,double toUseSampleRate)
    {
        clearMagnitudeBuffer(preMagnitudeBuffer);
        pushSamplesIntoBuffer(preMagnitudeBuffer, channelData,bufferSize);
        performForwardFFT(preMagnitudeBuffer);
        getMagnitudeBuffer(preMagnitudeBuffer, magnitudeBuffer);
        getMaxMagnitude(magnitudeBuffer);
        return getFrequency(toUseSampleRate);
    }
  
};