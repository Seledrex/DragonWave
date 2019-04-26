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
	void setCarrierPitchShift(float* shift);
	void setCarrierFilterParams(float newType, float newCutoff, float newQ);
	void setCarrierEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel);
	void setCarrierFilterEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newDepth);

	//==============================================================================
	void setFmOscParams(float* newFrequency, float* newDepth);
	void setFmFilterParams(float* newType, float* newCutoff, float* newQ);
	void setFmEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel);

	void setGlobalParams(float* newUnison, float* newDetune, float* newSpread);

private:

	//==============================================================================
	int wavetableSize = 0;
	float frequency = 0.0f;
	float level = 0.0f;
	int pitchShift = 0;

	float carrierFilterCutoff = 0.0f;
	int carrierFilterType = 0;
	float carrierFilterQ = 0.0f;

	float fmFrequency = 0.0f;
	float fmDepth = 0.0f;

	float carrierEnvLevel = 0.0f;
	float fmEnvLevel = 0.0f;
	float carrierFilterEnvLevel = 0.0f;

	float carrierWavetableMix = 0.0f;
	float fmWavetableMix = 0.0f;

	float fmAttack = 0.0f;
	float fmDecay = 0.0f;
	float fmSustain = 0.0f;
	float fmRelease = 0.0f;

	//==============================================================================
	int currentUnison = 1;
	int previousUnison = 0;

	float detune = 0.0f;
	float spread = 0.0f;

	std::vector<float> carrierIndices;
	std::vector<float> carrierDeltas;
	std::vector<float> fmIndices;
	std::vector<float> fmDeltas;

	std::vector<float> detuneAmounts;
	std::vector<float> spreadAmounts;

	OwnedArray<IIRFilter> fmFilters;
	OwnedArray<ADSR> fmEnvelopes;

	IIRFilter carrierFilterL;
	IIRFilter carrierFilterR;

	int fmFilterType = 0;
	float fmFilterCutoff = 0.0f;
	float fmFilterQ = 0.0f;

	//==============================================================================
	WavetableSound* ws = nullptr;
	ReferenceCountedWavetable::Ptr carrierPtr;
	ReferenceCountedWavetable::Ptr fmPtr;
	Wavetable* carrier = nullptr;
	Wavetable* fm = nullptr;

	//==============================================================================
	std::pair<int, int> carrierBIs;
	std::pair<int, int> fmBIs;

	//==============================================================================
	Random random;
	IIRFilter carrierFilter;
	IIRFilter fmFilter;
	ADSR carrierEnvelope;
	ADSR carrierFilterEnvelope;

	bool noteIsActive = false;

	//==============================================================================
	NormalisableRange<float> filterCutoffRange;
	float filterCutoffLowerBound = 0.0f;
	float filterCutoffUpperBound = 0.0f;
	float filterCutoffDirection = 1.0f;

	void setFilterParams(IIRFilter* filter, int type, float cutoff, float q);

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableVoice);
};