
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ExpressoMachineAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ExpressoMachineAudioProcessorEditor (ExpressoMachineAudioProcessor&);
    ~ExpressoMachineAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ExpressoMachineAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExpressoMachineAudioProcessorEditor)
};
