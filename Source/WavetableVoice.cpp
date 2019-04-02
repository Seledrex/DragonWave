/*
  ==============================================================================

	WavetableVoice.cpp
	Created: 22 Mar 2019 12:14:50pm
	Author:  Eric

  ==============================================================================
*/

#include "WavetableVoice.h"
#include "WavetableSound.h"

WavetableVoice::WavetableVoice()
{
}

bool WavetableVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<WavetableSound*> (sound) != nullptr;
}

void WavetableVoice::startNote(
	int midiNoteNumber,
	float velocity,
	SynthesiserSound* sound,
	int /*currentPitchWheelPosition*/)
{
	// Convert MIDI to frequency
	float frequency = (float)MidiMessage::getMidiNoteInHertz(midiNoteNumber);

	// Get the wavetable information
	wavetableSound = dynamic_cast<WavetableSound*>(sound);
	boundingFrequencies = wavetableSound->getBoundingFrequencies(frequency);
	boundingIndexes = wavetableSound->getBoundingIndexes(boundingFrequencies);
	wavetableSize = wavetableSound->getWavetableSize();

	// Set voice data
	level = velocity * 0.15f;
	currentIndex = 0.0;
	tableDelta = frequency * wavetableSize / (float)getSampleRate();

	// Calculate mixing proportions
	if (boundingFrequencies.first != boundingFrequencies.second)
		wavetableMix = (frequency - boundingFrequencies.first) / (boundingFrequencies.second - boundingFrequencies.first);
	else
		wavetableMix = 1;
}

void WavetableVoice::renderNextBlock(AudioSampleBuffer & outputBuffer, int startSample, int numSamples)
{
	// Make sure we have a note to play
	if (tableDelta != 0)
	{
		while (--numSamples >= 0)
		{
			// Calculate current sample
			auto currentSample = getNextSample() * level;

			// Place sample into output buffer
			for (auto i = 0; i < outputBuffer.getNumChannels(); i++)
				outputBuffer.addSample(i, startSample, currentSample);

			startSample++;
		}
	}
}

void WavetableVoice::stopNote(float /*velocity*/, bool /*allowTailOff*/)
{
	clearCurrentNote();
	tableDelta = 0;
}

void WavetableVoice::pitchWheelMoved(int /*newPitchWheelValue*/)
{
}

void WavetableVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
}

forcedinline float WavetableVoice::getNextSample() noexcept
{
	// Get the indexes that surround the current index estimate
	auto indexBefore = (unsigned int)currentIndex;
	auto indexAfter = indexBefore + 1;

	// Find the fraction of how far current sample is towards the next
	auto frac = currentIndex - (float)indexBefore;

	// Get next sample on lower wavetable
	auto* tableLo = wavetableSound->getWavetables().getReadPointer(boundingIndexes.first);
	auto valueBeforeLo = tableLo[indexBefore];
	auto valueAfterLo = tableLo[indexAfter];
	float currentSampleLo = valueBeforeLo + frac * (valueAfterLo - valueBeforeLo);

	// Get next sample on higher wavetable
	auto * tableHi = wavetableSound->getWavetables().getReadPointer(boundingIndexes.second);
	auto valueBeforeHi = tableHi[indexBefore];
	auto valueAfterHi = tableHi[indexAfter];
	float currentSampleHi = valueBeforeHi + frac * (valueAfterHi - valueBeforeHi);

	// Increment phase change and prevent overflow
	currentIndex += tableDelta;
	currentIndex = std::fmod(currentIndex, (float)wavetableSize);

	// Mix samples from both wavetables
	auto currentSample = currentSampleLo * (1.0f - wavetableMix) + currentSampleHi * wavetableMix;
	return currentSample;
}