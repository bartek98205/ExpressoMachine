
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ExpressoMachineAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ExpressoMachineAudioProcessorEditor (ExpressoMachineAudioProcessor&);
    void buttonClicked(juce::Button* button);
    ~ExpressoMachineAudioProcessorEditor() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ExpressoMachineAudioProcessor& audioProcessor;

    juce::Slider gainS;
    juce::Label gainL;
    juce::Slider dW;
    juce::Label dW_label;
    juce::TextButton gainType;
    juce::TextButton boostType;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExpressoMachineAudioProcessorEditor)
};
