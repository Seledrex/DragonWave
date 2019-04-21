/*
  ==============================================================================

    Waveshaper.h
    Created: 19 Apr 2019 8:26:53pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Waveshaper
{
public:

	//==============================================================================
	enum TransferFunction
	{
		FAINT,
		LIGHT,
		MODERATE,
		HEAVY,
		HARSH
	};

	//==============================================================================
	Waveshaper();

	//==============================================================================
	void setParameters(TransferFunction f, float dry, float wet);
	void processMono(float* buffer, int numSamples);
	void processStereo(float* leftChannel, float* rightChannel, int numSamples);
	float processSingleSample(float sample);

private:

	//==============================================================================
	int size = 8192;

	//==============================================================================
	std::vector<float> faintTable;
	std::vector<float> lightTable;
	std::vector<float> moderateTable;
	std::vector<float> heavyTable;
	std::vector<float> harshTable;
	std::vector<float>& currentTable;

	//==============================================================================
	TransferFunction f = MODERATE;
	float dry = 0.0f;
	float wet = 0.0f;

	//==============================================================================
	std::vector<float> sampleTransferFunction(std::function<float(float)> f);

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Waveshaper)
};