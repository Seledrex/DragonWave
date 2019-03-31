/*
  ==============================================================================

	WavetableSound.cpp
	Created: 22 Mar 2019 12:14:37pm
	Author:  Eric

  ==============================================================================
*/

#include "WavetableSound.h"

WavetableSound::WavetableSound()
{
	auto square = std::vector<float>(tableSize, -1.0);
	for (int i = 0; i < tableSize / 2; i++)
		square[i] = 1.0;

	auto sawtooth = Util::linspace(1.0, -1.0, tableSize);
	wavetable = std::make_unique<Wavetable>(sawtooth, 44100);
}

bool WavetableSound::appliesToNote(int)
{
	return true;
}

bool WavetableSound::appliesToChannel(int)
{
	return true;
}

Wavetable* WavetableSound::getWavetable()
{
	return wavetable.get();
}