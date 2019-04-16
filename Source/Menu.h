/*
  ==============================================================================

    Menu.h
    Created: 15 Apr 2019 10:40:38pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class Menu    : public Component, public Button::Listener
{
public:

	//==============================================================================
    Menu(DragonWaveAudioProcessor&);
    ~Menu();

	//==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	TextButton openButton;
	TextButton saveButton;
	TextButton resetButton;

	//==============================================================================
	Label presetLabel;

	//==============================================================================
	void buttonClicked(Button* button);
	void buttonStateChanged(Button* button);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Menu)
};
