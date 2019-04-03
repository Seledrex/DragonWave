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
class DragonWaveAudioProcessorEditor : public AudioProcessorEditor,
	                                   public Timer,
	                                   public Button::Listener
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
	TextButton sineButton;
	TextButton triangleButton;
	TextButton sawtoothButton;
	TextButton squareButton;
	TextButton noiseButton;

	//==============================================================================
	int frameCounter;

	//==============================================================================
	void timerCallback() override;
	int getFrameCounter();

	//==============================================================================
	void openButtonClicked();

	//==============================================================================
	void buttonClicked(Button* button);
	void buttonStateChanged(Button* button);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonWaveAudioProcessorEditor)
};
