
#pragma once

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
    PitchDetector() :
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
        resizeVectors();
    }
   
    void resizeVectors()
    {
        preMagnitudeBuffer.resize(detectFftSize);
        magnitudeBuffer.resize(halfDetectFftSize);
    }

    void clearMagnitudeBuffer(std::vector <float>& myBuffer)
    {
        std::fill(myBuffer.begin(), myBuffer.end(), 0.0f);
    }
    
    void pushSamplesIntoBuffer(std::vector <float>& myBuffer, const float* channelData, int bufferSize)
    {
        std::copy(channelData, channelData + bufferSize, myBuffer.begin());
    }

    void performForwardFFT(std::vector <float>& myBuffer)
    {
        detectFft.performRealOnlyForwardTransform(myBuffer.data(), false);
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

    void getMagnitudeBuffer(std::vector <float>& myBuffer, std::vector <float>& bufferToFill)
    {
        int count = 0;
        const int stepSize = 2;

        for (auto it = myBuffer.begin(); it != myBuffer.end(); it += stepSize)
        {
            bufferToFill[count] = calculateMagnitude(*it, *(it + 1));
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