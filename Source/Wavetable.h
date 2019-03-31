/*
  ==============================================================================

	Wavetable.h
	Created: 25 Mar 2019 6:30:06pm
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

	// Initializes with an input wavetable and the sampling rate
	Wavetable(std::vector<float> inTable, int samplingRate);

	// Returns the two frequencies the bound the target frequency
	std::pair<float, float> getBoundingFrequencies(float targetFrequency);

	// Returns the two indexes that correspond to the target frequencies
	std::pair<int, int> getBoundingIndexes(std::pair<float, float> targetFrequencies);

	// Gets the wavetable buffer
	AudioSampleBuffer* getTables();

	// Gets the wavetable's size
	int getSize();

private:

	// Size of wavetable
	int wavetableSize;

	// Buffer that holds each wavetable on a seperate channel
	AudioSampleBuffer tables;

	// Frequencies that will have wavetables generated for them
	std::vector<float> tableFrequencies{
		32.7f, 65.41f, 130.81f, 261.63f, 523.25f,
		1046.50f, 2093.00f, 4186.01f
	};

	// Maps frequencies to wavetables
	std::map<float, int> tableMap;

	// Finds the FFT order based off the sampling rate
	int findFFTOrder(int samplingRate);

	// Finds the nearest frequencies that bound the target
	std::pair<float, float> findNearest(float target);
};