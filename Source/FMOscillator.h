/*
  ==============================================================================

    FMOscillator.h
    Created: 14 Apr 2019 7:48:06pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class FMOscillator    : public Component,
	                    public Button::Listener
{
public:
    FMOscillator(DragonWaveAudioProcessor&);
    ~FMOscillator();

    void paint (Graphics&) override;
    void resized() override;

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	TextButton openButton;
	TextButton sineButton;
	TextButton triangleButton;
	TextButton sawtoothButton;
	TextButton squareButton;
	TextButton noiseButton;

	//==============================================================================
	Slider fmFrequencySlider;
	Slider fmDepthSlider;

	//==============================================================================
	Label fmFrequencyLabel;
	Label fmDepthLabel;

	//==============================================================================
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> fmFrequencyAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> fmDepthAttach;

	//==============================================================================
	void buttonClicked(Button* button);
	void buttonStateChanged(Button* button);
	void openButtonClicked();

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMOscillator)
};
