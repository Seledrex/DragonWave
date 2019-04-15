/*
  ==============================================================================

	WavetableVoice.h
	Created: 22 Mar 2019 12:14:50pm
	Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WavetableSound.h"

class WavetableVoice : public SynthesiserVoice
{
public:
	WavetableVoice();

	//==============================================================================
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;

	//==============================================================================
	void setOscPitchShift(float* shift);
	void setOscFilterParams(float* newType, float* newCutoff, float* newQ);
	void setOscEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel);
	
	//==============================================================================
	void setFmOscParams(float* newFrequency, float* newDepth);
	void setFmFilterParams(float* newType, float* newCutoff, float* newQ);

private:

	//==============================================================================
	int wavetableSize = 0;
	int pitchShift = 0;
	float frequency = 0.0f;
	float level = 0.0f;
	float oscEnvLevel = 0.0f;
	float fmFrequency = 0.0f;
	float fmDepth = 0.0f;

	float currentCarrierIndex = 0.0f;
	float carrierTableDelta = 0.0f;
	float carrierWavetableMix = 0.0f;

	float currentFmIndex = 0.0f;
	float fmTableDelta = 0.0f;
	float fmWavetableMix = 0.0f;

	//==============================================================================
	WavetableSound* ws = nullptr;
	Wavetable* carrier = nullptr;
	Wavetable* fm = nullptr;

	//==============================================================================
	std::pair<int, int> carrierBIs;
	std::pair<int, int> fmBIs;

	//==============================================================================
	Random random;
	IIRFilter carrierFilter;
	IIRFilter fmFilter;
	ADSR oscEnvelope;

	//==============================================================================
	forcedinline float getNextSample() noexcept;
	forcedinline void modulateFrequency() noexcept;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableVoice);
};