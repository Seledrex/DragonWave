/*
  ==============================================================================

	WavetableSound.h
	Created: 22 Mar 2019 12:14:37pm
	Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Functional.h"
#include "Util.h"

//==============================================================================

class WavetableSound : public SynthesiserSound
{
public:

	//==============================================================================
	static const int wavetableSize = 2048;

	//==============================================================================
	enum Waveform {
		Sine,
		Triangle,
		Sawtooth,
		Square,
		Noise,
		Arbitrary
	};

	//==============================================================================
	WavetableSound();
	WavetableSound(const WavetableSound& other);

	//==============================================================================
	bool appliesToNote(int) override;
	bool appliesToChannel(int) override;

	//==============================================================================
	void makeArbitrary(std::vector<float> wavetable);
	void makeSine();
	void makeTriangle();
	void makeSawtooth();
	void makeSquare();
	void makeNoise();

	//==============================================================================
	AudioSampleBuffer& getWavetables();
	int getWavetableSize();
	Waveform getWaveformType();
	bool isInitialized();

	//==============================================================================
	std::pair<float, float> getBoundingFrequencies(float target);
	std::pair<int, int> getBoundingIndexes(std::pair<float, float> targetFrequencies);

private:
	const int fftOrder = 16;
	const int samplingRate = 44100;
	bool initialized = false;

	//==============================================================================
	const float lowestFrequency = 20.0f;
	const float highestFrequency = 8000.0f;

	//==============================================================================
	Waveform type;
	AudioSampleBuffer wavetables;
	std::map<float, int> wavetableMap;
	std::vector<float> wavetableFrequencies{
		32.7f, 65.41f, 130.81f, 261.63f, 523.25f,
		1046.50f, 2093.00f, 4186.01f
	};

	//==============================================================================
	void updateWavetableMap();

	//==============================================================================
	JUCE_LEAK_DETECTOR(WavetableSound);
};