/*
  ==============================================================================

    Global.h
    Created: 20 Apr 2019 10:04:12pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class Global    : public Component
{
public:

	//==============================================================================
    Global(DragonWaveAudioProcessor& p);
    ~Global();

	//==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	Slider unisonSlider;
	Slider detuneSlider;
	Slider spreadSlider;
	Slider masterSlider;

	//==============================================================================
	Label unisonLabel;
	Label detuneLabel;
	Label spreadLabel;
	Label masterLabel;

	//==============================================================================
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> unisonAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> detuneAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> spreadAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> masterAttach;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Global)
};
