/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DragonWaveAudioProcessorEditor : public AudioProcessorEditor, public Timer
{
public:
	DragonWaveAudioProcessorEditor(DragonWaveAudioProcessor&);
	~DragonWaveAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	int tableSize = 2048;
	float animationWidth = 200.0f;
	float animationHeight = 100.0f;
	NormalisableRange<float> yRange;
	NormalisableRange<float> xRange;

	//==============================================================================
	TextButton openButton;
	AudioFormatManager formatManager;

	//==============================================================================
	int frameCounter;

	//==============================================================================
	void timerCallback() override;
	int getFrameCounter();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonWaveAudioProcessorEditor)
};
