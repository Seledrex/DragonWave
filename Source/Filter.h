/*
  ==============================================================================

    Filter.h
    Created: 8 Apr 2019 2:32:20am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Filter    : public Component
{
public:
    Filter();
    ~Filter();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
