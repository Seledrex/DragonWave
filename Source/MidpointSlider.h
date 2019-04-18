/*
  ==============================================================================

    MidpointSlider.h
    Created: 17 Apr 2019 11:18:00pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MidpointSlider : public LookAndFeel_V4
{
public:
	MidpointSlider();

	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;
	
};