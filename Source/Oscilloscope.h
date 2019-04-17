/*
  ==============================================================================

    Oscilloscope.h
    Created: 7 Apr 2019 11:25:43pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Oscilloscope    : public Component
{
public:
    Oscilloscope(DragonWaveAudioProcessor& p, String title);
    ~Oscilloscope();

	//==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	//=============================================================================
	void setSound(ReferenceCountedWavetable::Ptr sound);

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	String title;
	ReferenceCountedWavetable::Ptr retainedCurrentWavetable;

	//==============================================================================
	int downsamplingFactor = 2;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscilloscope)
};
