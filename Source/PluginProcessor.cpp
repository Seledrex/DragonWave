/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WavetableVoice.h"
#include "LoadingThread.h"

//==============================================================================
DragonWaveAudioProcessor::DragonWaveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	parameters(*this, nullptr)
#endif
{
	loadingThread = std::unique_ptr<LoadingThread>(new LoadingThread(*this));

	for (auto i = 0; i < 32; i++) {
		synth.addVoice(new WavetableVoice());
	}

	chosenWaveform = WavetableSound::Waveform::Sawtooth;
	loadingThread->notify();
}

DragonWaveAudioProcessor::~DragonWaveAudioProcessor()
{
}

//==============================================================================
const String DragonWaveAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool DragonWaveAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool DragonWaveAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool DragonWaveAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double DragonWaveAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int DragonWaveAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int DragonWaveAudioProcessor::getCurrentProgram()
{
	return 0;
}

void DragonWaveAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const String DragonWaveAudioProcessor::getProgramName(int /*index*/)
{
	return {};
}

void DragonWaveAudioProcessor::changeProgramName(int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void DragonWaveAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);
}

void DragonWaveAudioProcessor::releaseResources()
{
	currentSound = nullptr;
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DragonWaveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
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

void DragonWaveAudioProcessor::processBlock(AudioBuffer<float> & buffer, MidiBuffer & midiMessages)
{
	buffer.clear();

	int time;
	MidiMessage m;

	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
	{
		if (m.isNoteOn())
			noteOnCount++;
		else if (m.isNoteOff())
			noteOnCount--;
	}

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool DragonWaveAudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* DragonWaveAudioProcessor::createEditor()
{
	return new DragonWaveAudioProcessorEditor(*this);
}

//==============================================================================
void DragonWaveAudioProcessor::getStateInformation(MemoryBlock& /*destData*/)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void DragonWaveAudioProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new DragonWaveAudioProcessor();
}