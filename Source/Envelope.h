/*
  ==============================================================================

    Envelope.h
    Created: 12 Apr 2019 3:36:04pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MidpointSlider.h"

//==============================================================================

class Envelope    : public Component
{
public:
    Envelope(DragonWaveAudioProcessor& p, String title,
		String attackId, String decayId, String sustainId, String releaseId,
		String levelId, bool isFilterEnvelope = false);
    ~Envelope();

    void paint (Graphics&) override;
    void resized() override;

private:
	DragonWaveAudioProcessor& processor;
	MidpointSlider midpointSliderLAF;

	//==============================================================================
	String title;

	//==============================================================================
	Slider attackSlider;
	Slider decaySlider;
	Slider sustainSlider;
	Slider releaseSlider;
	Slider levelSlider;

	//==============================================================================
	Label attackLabel;
	Label decayLabel;
	Label sustainLabel;
	Label releaseLabel;
	Label levelLabel;

	//==============================================================================
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> decayAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sustainAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> levelAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope)
};
