/*
  ==============================================================================

    Wavetable.h
    Created: 14 Apr 2019 9:16:40pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Functional.h"
#include "Util.h"

class Wavetable
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

	static String waveformToString(Waveform type)
	{
		switch (type) {
		case Wavetable::Waveform::Sine:
			return "Sine";
		case Wavetable::Waveform::Triangle:
			return "Triangle";
		case Wavetable::Waveform::Sawtooth:
			return "Sawtooth";
		case Wavetable::Waveform::Square:
			return "Square";
		case Wavetable::Waveform::Noise:
			return "Noise";
		case Wavetable::Waveform::Arbitrary:
			return "Arbitrary";
		default:
			return "Sine";
		}
	}

	//==============================================================================
	Wavetable(Waveform type, std::vector<float> wavetable = std::vector<float>());
	Wavetable(const Wavetable& other);

	//==============================================================================
	AudioSampleBuffer& getWavetables();
	int getWavetableSize();
	Waveform getWaveformType();

	//==============================================================================
	std::pair<float, float> getBoundingFrequencies(float target);
	std::pair<int, int> getBoundingIndexes(std::pair<float, float> targetFrequencies);

private:
	const int fftOrder = 16;
	const int samplingRate = 44100;

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
	void makeArbitrary(std::vector<float> wavetable);
	void makeSine();
	void makeTriangle();
	void makeSawtooth();
	void makeSquare();
	void makeNoise();
	void updateWavetableMap();

	//==============================================================================
	JUCE_LEAK_DETECTOR(Wavetable);
};