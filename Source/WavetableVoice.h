/*
  ==============================================================================

	WavetableVoice.h
	Created: 22 Mar 2019 12:14:50pm
	Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wavetable.h"

class WavetableVoice : public SynthesiserVoice
{
public:
	WavetableVoice();
	bool canPlaySound(SynthesiserSound* sound) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;

private:
	Wavetable* wavetable = NULL;
	std::pair<float, float> boundingFrequencies;
	std::pair<int, int> boundingIndexes;
	int tableSize = 0;
	float currentIndex = 0.0f;
	float tableDelta = 0.0f;
	float level = 0.0f;
	float wavetableMix = 0.0f;

	forcedinline float getNextSample() noexcept;
};