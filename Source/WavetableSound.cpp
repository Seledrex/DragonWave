/*
  ==============================================================================

	WavetableSound.cpp
	Created: 22 Mar 2019 12:14:37pm
	Author:  Eric

  ==============================================================================
*/

#include "WavetableSound.h"

WavetableSound::WavetableSound(ReferenceCountedWavetable::Ptr carrier, ReferenceCountedWavetable::Ptr fm)
{
	this->carrier = carrier;
	this->fm = fm;
}

//==============================================================================
bool WavetableSound::appliesToNote(int)
{
	return true;
}

bool WavetableSound::appliesToChannel(int)
{
	return true;
}