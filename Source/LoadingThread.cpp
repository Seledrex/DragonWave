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
	for (auto i = processor.sounds.size(); --i >= 0;)
	{
		ReferenceCountedSound::Ptr sound(processor.sounds.getUnchecked(i));

		if (sound->getReferenceCount() == 2)
			processor.sounds.remove(i);
	}
}

void LoadingThread::checkForWaveformToSet()
{
	auto retainedChosenWaveform = processor.chosenWaveform;

	// Check to see if a basic waveform was chosen
	if (previousWaveform != retainedChosenWaveform)
	{
		ReferenceCountedSound::Ptr newSound = new ReferenceCountedSound("", retainedChosenWaveform);

		processor.currentSound = newSound;
		processor.sounds.add(newSound);

		processor.synth.clearSounds();
		processor.synth.addSound(new WavetableSound(*processor.currentSound->getSound()));

		previousWaveform = retainedChosenWaveform;
	}
}

void LoadingThread::checkForPathToOpen()
{
	String pathToOpen;
	pathToOpen.swapWith(processor.chosenPath);

	// Check if a wavetable file path was given
	if (pathToOpen.isNotEmpty())
	{
		// Open file and create file reader
		File file(pathToOpen);
		std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
		
		if (reader.get() != nullptr)
		{
			// Make sure that the wave file has the correct number of samples
			if (reader->lengthInSamples == WavetableSound::wavetableSize)
			{
				// Read the wave file into a buffer
				AudioSampleBuffer buffer;
				buffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
				reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

				// Convert the buffer into a float vector
				std::vector<float> wavetable(buffer.getNumSamples());
				for (int i = 0; i < wavetable.size(); i++)
					wavetable[i] = buffer.getSample(0, i);

				// Generate a new wavetable sound
				ReferenceCountedSound::Ptr newSound = new ReferenceCountedSound(
					file.getFullPathName(),
					WavetableSound::Waveform::Arbitrary,
					wavetable);

				// Update current sound and sound array
				processor.currentSound = newSound;
				processor.sounds.add(newSound);

				// Update synth with a copy of the new sound
				processor.synth.clearSounds();
				processor.synth.addSound(new WavetableSound(*processor.currentSound->getSound()));

				// Update chosen waveform
				previousWaveform = WavetableSound::Waveform::Arbitrary;
				processor.chosenWaveform = WavetableSound::Waveform::Arbitrary;
			}
			else
			{
				// Show error message if wave file does not have correct number of samples
				AlertWindow::showMessageBox(
					AlertWindow::AlertIconType::WarningIcon,
					"Error Loading Wavetable",
					"Wavetable must only contain " + String(WavetableSound::wavetableSize) + " samples.");
			}
		}
	}
}
