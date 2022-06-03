/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbTest1AudioProcessor::ReverbTest1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ReverbTest1AudioProcessor::~ReverbTest1AudioProcessor()
{
}

//==============================================================================
const juce::String ReverbTest1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbTest1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbTest1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbTest1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbTest1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbTest1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbTest1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbTest1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbTest1AudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbTest1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbTest1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(samplesPerBlock);
    reverb.setSampleRate(sampleRate);
}

void ReverbTest1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbTest1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ReverbTest1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
    {
        // mono in (0, 0), stereo out (0, 1)
        reverb.process(buffer.getReadPointer(0), buffer.getReadPointer(0),
            buffer.getWritePointer(0), buffer.getWritePointer(1),
            buffer.getNumSamples());
    }
    else if (totalNumInputChannels == 2 && totalNumOutputChannels == 2)
    {
        // stereo in (0, 1), stereo out (0, 1)
        reverb.process(buffer.getReadPointer(0), buffer.getReadPointer(1),
            buffer.getWritePointer(0), buffer.getWritePointer(1),
            buffer.getNumSamples());

        delay.initDelay(500, getSampleRate());
        delay.next(buffer.getWritePointer(0));
        delay.getOutput();
    }
    else
    {
        jassertfalse; // channel layout not supported
    }
}

//==============================================================================
bool ReverbTest1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbTest1AudioProcessor::createEditor()
{
    return new ReverbTest1AudioProcessorEditor (*this);
}

//==============================================================================
void ReverbTest1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverbTest1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbTest1AudioProcessor();
}
