/*
  ==============================================================================

    ReferenceCountedSound.cpp
    Created: 2 Apr 2019 4:22:35pm
    Author:  Eric

  ==============================================================================
*/

#include "ReferenceCountedSound.h"

/*
This class is used to keep track of how many references are associated with a loaded
sound. This is to ensure that the multithreading will work seemlessly.
*/
ReferenceCountedSound::ReferenceCountedSound(const String& path,
											 WavetableSound::Waveform type,
											 const std::vector<float> wavetable)
	: path(path), sound()
{
	this->type = type;

	// Create a sound using the specified type
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
		DBG("Sound from '" + path + "' created");
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
		DBG("Sound from '" + path + "' destroyed");
		break;
	}
}

//==============================================================================
WavetableSound* ReferenceCountedSound::getSound()
{
	return &sound;
}

String ReferenceCountedSound::getPath()
{
	return path;
}
