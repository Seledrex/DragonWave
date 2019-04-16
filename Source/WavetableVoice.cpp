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
	frequency = (float)MidiMessage::getMidiNoteInHertz(midiNoteNumber + pitchShift);

	// Get the wavetable information
	ws = dynamic_cast<WavetableSound*>(sound);
	carrier = ws->carrier->getWavetable();
	fm = ws->fm->getWavetable();

	// Set wavetable size
	wavetableSize = carrier->getWavetableSize();

	// Get bounding frequencies and indices
	auto carrierBFs = carrier->getBoundingFrequencies(frequency);
	auto fmBFs = fm->getBoundingFrequencies(fmFrequency);
	carrierBIs = carrier->getBoundingIndexes(carrierBFs);
	fmBIs = fm->getBoundingIndexes(fmBFs);

	// Set level based off velocity
	level = velocity * 0.15f;

	// Set carrier oscillator wavetable index and delta
	currentCarrierIndex = 0.0f;
	carrierTableDelta = frequency * wavetableSize / (float)getSampleRate();

	// Set FM oscillator wavetable index and delta
	currentFmIndex = 0.0f;
	fmTableDelta = frequency * wavetableSize / (float)getSampleRate();

	// Calculate carrier oscillator mixing proportions
	if (carrierBFs.first != carrierBFs.second)
		carrierWavetableMix = (frequency - carrierBFs.first) / (carrierBFs.second - carrierBFs.first);
	else
		carrierWavetableMix = 1;

	// Calculate FM oscillator mixing proportions
	if (fmBFs.first != fmBFs.second)
		fmWavetableMix = (frequency - fmBFs.first) / (fmBFs.second - fmBFs.first);
	else
		fmWavetableMix = 1;

	// Start attack phase
	carrierEnvelope.noteOn();
	fmEnvelope.noteOn();
}

void WavetableVoice::renderNextBlock(AudioSampleBuffer & outputBuffer, int startSample, int numSamples)
{
	// Make sure we have a note to play
	if (carrierTableDelta != 0)
	{
		if (carrier->getWaveformType() == Wavetable::Waveform::Noise)
		{
			while (--numSamples >= 0)
			{
				auto currentEnv = carrierEnvelope.getNextSample();
				auto currentSample = (random.nextFloat() * 2 - 1) * currentEnv * carrierEnvLevel * level;

				for (auto i = 0; i < outputBuffer.getNumChannels(); i++)
					outputBuffer.addSample(i, startSample, currentSample);

				startSample++;
			}
		}
		else
		{
			while (--numSamples >= 0)
			{
				// Modulate carrier frequency
				modulateFrequency();

				// Get next envelope value
				auto currentEnv = carrierEnvelope.getNextSample() * carrierEnvLevel;

				// Calculate current sample
				auto currentSample = getNextSample() * currentEnv * level;
				
				// Place sample into output buffer
				for (auto i = 0; i < outputBuffer.getNumChannels(); i++)
					outputBuffer.addSample(i, startSample, currentSample);

				startSample++;
			}
		}
	}
}

void WavetableVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
	if (allowTailOff)
	{
		carrierEnvelope.noteOff();
		fmEnvelope.noteOff();
	}
	else
	{
		clearCurrentNote();
		carrierTableDelta = 0;
	}
}

void WavetableVoice::pitchWheelMoved(int /*newPitchWheelValue*/)
{
}

void WavetableVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
}

void WavetableVoice::setCarrierPitchShift(float* shift)
{
	pitchShift = (int)*shift;
}

void WavetableVoice::setCarrierFilterParams(float* newType, float* newCutoff, float* newQ)
{
	int type = (int)* newType;
	double cutoff = (double)* newCutoff;
	double q = (double)* newQ;

	switch (type)
	{
	case 0:
		carrierFilter.setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), cutoff, q));
		break;
	case 1:
		carrierFilter.setCoefficients(IIRCoefficients::makeHighPass(getSampleRate(), cutoff, q));
		break;
	case 2:
		carrierFilter.setCoefficients(IIRCoefficients::makeBandPass(getSampleRate(), cutoff, q));
		break;
	case 3:
		carrierFilter.setCoefficients(IIRCoefficients::makeNotchFilter(getSampleRate(), cutoff, q));
		break;
	case 4:
		carrierFilter.setCoefficients(IIRCoefficients::makeAllPass(getSampleRate(), cutoff, q));
		break;
	default:
		carrierFilter.setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), cutoff, q));
	}
}

void WavetableVoice::setCarrierEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel)
{
	carrierEnvelope.setSampleRate(getSampleRate());
	carrierEnvelope.setParameters({ *newAttack, *newDecay, *newSustain, *newRelease });
	carrierEnvLevel = *newLevel;
}

