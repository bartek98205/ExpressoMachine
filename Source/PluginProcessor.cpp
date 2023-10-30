
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PitchDetector.h"

ExpressoMachineAudioProcessor::ExpressoMachineAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    bufferSize(960),
    lowestRMS(0),
    maxRMS(1),
    gainBlendValue(0),
    cleanBlendValue(0),
    filterBlendValue(0),
    mySampleRate(0),
    qFactor(0.1),
    notchPassFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(96000, 2200.0f, 0.1f, 1.0f))
 
#endif
{
}

ExpressoMachineAudioProcessor::~ExpressoMachineAudioProcessor()
{
}

const juce::String ExpressoMachineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ExpressoMachineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ExpressoMachineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ExpressoMachineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ExpressoMachineAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ExpressoMachineAudioProcessor::getNumPrograms()
{
    return 1;   
}

int ExpressoMachineAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExpressoMachineAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ExpressoMachineAudioProcessor::getProgramName (int index)
{
    return {};
}

void ExpressoMachineAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void ExpressoMachineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    bufferSize = samplesPerBlock;
    mySampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = mySampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    notchPassFilter.prepare(spec);
    notchPassFilter.reset();
    lowPassFilter.prepare(spec);
    lowPassFilter.reset();

    juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
    lowPassFilter.state->setCutOffFrequency(mySampleRate, 10000, 0.4);

}

void ExpressoMachineAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ExpressoMachineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ExpressoMachineAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
   
    test.testSin(bufferSize, mySampleRate, 220, 0.1);

    for (int channel = 0; channel < 1; ++channel)
    {
       auto* channelData = buffer.getWritePointer (channel);
       float bufferRMS = rms.my_getRMS(channel, buffer, bufferSize);   

       rms.setBoundsOfRMS(bufferRMS);

       cleanBlendValue = blend.dryWetRatio(bufferRMS, 0.01f, 1.0f);
       gainBlendValue = blend.dryWetRatio(bufferRMS, 1.0f, 5.5f);
       filterBlendValue = blend.dryWetRatio(bufferRMS, 0.1f, 8.0f);

       pitch.getHarmonics(2, 4, channelData, mySampleRate);
       pitchShifter.processExpressoEffcet(buffer,channelData, channel,cleanBlendValue, gainBlendValue);     

       juce::dsp::AudioBlock<float> block = buffer;
       *notchPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(mySampleRate, (880.0f, 1320.0f), qFactor, (filterBlendValue));
        notchPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
       
       juce::dsp::AudioBlock<float> lowBlock = buffer;
       lowPassFilter.process(juce::dsp::ProcessContextReplacing<float>(lowBlock));
    }
}

bool ExpressoMachineAudioProcessor::hasEditor() const
{
    return true; 
}

juce::AudioProcessorEditor* ExpressoMachineAudioProcessor::createEditor()
{
    return new ExpressoMachineAudioProcessorEditor (*this);
}

void ExpressoMachineAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
  
}

void ExpressoMachineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExpressoMachineAudioProcessor();
}
