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
	carrierDeltas = std::vector<float>(1, 0.0f);
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
	carrierPtr = ReferenceCountedWavetable::Ptr(ws->carrier);
	fmPtr = ReferenceCountedWavetable::Ptr(ws->fm);

	carrier = carrierPtr->getWavetable();
	fm = fmPtr->getWavetable();

	// Set wavetable size
	wavetableSize = carrier->getWavetableSize();

	// Get bounding frequencies and indices
	auto carrierBFs = carrier->getBoundingFrequencies(frequency);
	auto fmBFs = fm->getBoundingFrequencies(fmFrequency);
	carrierBIs = carrier->getBoundingIndexes(carrierBFs);
	fmBIs = fm->getBoundingIndexes(fmBFs);

	// Set level based off velocity
	level = velocity * 0.15f;

	// Set unison
	carrierIndices = std::vector<float>(currentUnison, 0.0f);
	carrierDeltas = std::vector<float>(currentUnison, 0.0f);
	fmIndices = std::vector<float>(currentUnison, 0.0f);
	fmDeltas = std::vector<float>(currentUnison, 0.0f);

	// Set detune and spread amounts
	if (currentUnison == 1)
	{
		detuneAmounts = std::vector<float>(1, 0.0f);
		spreadAmounts = std::vector<float>(1, 0.0f);
	}
	else
	{
		detuneAmounts = Util::linspace(-detune, detune, currentUnison);
		spreadAmounts = Util::linspace(-1.0f, 1.0f, currentUnison);
	}

	// Set wavetable index and delta
	for (int i = 0; i < currentUnison; i++) {
		carrierDeltas[i] = (frequency + (frequency * detuneAmounts[i])) * wavetableSize / (float)getSampleRate();
		fmDeltas[i] = (frequency + (frequency * detuneAmounts[i])) * wavetableSize / (float)getSampleRate();
	}

	if (currentUnison > 1)
	{
		auto phases = Util::linspace(0.0, 0.375, currentUnison);
		for (int i = 0; i < currentUnison; i++)
		{
			carrierIndices[i] = phases[i] * (float)wavetableSize;
		}
	}

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

	// Setup FM filters
	fmFilters.clearQuick(true);
	for (int i = 0; i < currentUnison; i++)
	{
		IIRFilter* filter = new IIRFilter();
		setFilterParams(filter, fmFilterType, fmFilterCutoff, fmFilterQ);
		fmFilters.add(filter);
	}

	// Start attack phase
	carrierEnvelope.noteOn();
	carrierFilterEnvelope.noteOn();

	fmEnvelopes.clearQuick(true);
	for (int i = 0; i < currentUnison; i++)
	{
		ADSR* fmEnv = new ADSR();
		fmEnv->setSampleRate(getSampleRate());
		fmEnv->setParameters({ fmAttack, fmDecay, fmSustain, fmRelease });
		fmEnv->noteOn();
		fmEnvelopes.add(fmEnv);
	}

	previousUnison = currentUnison;
	noteIsActive = true;
}

