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

	// Keep a copy of the current sound
	int retainedNoteOnCount = processor.noteOnCount;

	// Do not draw waveform if there is none to draw
	if (retainedCurrentWavetable == nullptr) {
		return;
	}

	// Get sound and ensure it's initialized
	auto* sound = retainedCurrentWavetable->getWavetable();

	// Downsample wavetable
	std::vector<float> downsampledWavetable;
	for (int i = 0; i < Wavetable::wavetableSize; i += downsamplingFactor)
		downsampledWavetable.push_back(sound->getWavetables().getSample(0, i));

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

	// Check if the note on count is more than 0
	if (retainedNoteOnCount > 0)
	{
		// Create animated waveform path
		Path animatedPath;
		int animatedPathSize = wavetableSize / 4;
		int animatedPathDelta = frameCount * 50 % wavetableSize + wavetableSize / 2;

		std::vector<int> animatedIndices;
		for (int i = -1 * animatedPathSize / 2; i < animatedPathSize / 2; i++)
		{
			int currentIndex = animatedPathDelta + i;
			animatedIndices.push_back(currentIndex % wavetableSize);
		}

		for (int i = 0; i < animatedIndices.size(); i++)
		{
			int index = animatedIndices[i];
			float sample = downsampledWavetable[index];
			float y = yOffset + -1.0f * yRange.convertFrom0to1((sample + 1.0f) / 2.0f);
			x = getWidth() / 2.0f - animationWidth / 2.0f + dx * index;
			Point<float> p(x, y);

			if (i == 0)
				animatedPath.startNewSubPath(p);
			else
				animatedPath.lineTo(p);
		}

		// Draw animated waveform path
		g.setColour(Colours::antiquewhite);
		g.strokePath(animatedPath, PathStrokeType(4.0f));
	}

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(area, (float)Constants::PADDING, 2.0f);
}

void Oscilloscope::resized()
{
}

void Oscilloscope::incrementFrameCount()
{
	frameCount++;
}

void Oscilloscope::setSound(ReferenceCountedWavetable::Ptr wavetable)
{
	retainedCurrentWavetable = wavetable;
}
