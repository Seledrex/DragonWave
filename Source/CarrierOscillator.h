/*
  ==============================================================================

    CarrierOscillator.h
    Created: 8 Apr 2019 2:31:17am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class CarrierOscillator    : public Component,
	                         public Button::Listener
{
public:
    CarrierOscillator(DragonWaveAudioProcessor&);
    ~CarrierOscillator();

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
	Slider pitchSlider;
	Slider voicesSlider;

	//==============================================================================
	Label pitchLabel;
	Label voicesLabel;

	//==============================================================================
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> pitchAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> voicesAttach;

	//==============================================================================
	void buttonClicked(Button* button);
	void buttonStateChanged(Button* button);
	void openButtonClicked();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CarrierOscillator)
};
