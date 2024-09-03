/*
  ==============================================================================

    RMS.cpp
    Created: 3 Sep 2024 8:32:22pm
    Author:  barto

  ==============================================================================
*/

#include "RMS.h"
#include <cmath>

RMS::RMS(int channelCount, int startSample, int bufferSize)
	:m_channelCount(channelCount),
	m_startSample(startSample),
	m_bufferSize(bufferSize),
	m_maxRMS(0),
	m_minRMS(0)
{
};

float RMS::GetRMSValueOfBuffer(juce::AudioBuffer<float>& rmsBuffer)
{
	if ((m_bufferSize == 0 || m_channelCount == 0)
		|| (rmsBuffer.getNumChannels() != m_channelCount
			|| rmsBuffer.getNumSamples() != m_bufferSize))
		return 0.0f;

	float sumSquares = 0.0f;

	for (int channel = 0; channel < m_channelCount; channel++) {

		const float* channelData = rmsBuffer.getReadPointer(channel);

		for (int sample = 0; sample < m_bufferSize; sample++) {
			float value = channelData[sample];
			sumSquares += value * value;
		}
	}

	return std::sqrt(sumSquares / (m_bufferSize * m_channelCount));
}

void RMS::SetBoundsOfRMS(float recentBufferRMS)
{
	// Check for NaN values and skip if recentBufferRMS is not a number
	if (std::isnan(recentBufferRMS)) {
		return;
	}

	if (recentBufferRMS > m_maxRMS) {
		m_maxRMS = recentBufferRMS;
	}

	if (recentBufferRMS < m_minRMS) {
		m_minRMS = recentBufferRMS;
	}
}

void RMS::CleanBoundsOfRMS()
{
	m_maxRMS = 0.0f;
	m_minRMS = 0.0f;
}

void RMS::ProccesRMSAction(juce::AudioBuffer<float>& rmsBuffer)
{
	float rmsValue = GetRMSValueOfBuffer(rmsBuffer);

	SetBoundsOfRMS(rmsValue);

	if (rmsValue <= 0)
		CleanBoundsOfRMS();
}
