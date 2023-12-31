
#include "PluginProcessor.h"
#include "PluginEditor.h"

ExpressoMachineAudioProcessorEditor::ExpressoMachineAudioProcessorEditor(ExpressoMachineAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{

    addAndMakeVisible(gainS);
    gainS.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainS.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 72, 32);
    gainS.setRange(0.1, 5, 0.1);

    gainS.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::ghostwhite);
    addAndMakeVisible(gainL);
    gainL.setText("Gain", juce::dontSendNotification);
    gainL.attachToComponent(&gainS, true);


    addAndMakeVisible(gainType);
    gainType.setButtonText("Easy gain"); // Overdrive

    addAndMakeVisible(boostType);
    boostType.setButtonText("Classic tones");   /// Extra tones


    addAndMakeVisible(dW);
    dW.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dW.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 72, 32);
    dW.setRange(0.1, 1, 0.1);
    dW.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::ghostwhite);
    addAndMakeVisible(dW_label);
    dW_label.setText("Dry/Wet", juce::dontSendNotification);
    //dW_label.attachToComponent(&dW, false);


    setSize(width,height);
}

ExpressoMachineAudioProcessorEditor::~ExpressoMachineAudioProcessorEditor()
{
}

void ExpressoMachineAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey.darker(0.1f));

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.setColour(juce::Colours::white);
    g.drawFittedText("ExpressoMachine", getLocalBounds(), juce::Justification::centredTop, 1);
    g.setFont(15.0f);
    g.fillAll(juce::Colours::black);

    g.setOpacity(1.0f);
    g.setColour(juce::Colours::white);
    disp.drawFrame(g);
}

void ExpressoMachineAudioProcessorEditor::resized()
{
    gainS.setBounds(50, height/5, 72, height/2 );
    dW.setBounds(width - (50 + 72), height / 5, 72, height/2);
    gainL.setBounds(65 , ((height / 5) - 25), 100, 24);
    dW_label.setBounds((width - ( 42 + 72)) , ((height / 5) - 25) , 100, 24);
    gainType.setBounds(width/ 3 , height - 200, 72, 72);
    boostType.setBounds(width /2 + 75 , height - 200, 72, 72);
}


void ExpressoMachineAudioProcessorEditor::buttonClicked(juce::Button* button) {

    if (button == &gainType) {

        if (audioProcessor.gainType == true) {
            audioProcessor.gainType = false;
            gainType.setButtonText("OverDrive");

        }
        else {
            audioProcessor.gainType = true;
            gainType.setButtonText("Distortion");
        }

    }

}