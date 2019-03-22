/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RoachNate_a2AudioProcessor::RoachNate_a2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    currentSampleRate = 0.0f;
    currentAngle = 0.0f;
    angleDelta = 0.0f;
    sinFreq = 0.0f;
}

RoachNate_a2AudioProcessor::~RoachNate_a2AudioProcessor()
{
}

//==============================================================================
const String RoachNate_a2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RoachNate_a2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RoachNate_a2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RoachNate_a2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RoachNate_a2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RoachNate_a2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RoachNate_a2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void RoachNate_a2AudioProcessor::setCurrentProgram (int index)
{
}

const String RoachNate_a2AudioProcessor::getProgramName (int index)
{
    return {};
}

void RoachNate_a2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void RoachNate_a2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void RoachNate_a2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RoachNate_a2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void RoachNate_a2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    // Set up another copy of the input buffer so we can keep the original
    AudioBuffer<float>wetBuffer(totalNumInputChannels, buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData =  wetBuffer.getWritePointer(channel);
        // We are getting an audio block size of 512
        // ..do something to the data...
        for(int sample = 0; sample < 512; ++sample)
        {
            auto currentSinsample = (float) std::sin(currentAngle);
            currentAngle += angleDelta;
            //lets make our sin wave wiggle
            wetData[sample] = wetData[sample] * currentSinsample;
            
            float quantum = powf(1.0f, 0);
            auto shapedSample = floor(wetData[sample] * quantum)/quantum;
            wetData[sample] = shapedSample;
            channelData[sample] = channelData[sample] * 0.4f + wetData[sample] * 0.6f;
        }
    }
}

//==============================================================================
bool RoachNate_a2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* RoachNate_a2AudioProcessor::createEditor()
{
    return new RoachNate_a2AudioProcessorEditor (*this);
}

//==============================================================================
void RoachNate_a2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RoachNate_a2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RoachNate_a2AudioProcessor();
}
