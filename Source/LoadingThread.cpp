/*
  ==============================================================================

    LoadingThread.cpp
    Created: 2 Apr 2019 4:38:20pm
    Author:  Eric

  ==============================================================================
*/

#include "LoadingThread.h"
#include "PluginProcessor.h"
#include "WavetableSound.h"

LoadingThread::LoadingThread(DragonWaveAudioProcessor& p)
	: Thread("LoadingThread"), processor(p)
{
	// Set up audio format manager and start the thread
	formatManager.registerBasicFormats();
	startThread();
}

LoadingThread::~LoadingThread()
{
	// Give time for thread to finish up
	stopThread(4000);
}

//==============================================================================
void LoadingThread::run()
{
	// Perform these checks at a regular time interval
	while (!threadShouldExit())
	{
		checkForWaveformToSet();
		checkForPathToOpen();
		checkForSoundsToFree();
		wait(500);
	}
}

//==============================================================================
void LoadingThread::checkForSoundsToFree()
{
	// If a sound's reference count is too low, it is done being used and is safe for removal
	for (auto i = processor.carrierWavetables.size(); --i >= 0;)
	{
		ReferenceCountedWavetable::Ptr wavetable(processor.carrierWavetables.getUnchecked(i));

		if (wavetable->getReferenceCount() == 2)
			processor.carrierWavetables.remove(i);
	}

	for (auto i = processor.fmWavetables.size(); --i >= 0;)
	{
		ReferenceCountedWavetable::Ptr wavetable(processor.fmWavetables.getUnchecked(i));

		if (wavetable->getReferenceCount() == 2)
			processor.fmWavetables.remove(i);
	}
}

void LoadingThread::checkForWaveformToSet()
{
	auto retainedChosenCarrierWaveform = processor.chosenCarrierWaveform;
	auto retainedChosenFMWaveform = processor.chosenFMWaveform;

	// Check to see if a new carrier waveform was chosen
	if (previousCarrierWaveform != retainedChosenCarrierWaveform)
	{
		ReferenceCountedWavetable::Ptr newWavetable = new ReferenceCountedWavetable("", retainedChosenCarrierWaveform);

		processor.currentCarrierWavetable = newWavetable;
		processor.carrierWavetables.add(newWavetable);

		processor.synth.clearSounds();
		processor.synth.addSound(new WavetableSound(
			ReferenceCountedWavetable::Ptr(newWavetable),
			ReferenceCountedWavetable::Ptr(processor.currentFMWavetable)
		));

		previousCarrierWaveform = retainedChosenCarrierWaveform;
	}

	// Check to see if a new fm waveform was chosen
	if (previousFMWaveform != retainedChosenFMWaveform)
	{
		ReferenceCountedWavetable::Ptr newWavetable = new ReferenceCountedWavetable("", retainedChosenFMWaveform);

		processor.currentFMWavetable = newWavetable;
		processor.fmWavetables.add(newWavetable);

		processor.synth.clearSounds();
		processor.synth.addSound(new WavetableSound(
			ReferenceCountedWavetable::Ptr(processor.currentCarrierWavetable),
			ReferenceCountedWavetable::Ptr(newWavetable)
		));

		previousFMWaveform = retainedChosenFMWaveform;
	}
}

void LoadingThread::checkForPathToOpen()
{
	String carrierPathToOpen;
	carrierPathToOpen.swapWith(processor.chosenCarrierPath);

	if (carrierPathToOpen.isNotEmpty())
		loadFromDisk(OscType::Carrier, carrierPathToOpen);
	
	String fmPathToOpen;
	fmPathToOpen.swapWith(processor.chosenFMPath);

	if (fmPathToOpen.isNotEmpty())
		loadFromDisk(OscType::FM, fmPathToOpen);
}

void LoadingThread::loadFromDisk(OscType type, String pathToOpen)
{
	// Open file and create file reader
	File file(pathToOpen);
	std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

	if (reader.get() != nullptr)
	{
		// Make sure that the wave file has the correct number of samples
		if (reader->lengthInSamples == Wavetable::wavetableSize)
		{
			// Read the wave file into a buffer
			AudioSampleBuffer buffer;
			buffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
			reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

			// Convert the buffer into a float vector
			std::vector<float> wavetable(buffer.getNumSamples());
			for (int i = 0; i < wavetable.size(); i++)
				wavetable[i] = buffer.getSample(0, i);

			// Generate a new wavetable
			ReferenceCountedWavetable::Ptr newWavetable = new ReferenceCountedWavetable(
				file.getFullPathName(),
				Wavetable::Waveform::Arbitrary,
				wavetable);

			if (type == OscType::Carrier)
			{
				// Update current sound and sound array
				processor.currentCarrierWavetable = newWavetable;
				processor.carrierWavetables.add(newWavetable);

				// Update synth with a copy of the new sound
				processor.synth.clearSounds();
				processor.synth.addSound(new WavetableSound(
					ReferenceCountedWavetable::Ptr(newWavetable),
					ReferenceCountedWavetable::Ptr(processor.currentFMWavetable)
				));

				// Update chosen waveform
				previousCarrierWaveform = Wavetable::Waveform::Arbitrary;
				processor.chosenCarrierWaveform = Wavetable::Waveform::Arbitrary;
			}
			else
			{
				// Update current sound and sound array
				processor.currentFMWavetable = newWavetable;
				processor.fmWavetables.add(newWavetable);

				// Update synth with a copy of the new sound
				processor.synth.clearSounds();
				processor.synth.addSound(new WavetableSound(
					ReferenceCountedWavetable::Ptr(processor.currentCarrierWavetable),
					ReferenceCountedWavetable::Ptr(newWavetable)
				));

				// Update chosen waveform
				previousFMWaveform = Wavetable::Waveform::Arbitrary;
				processor.chosenFMWaveform = Wavetable::Waveform::Arbitrary;
			}
		}
		else
		{
			// Show error message if wave file does not have correct number of samples
			AlertWindow::showMessageBox(
				AlertWindow::AlertIconType::WarningIcon,
				"Error Loading Wavetable",
				"Wavetable must only contain " + String(Wavetable::wavetableSize) + " samples.");
		}
	}
}
