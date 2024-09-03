#pragma once
#include <JuceHeader.h>
/*
  ==============================================================================

    RMS.h
    Created: 3 Sep 2024 8:32:22pm
    Author:  barto

  ==============================================================================
*/

/**
 * @class RMS
 * @brief A class designed to handle RMS (Root Mean Square) calculations for audio signals.
 *
 * This class is used to track the maximum and minimum RMS levels observed in an audio signal.
 * It also provides functionality to calculate the RMS level for a specific audio buffer,
 * allowing real-time monitoring of the signal's power level.
 *
 */

class RMS
{
public:

	/**
		* @brief Constructs an RMSClass object with specified parameters.
		*
		* @param channelCount The number of audio channels to consider in the RMS calculation.
		* @param startSample The index of the first sample to start RMS calculations from.
		* @param bufferSize The number of samples in the buffer used for RMS calculations.
	*/
	RMS(int channelCount, int startSample, int bufferSize) {};


	/**
		* @brief Returns RMS of audio signal.
		*
		* This method calculates the RMS value of the audio signal stored in the provided
		* audio buffer.
		*
		* @param rmsBuffer A reference to a JUCE `AudioBuffer<float>` object containing
		*  the audio data for which the RMS value is to be calculated.
		*
		* @return The calculated RMS value as a float.
	*/
	float GetRMSValueOfBuffer(juce::AudioBuffer<float>& rmsBuffer);


	/**
		*@brief Updates the minimumand maximum RMS values based on the recent RMS value.
		*
		* This method checks the provided recent RMS valueand updates the minRMSand maxRMS
		* members if the recent value is lower than the current minRMS or higher than the
		* current maxRMS, respectively.
		*
		* @param recentBufferRMS The most recent RMS value of the buffer to compare against
		* the current minand max RMS values.
	*/
	void SetBoundsOfRMS(float recentBufferRMS);


	/**
		 * @brief Resets the minimum and maximum RMS values to their initial states.
		 *
		 * This method resets the minRMS and maxRMS values to their predefined initial
		 * states. It is useful for restarting the RMS monitoring process, clearing out
		 * any previously stored extreme values and allowing new measurements to be
		 * tracked from a clean state.
	 */
	void CleanBoundsOfRMS();

	/**
		*@brief
	*/
	void ProccesRMSAction(juce::AudioBuffer<float>& rmsBuffer);

	float m_rmsValue;

private:
	int m_channelCount;
	int m_startSample;
	int m_bufferSize;
	float m_maxRMS;
	float m_minRMS;
};

