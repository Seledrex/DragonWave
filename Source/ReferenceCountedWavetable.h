/*
  ==============================================================================

    ReferenceCountedSound.h
    Created: 2 Apr 2019 4:22:35pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Wavetable.h"

class ReferenceCountedWavetable : public ReferenceCountedObject
{
public:
	typedef ReferenceCountedObjectPtr<ReferenceCountedWavetable> Ptr;

	//==============================================================================
	ReferenceCountedWavetable(const String& path,
						  Wavetable::Waveform type,
						  const std::vector<float> wavetable = std::vector<float>());
	~ReferenceCountedWavetable();

	//==============================================================================
	Wavetable* getWavetable();
	String getPath();

private:
	String path;
	Wavetable wavetable;
	Wavetable::Waveform type;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedWavetable)
};