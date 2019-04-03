/*
  ==============================================================================

    ReferenceCountedSound.h
    Created: 2 Apr 2019 4:22:35pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "WavetableSound.h"

class ReferenceCountedSound : public ReferenceCountedObject
{
public:
	typedef ReferenceCountedObjectPtr<ReferenceCountedSound> Ptr;

	//==============================================================================
	ReferenceCountedSound(const String& nameToUse,
						  WavetableSound::Waveform type,
						  const std::vector<float> wavetable = std::vector<float>());
	~ReferenceCountedSound();

	//==============================================================================
	WavetableSound* getSound();

private:
	String name;
	WavetableSound sound;
	WavetableSound::Waveform type;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedSound)
};