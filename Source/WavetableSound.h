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
#include "ReferenceCountedWavetable.h"

//==============================================================================

class WavetableSound : public SynthesiserSound
{
public:

	ReferenceCountedWavetable::Ptr carrier;
	ReferenceCountedWavetable::Ptr fm;

	//==============================================================================
	WavetableSound(ReferenceCountedWavetable::Ptr carrier, ReferenceCountedWavetable::Ptr fm);

	//==============================================================================
	bool appliesToNote(int) override;
	bool appliesToChannel(int) override;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavetableSound);
};