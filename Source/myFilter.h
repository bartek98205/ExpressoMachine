
#pragma once

class myFilters
{
public:
    juce::dsp::ProcessorDuplicator <juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients<float>> notchPassFilter;
    juce::dsp::ProcessorDuplicator <juce::dsp::StateVariableFilter::Filter<float>, juce::dsp::StateVariableFilter::Parameters<float>> lowPassFilter;
    float qFactor;

    myFilters():
        qFactor(0.1),
        notchPassFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(96000, 2200.0f, 0.1f, 1.0f))
    
    {}

};