void WavetableVoice::setFmOscParams(float* newFrequency, float* newDepth)
{
	// Update frequency modulation phase change
	auto frequencyMultiplierRange = NormalisableRange<float>(0.125f, 3.0f);
	float newFmFrequency = frequency * frequencyMultiplierRange.convertFrom0to1(*newFrequency);
	fmTableDelta = newFmFrequency * wavetableSize / (float)getSampleRate();

	// Update frequency modulation depth
	if (frequency > 0.0f) {
		auto depthRange = NormalisableRange<float>(0.0f, frequency);
		fmDepth = depthRange.convertFrom0to1(*newDepth);
	}
	else
		fmDepth = 0.0f;
	
}

void WavetableVoice::setFmFilterParams(float* newType, float* newCutoff, float* newQ)
{
	int type = (int)* newType;
	double cutoff = (double)* newCutoff;
	double q = (double)* newQ;

	switch (type)
	{
	case 0:
		fmFilter.setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), cutoff, q));
		break;
	case 1:
		fmFilter.setCoefficients(IIRCoefficients::makeHighPass(getSampleRate(), cutoff, q));
		break;
	case 2:
		fmFilter.setCoefficients(IIRCoefficients::makeBandPass(getSampleRate(), cutoff, q));
		break;
	case 3:
		fmFilter.setCoefficients(IIRCoefficients::makeNotchFilter(getSampleRate(), cutoff, q));
		break;
	case 4:
		fmFilter.setCoefficients(IIRCoefficients::makeAllPass(getSampleRate(), cutoff, q));
		break;
	default:
		fmFilter.setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), cutoff, q));
	}
}

void WavetableVoice::setFmEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel)
{
	fmEnvelope.setSampleRate(getSampleRate());
	fmEnvelope.setParameters({ *newAttack, *newDecay, *newSustain, *newRelease });
	fmEnvLevel = *newLevel;
}

forcedinline float WavetableVoice::getNextSample() noexcept
{
	// Get the indexes that surround the current index estimate
	auto indexBefore = (unsigned int)currentCarrierIndex;
	auto indexAfter = indexBefore + 1;

	// Find the fraction of how far current sample is towards the next
	auto frac = currentCarrierIndex - (float)indexBefore;

	// Get next sample on lower wavetable
	auto* tableLo = carrier->getWavetables().getReadPointer(carrierBIs.first);
	auto valueBeforeLo = tableLo[indexBefore];
	auto valueAfterLo = tableLo[indexAfter];
	float currentSampleLo = valueBeforeLo + frac * (valueAfterLo - valueBeforeLo);

	// Get next sample on higher wavetable
	auto * tableHi = carrier->getWavetables().getReadPointer(carrierBIs.second);
	auto valueBeforeHi = tableHi[indexBefore];
	auto valueAfterHi = tableHi[indexAfter];
	float currentSampleHi = valueBeforeHi + frac * (valueAfterHi - valueBeforeHi);

	// Increment phase change and prevent overflow
	currentCarrierIndex += carrierTableDelta;
	currentCarrierIndex = std::fmod(currentCarrierIndex, (float)wavetableSize);

	// Mix samples from both wavetables
	auto currentSample = currentSampleLo * (1.0f - carrierWavetableMix) + currentSampleHi * carrierWavetableMix;

	// Apply filter
	currentSample = carrierFilter.processSingleSampleRaw(currentSample);

	return currentSample;
}

forcedinline void WavetableVoice::modulateFrequency() noexcept
{
	// Get the indexes that surround the current index estimate
	auto indexBefore = (unsigned int)currentFmIndex;
	auto indexAfter = indexBefore + 1;

	// Find the fraction of how far current sample is towards the next
	auto frac = currentFmIndex - (float)indexBefore;

	// Get next sample on lower wavetable
	auto* tableLo = fm->getWavetables().getReadPointer(fmBIs.first);
	auto valueBeforeLo = tableLo[indexBefore];
	auto valueAfterLo = tableLo[indexAfter];
	float currentSampleLo = valueBeforeLo + frac * (valueAfterLo - valueBeforeLo);

	// Get next sample on higher wavetable
	auto * tableHi = fm->getWavetables().getReadPointer(fmBIs.second);
	auto valueBeforeHi = tableHi[indexBefore];
	auto valueAfterHi = tableHi[indexAfter];
	float currentSampleHi = valueBeforeHi + frac * (valueAfterHi - valueBeforeHi);

	// Increment phase change and prevent overflow
	currentFmIndex += fmTableDelta;
	currentFmIndex = std::fmod(currentFmIndex, (float)wavetableSize);

	// Mix samples from both wavetables
	auto currentSample = currentSampleLo * (1.0f - fmWavetableMix) + currentSampleHi * fmWavetableMix;
	currentSample = fmFilter.processSingleSampleRaw(currentSample);

	// Apply envelope
	float currentFmEnv = fmEnvelope.getNextSample() * fmEnvLevel;
	currentSample *= currentFmEnv;

	// Calculate new frequency
	float newFrequency = std::abs(frequency + currentSample * fmDepth);
	carrierTableDelta = newFrequency * wavetableSize / (float)getSampleRate();
}
