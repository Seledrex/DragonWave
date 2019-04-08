/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscilloscope.h"
#include "CarrierOscillator.h"

//==============================================================================
/**
*/
class DragonWaveAudioProcessorEditor : public AudioProcessorEditor,
	                                   public Timer
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
	Oscilloscope carrierOscilloscopeGUI;
	CarrierOscillator carrierOscGUI;

	//==============================================================================
	int frameCounter;
	void timerCallback() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragonWaveAudioProcessorEditor)
};
