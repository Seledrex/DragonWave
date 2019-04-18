/*
  ==============================================================================

    MidpointSlider.cpp
    Created: 17 Apr 2019 11:18:00pm
    Author:  Eric

  ==============================================================================
*/

#include "MidpointSlider.h"

MidpointSlider::MidpointSlider()
{
}

void MidpointSlider::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
	float sliderPos,
	float minSliderPos,
	float maxSliderPos,
	const Slider::SliderStyle style, Slider& slider)
{
	ignoreUnused(minSliderPos);
	ignoreUnused(maxSliderPos);
	ignoreUnused(style);

	if (slider.isBar())
	{
		g.setColour(slider.findColour(Slider::trackColourId));
		g.fillRect(slider.isHorizontal() ? Rectangle<float>(static_cast<float> (x), y + 0.5f, sliderPos - x, height - 1.0f)
			: Rectangle<float>(x + 0.5f, sliderPos, width - 1.0f, y + (height - sliderPos)));
	}
	else
	{
		auto trackWidth = jmin(6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);

		Point<float> startPoint(slider.isHorizontal() ? x : x + width * 0.5f,
			slider.isHorizontal() ? y + height * 0.5f : height + y);

		Point<float> endPoint(slider.isHorizontal() ? width + x : startPoint.x,
			slider.isHorizontal() ? startPoint.y : y);

		Point<float> midPoint((startPoint.x + endPoint.x) / 2, (startPoint.y + endPoint.y) / 2);

		Path backgroundTrack;
		backgroundTrack.startNewSubPath(startPoint);
		backgroundTrack.lineTo(endPoint);
		g.setColour(slider.findColour(Slider::backgroundColourId));
		g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

		Path valueTrack;
		Point<float> minPoint, maxPoint, thumbPoint;

		auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
		auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;

		minPoint = startPoint;
		maxPoint = { kx, ky };

		auto thumbWidth = getSliderThumbRadius(slider);

		valueTrack.startNewSubPath(maxPoint);
		valueTrack.lineTo(midPoint);
		g.setColour(slider.findColour(Slider::trackColourId));
		g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

		g.setColour(slider.findColour(Slider::thumbColourId));
		g.fillEllipse(Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(maxPoint));
	}
}
