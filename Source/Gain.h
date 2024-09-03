/*
  ==============================================================================

    Gain.h
    Created: 3 Sep 2024 9:27:38pm
    Author:  barto

  ==============================================================================
*/

/**
* @class Gain
* @brief This class implements a simple gain effect for audio processing.
*
* The Gain class provides functionality to apply gain(amplification) to audio samples.
* It supports different types of gain(soft and hard) and can modify audio signals by
* scaling them with a gain factor.This class is useful in audio processing to adjust
* the volume or apply effects to audio signals.
*/

#pragma once

class Gain
{
public:

    /**
     * @enum TypeOfGain
     * @brief Defines the type of gain effect to be applied.
     *
     * - `soft`: Represents a soft gain effect, typically less aggressive.
     * - `hard`: Represents a hard gain effect, which might apply more drastic changes.
     */
    enum TypeOfGain { soft, hard };

    Gain() {};
    ~Gain() {};

    /**
     * @brief Modulo operation used in gain calculations.
     *
     * This method performs a modulo operation that returns the remainder
     * of the division of `n` by `d`. It's commonly used in calculations
     * to wrap around values within a certain range.
     *
     * @param n The numerator (dividend).
     * @param d The denominator (divisor).
     * @return The remainder of `n / d`.
     */
    float Mod (float n, float d);

    /**
     * @brief Apply basic gain to an audio sample.
     *
     * This method modifies the given audio sample by applying a gain effect.
     * It supports different types of gain (soft or hard) and uses the specified
     * gain factor to amplify or adjust the sample. The type of gain affects how
     * the amplification is applied, which can result in different audio characteristics.
     *
     * @param sample The audio sample to modify.
     * @param type The type of gain to apply (soft or hard).
     * @param gainFactor The factor by which to multiply the sample to apply the gain.
     * @return The modified audio sample with the gain effect applied.
     */
    float BasicGains(float sample, TypeOfGain type, int gainFactor);

};