void WavetableVoice::renderNextBlock(AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
	// Make sure we have a note to play
	if (noteIsActive)
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
			float currentFmSample = 0.0f;
			float currentCarrierSample = 0.0f;
			float gainFactor = 1.0f / (float)carrierDeltas.size();

			AudioSampleBuffer currentBuffer;
			currentBuffer.setSize(2, numSamples);
			currentBuffer.clear();

			for (int i = 0; i < previousUnison; i++)
			{
				for (int s = 0; s < numSamples; s++)
				{
					//==============================================================================
					// Modulate carrier frequency
					//==============================================================================

					float currentFmEnv = fmEnvelopes.getUnchecked(i)->getNextSample() * fmEnvLevel;

					// Get the indexes that surround the current index estimate
					auto indexBefore = (unsigned int)fmIndices[i];
					auto indexAfter = indexBefore + 1;

					// Find the fraction of how far current sample is towards the next
					auto frac = fmIndices[i] - (float)indexBefore;

					// Get next sample on lower wavetable
					auto* fmTableLo = fm->getWavetables().getReadPointer(fmBIs.first);
					auto valueBeforeLo = fmTableLo[indexBefore];
					auto valueAfterLo = fmTableLo[indexAfter];
					float currentSampleLo = valueBeforeLo + frac * (valueAfterLo - valueBeforeLo);

					// Get next sample on higher wavetable
					auto* fmTableHi = fm->getWavetables().getReadPointer(fmBIs.second);
					auto valueBeforeHi = fmTableHi[indexBefore];
					auto valueAfterHi = fmTableHi[indexAfter];
					float currentSampleHi = valueBeforeHi + frac * (valueAfterHi - valueBeforeHi);

					// Increment phase change and prevent overflow
					fmIndices[i] += fmDeltas[i];
					fmIndices[i] = std::fmod(fmIndices[i], (float)wavetableSize);

					// Mix samples from both wavetables
					currentFmSample = currentSampleLo * (1.0f - fmWavetableMix) + currentSampleHi * fmWavetableMix;
					currentFmSample = fmFilters.getUnchecked(i)->processSingleSampleRaw(currentFmSample);
					currentFmSample *= currentFmEnv;

					// Calculate new frequency
					float newFrequency = std::abs(frequency + (frequency * detuneAmounts[i]) + currentFmSample * fmDepth);
					carrierDeltas[i] = newFrequency * wavetableSize / (float)getSampleRate();

					//==============================================================================
					// Calculate current sample
					//==============================================================================

					// Get the indexes that surround the current index estimate
					indexBefore = (unsigned int)carrierIndices[i];
					indexAfter = indexBefore + 1;

					// Find the fraction of how far current sample is towards the next
					frac = carrierIndices[i] - (float)indexBefore;

					// Get next sample on lower wavetable
					auto* carrierTableLo = carrier->getWavetables().getReadPointer(carrierBIs.first);
					valueBeforeLo = carrierTableLo[indexBefore];
					valueAfterLo = carrierTableLo[indexAfter];
					currentSampleLo = valueBeforeLo + frac * (valueAfterLo - valueBeforeLo);

					// Get next sample on higher wavetable
					auto* carrierTableHi = carrier->getWavetables().getReadPointer(carrierBIs.second);
					valueBeforeHi = carrierTableHi[indexBefore];
					valueAfterHi = carrierTableHi[indexAfter];
					currentSampleHi = valueBeforeHi + frac * (valueAfterHi - valueBeforeHi);

					// Increment phase change and prevent overflow
					carrierIndices[i] += carrierDeltas[i];
					carrierIndices[i] = std::fmod(carrierIndices[i], (float)wavetableSize);

					// Mix samples from both wavetables 
					currentCarrierSample = currentSampleLo * (1.0f - carrierWavetableMix) + currentSampleHi * carrierWavetableMix;

					//==============================================================================
					// Mix down and spread
					//==============================================================================

					float leftSample = (currentCarrierSample + (currentCarrierSample * spreadAmounts[i] * spread)) * gainFactor;
					float rightSample = (currentCarrierSample - (currentCarrierSample * spreadAmounts[i] * spread)) * gainFactor;

					currentBuffer.addSample(0, s, leftSample);
					currentBuffer.addSample(1, s, rightSample);
				}
			}

			auto currentEnv = 0.0f;

			for (int s = 0; s < numSamples; s++)
			{
				//==============================================================================
				// Filter
				//==============================================================================

				float currentFilterEnvSample = carrierFilterEnvelope.getNextSample();
				float newCutoff = jmap(
					filterCutoffDirection == -1.0f ? 1 - currentFilterEnvSample : currentFilterEnvSample,
					filterCutoffLowerBound,
					filterCutoffUpperBound
				);

				setCarrierFilterParams((float)carrierFilterType, newCutoff, carrierFilterQ);
				float leftSample = carrierFilterL.processSingleSampleRaw(currentBuffer.getSample(0, s));
				float rightSample = carrierFilterR.processSingleSampleRaw(currentBuffer.getSample(1, s));

				//==============================================================================
				// Envelope
				//==============================================================================

				currentEnv = carrierEnvelope.getNextSample() * carrierEnvLevel;
				leftSample *= currentEnv * level;
				rightSample *= currentEnv * level;

				outputBuffer.addSample(0, startSample, leftSample);
				outputBuffer.addSample(1, startSample, rightSample);

				startSample++;
			}

			if (currentEnv < 0.001f) {
				clearCurrentNote();
				noteIsActive = false;
			}	
		}
	}
}

//==============================================================================
void WavetableVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
	if (allowTailOff)
	{
		carrierEnvelope.noteOff();
		carrierFilterEnvelope.noteOff();

		for (int i = 0; i < previousUnison; i++)
		{
			fmEnvelopes.getUnchecked(i)->noteOff();
		}
	}
	else
	{
		clearCurrentNote();
		noteIsActive = false;
	}
}

void WavetableVoice::pitchWheelMoved(int /*newPitchWheelValue*/)
{
}

void WavetableVoice::controllerMoved(int /*controllerNumber*/, int /*newControllerValue*/)
{
}

//==============================================================================
void WavetableVoice::setCarrierPitchShift(float* shift)
{
	pitchShift = (int)* shift;
}

