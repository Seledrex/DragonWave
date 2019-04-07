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

	//==============================================================================
	void checkForSoundsToFree();
	void checkForWaveformToSet();
	void checkForPathToOpen();

private:
	DragonWaveAudioProcessor& processor;
	AudioFormatManager formatManager;
	WavetableSound::Waveform previousWaveform = WavetableSound::Waveform::Arbitrary;;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoadingThread)
};