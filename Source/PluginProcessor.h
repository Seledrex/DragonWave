/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WavetableSound.h"
#include "ReferenceCountedWavetable.h"
#include "Constants.h"
#include "Waveshaper.h"

//==============================================================================
class LoadingThread;

//==============================================================================
class DragonWaveAudioProcessor : public AudioProcessor
{
public:

	//==============================================================================
	DragonWaveAudioProcessor();
	~DragonWaveAudioProcessor();

	//==============================================================================
	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

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
	AudioProcessorValueTreeState parameters;
	Synthesiser synth;

	//==============================================================================
	std::unique_ptr<LoadingThread> loadingThread;
	ReferenceCountedArray<ReferenceCountedWavetable> carrierWavetables;
	ReferenceCountedArray<ReferenceCountedWavetable> fmWavetables;
	ReferenceCountedWavetable::Ptr currentCarrierWavetable;
	ReferenceCountedWavetable::Ptr currentFMWavetable;

	//==============================================================================
	String chosenCarrierPath = "";
	String chosenFMPath = "";
	Wavetable::Waveform chosenCarrierWaveform = Wavetable::Waveform::Sawtooth;
	Wavetable::Waveform chosenFMWaveform = Wavetable::Waveform::Sine;

	//==============================================================================
	IIRFilter lowFilterL;
	IIRFilter lowFilterR;
	IIRFilter midFilterL;
	IIRFilter midFilterR;
	IIRFilter highFilterL;
	IIRFilter highFilterR;

	//==============================================================================
	Reverb reverb;
	Waveshaper waveshaper;

private:

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonWaveAudioProcessor)
};
