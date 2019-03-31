/*
  ==============================================================================

	WavetableSound.h
	Created: 22 Mar 2019 12:14:37pm
	Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wavetable.h"
#include "Util.h"

class WavetableSound : public SynthesiserSound
{
public:
	WavetableSound();
	bool appliesToNote(int) override;
	bool appliesToChannel(int) override;
	Wavetable* getWavetable();

private:
	const int tableSize = 2048;
	std::unique_ptr<Wavetable> wavetable = nullptr;
};