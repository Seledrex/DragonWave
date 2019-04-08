/*
  ==============================================================================

    Filter.cpp
    Created: 8 Apr 2019 2:32:20am
    Author:  Eric

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

//==============================================================================
Filter::Filter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Filter::~Filter()
{
}

void Filter::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Filter", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Filter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
