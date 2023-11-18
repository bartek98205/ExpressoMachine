/// based on Juce tutorial https://docs.juce.com/master/tutorial_spectrum_analyser.html
/// in progress
#pragma once

class specDisp : public juce::AudioAppComponent,
				private juce::Timer
{
public:

	specDisp()
		:fftDisp(fftOrder),
		window (fftSize, juce::dsp::WindowingFunction<float>::hann)
	{
		setOpaque(true);
		setAudioChannels(2, 0);  
		startTimerHz(30);
		setSize(700, 500);
	}

	~specDisp()
	{
		shutdownAudio();
	}


	void prepareToPlay (int, double) override {};
	void releaseResources() override {};

	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
	{
		if (bufferToFill.buffer->getNumChannels() > 0)
		{
			auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

			for (auto i = 0; i < bufferToFill.numSamples; ++i)
				pushNextSampleIntoFifo(channelData[i]);
		}
	}

	void pushNextSampleIntoFifo(float sample) noexcept
	{
		
		if (fifoIndex == fftSize)               
		{
			if (!nextFFTBlockReady)            
			{
				juce::zeromem(fftData, sizeof(fftData));
				memcpy(fftData, fifo, sizeof(fifo));
				nextFFTBlockReady = true;
			}

			fifoIndex = 0;
		}

		fifo[fifoIndex++] = sample;            
	}

	enum
	{
		fftOrder = 11,
		fftSize = 1 << fftOrder,
		scopeSize = 512
	};
private:
	juce::dsp::FFT fftDisp;
	juce::dsp::WindowingFunction<float> window;
	
	float fifo[fftSize];
	float fftData[2 * fftSize];
	int fifoIndex = 0;
	bool nextFFTBlockReady = false;               
	float scopeData[scopeSize];                   

	///JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyserComponent)
};
