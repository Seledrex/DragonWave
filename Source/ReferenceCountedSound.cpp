/*
  ==============================================================================

    ReferenceCountedSound.cpp
    Created: 2 Apr 2019 4:22:35pm
    Author:  Eric

  ==============================================================================
*/

#include "ReferenceCountedSound.h"

ReferenceCountedSound::ReferenceCountedSound(const String& nameToUse,
											 WavetableSound::Waveform type,
											 const std::vector<float> wavetable)
	: name(nameToUse), sound()
{
	this->type = type;

	switch (type) {
	case WavetableSound::Waveform::Sine:
		sound.makeSine();
		DBG("Sound named 'Sine' created");
		break;
	case WavetableSound::Waveform::Triangle:
		sound.makeTriangle();
		DBG("Sound named 'Triangle' created");
		break;
	case WavetableSound::Waveform::Sawtooth:
		sound.makeSawtooth();
		DBG("Sound named 'Sawtooth' created");
		break;
	case WavetableSound::Waveform::Square:
		sound.makeSquare();
		DBG("Sound named 'Square' created");
		break;
	case WavetableSound::Waveform::Noise:
		sound.makeNoise();
		DBG("Sound named 'Noise' created");
		break;
	case WavetableSound::Waveform::Arbitrary:
		sound.makeArbitrary(wavetable);
		DBG("Sound named '" + name + "' created");
		break;
	}
}

ReferenceCountedSound::~ReferenceCountedSound()
{
	switch (type) {
	case WavetableSound::Waveform::Sine:
		DBG("Sound named 'Sine' destroyed");
		break;
	case WavetableSound::Waveform::Triangle:
		DBG("Sound named 'Triangle' destroyed");
		break;
	case WavetableSound::Waveform::Sawtooth:
		DBG("Sound named 'Sawtooth' destroyed");
		break;
	case WavetableSound::Waveform::Square:
		DBG("Sound named 'Square' destroyed");
		break;
	case WavetableSound::Waveform::Noise:
		DBG("Sound named 'Noise' destroyed");
		break;
	case WavetableSound::Waveform::Arbitrary:
		DBG("Sound named '" + name + "' destroyed");
		break;
	}
}

WavetableSound* ReferenceCountedSound::getSound()
{
	return &sound;
}
