/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WavetableSound.h"
#include "ReferenceCountedSound.h"

//==============================================================================
class LoadingThread;

//==============================================================================
/**
*/
class DragonWaveAudioProcessor : public AudioProcessor
{
public:
	
	const String WAVEFORM_CHOICE_ID = "waveformChoice";
	const String WAVETABLE_PATH_ID = "wavetablePath";

	//==============================================================================
	DragonWaveAudioProcessor();
	~DragonWaveAudioProcessor();

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String& newName) override;

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	//==============================================================================
	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	//==============================================================================
	AudioProcessorValueTreeState parameters;
	WavetableSound* sound;
	Synthesiser synth;

	//==============================================================================
	int noteOnCount = 0;

	//==============================================================================
	std::unique_ptr<LoadingThread> loadingThread;
	ReferenceCountedArray<ReferenceCountedSound> sounds;
	ReferenceCountedSound::Ptr currentSound;

	//==============================================================================
	String chosenPath = "";
	WavetableSound::Waveform chosenWaveform = WavetableSound::Waveform::Sawtooth;

private:

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonWaveAudioProcessor)
};
