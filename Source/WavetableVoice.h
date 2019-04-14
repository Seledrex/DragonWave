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
	void setPitchShift(float* shift);
	void setFilterParams(float* newType, float* newCutoff, float* newQ);
	void setOscEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel);

private:
	Random random;
	WavetableSound* wavetableSound = nullptr;
	std::pair<float, float> boundingFrequencies;
	std::pair<int, int> boundingIndexes;

	//==============================================================================
	int wavetableSize = 0;
	float currentIndex = 0.0f;
	float tableDelta = 0.0f;
	float level = 0.0f;
	float wavetableMix = 0.0f;
	float oscEnvLevel = 0.0f;

	//==============================================================================
	int pitchShift = 0;

	//==============================================================================
	IIRFilter filter;
	ADSR oscEnvelope;

	//==============================================================================
	forcedinline float getNextSample() noexcept;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableVoice);
};