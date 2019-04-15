/*
  ==============================================================================

    LoadingThread.h
    Created: 2 Apr 2019 4:38:20pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class LoadingThread : public Thread
{
public:
	LoadingThread(DragonWaveAudioProcessor& p);
	~LoadingThread();

	//==============================================================================
	void run() override;

private:

	enum OscType {
		Carrier,
		FM
	};

	DragonWaveAudioProcessor& processor;
	AudioFormatManager formatManager;
	Wavetable::Waveform previousCarrierWaveform = Wavetable::Waveform::Arbitrary;
	Wavetable::Waveform previousFMWaveform = Wavetable::Waveform::Arbitrary;

	//==============================================================================
	void checkForSoundsToFree();
	void checkForWaveformToSet();
	void checkForPathToOpen();
	void loadFromDisk(OscType type, String pathToOpen);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoadingThread)
};