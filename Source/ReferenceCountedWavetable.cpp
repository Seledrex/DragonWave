/*
  ==============================================================================

    ReferenceCountedSound.cpp
    Created: 2 Apr 2019 4:22:35pm
    Author:  Eric

  ==============================================================================
*/

#include "ReferenceCountedWavetable.h"

/*
This class is used to keep track of how many references are associated with a loaded
sound. This is to ensure that the multithreading will work seemlessly.
*/
ReferenceCountedWavetable::ReferenceCountedWavetable(const String& path,
											 Wavetable::Waveform type,
											 const std::vector<float> wavetable)
	: path(path), wavetable(type, wavetable)
{
	this->type = type;
	if (type == Wavetable::Waveform::Arbitrary)
		DBG("Sound from '" + path + "' created");
	else
		DBG("Sound named " + Wavetable::waveformToString(type) + " created");
}

ReferenceCountedWavetable::~ReferenceCountedWavetable()
{
	if (type == Wavetable::Waveform::Arbitrary)
		DBG("Sound from '" + path + "' destroyed");
	else
		DBG("Sound named " + Wavetable::waveformToString(type) + " destroyed");
}

//==============================================================================
Wavetable* ReferenceCountedWavetable::getWavetable()
{
	return &wavetable;
}

String ReferenceCountedWavetable::getPath()
{
	return path;
}
