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
	void setFrameCount(int frameCount);
	void setSound(ReferenceCountedSound::Ptr retainedCurrentSound);

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	String title;
	ReferenceCountedSound::Ptr retainedCurrentSound;

	//==============================================================================
	int frameCount = 0;
	int wavetableSize = WavetableSound::wavetableSize;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscilloscope)
};
