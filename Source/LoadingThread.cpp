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
	formatManager.registerBasicFormats();
	previousWaveform = WavetableSound::Waveform::Arbitrary;
	startThread();
}

LoadingThread::~LoadingThread()
{
	stopThread(4000);
}

void LoadingThread::run()
{
	while (!threadShouldExit())
	{
		checkForWaveformToSet();
		checkForPathToOpen();
		checkForSoundsToFree();
		wait(500);
	}
}

void LoadingThread::checkForSoundsToFree()
{
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

	if (pathToOpen.isNotEmpty())
	{
		File file(pathToOpen);
		std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));
		
		if (reader.get() != nullptr)
		{
			if (reader->lengthInSamples == WavetableSound::wavetableSize)
			{
				AudioSampleBuffer buffer;
				buffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
				reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

				std::vector<float> wavetable(buffer.getNumSamples());
				for (int i = 0; i < wavetable.size(); i++)
					wavetable[i] = buffer.getSample(0, i);

				ReferenceCountedSound::Ptr newSound = new ReferenceCountedSound(
					file.getFullPathName(),
					WavetableSound::Waveform::Arbitrary,
					wavetable);

				processor.currentSound = newSound;
				processor.sounds.add(newSound);

				processor.synth.clearSounds();
				processor.synth.addSound(new WavetableSound(*processor.currentSound->getSound()));

				previousWaveform = WavetableSound::Waveform::Arbitrary;
				processor.chosenWaveform = WavetableSound::Waveform::Arbitrary;
			}
			else
			{
				AlertWindow::showMessageBox(
					AlertWindow::AlertIconType::WarningIcon,
					"Error Loading Wavetable",
					"Wavetable must only contain " + String(WavetableSound::wavetableSize) + " samples.");
			}
		}
	}
}
