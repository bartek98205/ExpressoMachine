
#pragma once

#include <JuceHeader.h>
#include "PitchDetector.h"
#include "testClass.h"
#include "RMS.h"
#include "Blend.h"
#include "Gain.h"
#include "PitchShifter.h"

class ExpressoMachineAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    ExpressoMachineAudioProcessor();
    ~ExpressoMachineAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override; 

    float cleanBlendValue, gainBlendValue, filterBlendValue;
    bool gainType;

private:
    PitchDetector pitch;
    TestClass test;
    RMSClass rms;
    BlendClass blend;
    GainClass gain;
    PitchShifter pitchShifter;

    int bufferSize;
    float mySampleRate;
    float lowestRMS,maxRMS;
    
    juce::dsp::ProcessorDuplicator <juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients<float>> notchPassFilter;
    juce::dsp::ProcessorDuplicator <juce::dsp::StateVariableFilter::Filter<float>, juce::dsp::StateVariableFilter::Parameters<float>> lowPassFilter;
    float qFactor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExpressoMachineAudioProcessor)
};
