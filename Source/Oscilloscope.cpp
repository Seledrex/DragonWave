/*
  ==============================================================================

    Oscilloscope.cpp
    Created: 7 Apr 2019 11:25:43pm
    Author:  Eric

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscilloscope.h"

//==============================================================================
Oscilloscope::Oscilloscope(DragonWaveAudioProcessor& p, String title) :
	processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);
	this->title = title;
}

Oscilloscope::~Oscilloscope()
{
}

void Oscilloscope::paint (Graphics& g)
{
	// Fill background
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

	// Draw title text
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 20);
	g.setColour(Colours::white);
	g.drawText(title, titleArea, Justification::centred);

	// Create area for component
	float areaX = (float)Constants::PADDING;
	float areaY = (float)titleArea.getPosition().getY() + (float)titleArea.getHeight() + (float)Constants::PADDING;
	float areaWidth = (float)Constants::COMPONENT_WIDTH - 2 * Constants::PADDING;
	float areaHeight = Constants::COMPONENT_HEIGHT - Constants::PADDING - areaY;
	Rectangle<float> area(areaX, areaY, areaWidth, areaHeight);

	// Calculate animation height and width
	float animationWidth = areaWidth - Constants::PADDING * 2;
	float animationHeight = areaHeight - Constants::PADDING * 2;

	// Do not draw waveform if there is none to draw
	if (retainedCurrentWavetable == nullptr) {
		return;
	}

	// Get wavetable
	auto* wavetable = retainedCurrentWavetable->getWavetable();

	// Downsample wavetable
	std::vector<float> downsampledWavetable;
	for (int i = 0; i < Wavetable::wavetableSize; i += downsamplingFactor)
		downsampledWavetable.push_back(wavetable->getWavetables().getSample(0, i));

	// Set size to be the downsampled size
	int wavetableSize = (int)downsampledWavetable.size();

	// Create value ranges for the animation
	NormalisableRange<float> yRange(-1.0f * animationHeight / 2.0f, animationHeight / 2.0f);
	NormalisableRange<float> xRange(0.0f, (float)wavetableSize);

	// Create static waveform path
	Path staticPath;
	float x = getWidth() / 2.0f - animationWidth / 2.0f;
	float dx = animationWidth / (float)wavetableSize;
	float yOffset = area.getCentreY();

	for (int i = 0; i < wavetableSize; i++)
	{
		float sample = downsampledWavetable[i];
		float y = yOffset + -1.0f * yRange.convertFrom0to1((sample + 1.0f) / 2.0f);
		Point<float> p(x, y);

		if (i == 0)
			staticPath.startNewSubPath(p);
		else
			staticPath.lineTo(p);

		x += dx;
	}

	// Draw the static waveform path
	g.setColour(getLookAndFeel().findColour(Slider::thumbColourId));
	g.strokePath(staticPath, PathStrokeType(4.0f));

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(area, (float)Constants::PADDING, 2.0f);
}

void Oscilloscope::resized()
{
}

void Oscilloscope::setSound(ReferenceCountedWavetable::Ptr wavetable)
{
	retainedCurrentWavetable = wavetable;
}
