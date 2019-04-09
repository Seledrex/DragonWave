/*
  ==============================================================================

    Filter.h
    Created: 8 Apr 2019 2:32:20am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class Filter    : public Component
{
public:
    Filter(DragonWaveAudioProcessor& p, String title, String cutoffId, String qId, String typeId);
    ~Filter();

	//==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	DragonWaveAudioProcessor& processor;
	String title;

	//==============================================================================
	Slider filterCutoff;
	Slider filterQ;

	//==============================================================================
	ComboBox filterMenu;

	//==============================================================================
	Label filterCutoffLabel;
	Label filterQLabel;
	Label filterMenuLabel;

	//==============================================================================
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> filterQAttach;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filterMenuAttach;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