void WavetableVoice::setCarrierFilterParams(float newType, float newCutoff, float newQ)
{
	carrierFilterType = (int)newType;
	carrierFilterCutoff = newCutoff;
	carrierFilterQ = newQ;

	setFilterParams(&carrierFilterL, carrierFilterType, carrierFilterCutoff, carrierFilterQ);
	setFilterParams(&carrierFilterR, carrierFilterType, carrierFilterCutoff, carrierFilterQ);
}

void WavetableVoice::setCarrierEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel)
{
	carrierEnvelope.setSampleRate(getSampleRate());
	carrierEnvelope.setParameters({ *newAttack, *newDecay, *newSustain, *newRelease });
	carrierEnvLevel = *newLevel;
}

void WavetableVoice::setCarrierFilterEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newDepth)
{
	carrierFilterEnvelope.setSampleRate(getSampleRate());
	carrierFilterEnvelope.setParameters({ *newAttack, *newDecay, *newSustain, *newRelease });
	carrierFilterEnvLevel = *newDepth;

	if (carrierFilterEnvLevel <= 0.0f)
	{
		filterCutoffLowerBound = 20.0f + (carrierFilterCutoff - 20.0f) * (1.0f + carrierFilterEnvLevel);
		filterCutoffUpperBound = carrierFilterCutoff + 0.001f;
		filterCutoffDirection = -1.0f;
	}
	else
	{
		filterCutoffLowerBound = carrierFilterCutoff;
		filterCutoffUpperBound = carrierFilterCutoff + (20000.0f - carrierFilterCutoff) * carrierFilterEnvLevel;
		filterCutoffDirection = 1.0f;
	}
}

//==============================================================================
void WavetableVoice::setFmOscParams(float* newFrequency, float* newDepth)
{
	// Update frequency modulation phase change
	fmFrequency = frequency * jmap(*newFrequency, 0.125f, 3.0f);

	for (int i = 0; i < previousUnison; i++)
		fmDeltas[i] = (fmFrequency + (fmFrequency * detuneAmounts[i])) * wavetableSize / (float)getSampleRate();

	// Update frequency modulation depth
	if (frequency > 0.0f)
		fmDepth = jmap(*newDepth, 0.0f, frequency);
	else
		fmDepth = 0.0f;
}

void WavetableVoice::setFmFilterParams(float* newType, float* newCutoff, float* newQ)
{
	fmFilterType = (int)* newType;
	fmFilterCutoff = *newCutoff;
	fmFilterQ = *newQ;

	for (int i = 0; i < previousUnison; i++)
		setFilterParams(fmFilters.getUnchecked(i), fmFilterType, fmFilterCutoff, fmFilterQ);
}

void WavetableVoice::setFmEnvParams(float* newAttack, float* newDecay, float* newSustain, float* newRelease, float* newLevel)
{
	fmAttack = *newAttack;
	fmDecay = *newDecay;
	fmSustain = *newSustain;
	fmRelease = *newRelease;

	for (int i = 0; i < previousUnison; i++)
	{
		fmEnvelopes.getUnchecked(i)->setSampleRate(getSampleRate());
		fmEnvelopes.getUnchecked(i)->setParameters({ fmAttack, fmDecay, fmSustain, fmRelease });
		fmEnvLevel = *newLevel;
	}
}

//==============================================================================
void WavetableVoice::setGlobalParams(float* newUnison, float* newDetune, float* newSpread)
{
	currentUnison = (int)* newUnison;
	detune = *newDetune;
	spread = *newSpread;

	if (previousUnison != 1)
		detuneAmounts = Util::linspace(-detune, detune, previousUnison);

	for (int i = 0; i < previousUnison; i++)
	{
		fmDeltas[i] = (fmFrequency + (fmFrequency * detuneAmounts[i])) * wavetableSize / (float)getSampleRate();
		carrierDeltas[i] = (frequency + (frequency * detuneAmounts[i])) * wavetableSize / (float)getSampleRate();
	}
}

//==============================================================================
void WavetableVoice::setFilterParams(IIRFilter * filter, int type, float cutoff, float q)
{
	switch (type)
	{
	case 0:
		filter->setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), (double)cutoff, (double)q));
		break;
	case 1:
		filter->setCoefficients(IIRCoefficients::makeHighPass(getSampleRate(), (double)cutoff, (double)q));
		break;
	case 2:
		filter->setCoefficients(IIRCoefficients::makeBandPass(getSampleRate(), (double)cutoff, (double)q));
		break;
	case 3:
		filter->setCoefficients(IIRCoefficients::makeNotchFilter(getSampleRate(), (double)cutoff, (double)q));
		break;
	case 4:
		filter->setCoefficients(IIRCoefficients::makeAllPass(getSampleRate(), (double)cutoff, (double)q));
		break;
	default:
		filter->setCoefficients(IIRCoefficients::makeLowPass(getSampleRate(), (double)cutoff, (double)q));
	}
}

