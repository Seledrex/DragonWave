/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DragonWaveAudioProcessorEditor::DragonWaveAudioProcessorEditor(DragonWaveAudioProcessor& p)
	: AudioProcessorEditor(&p),
	processor(p),
	yRange(-1.0f * animationHeight / 2.0f, animationHeight / 2.0f),
	xRange(0.0f, (float)tableSize)
{
	setSize(300, 250);
	startTimerHz(60);

	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open wavetable...");

	formatManager.registerBasicFormats();
}

DragonWaveAudioProcessorEditor::~DragonWaveAudioProcessorEditor()
{
}

//==============================================================================
void DragonWaveAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	Path staticPath;
	float x = getWidth() / 2.0f - animationWidth / 2.0f;
	float dx = animationWidth / (float)tableSize;

	for (int i = 0; i < tableSize; i++)
	{
		float sample = processor.sound->getWavetables().getSample(0, i);
		float y = getHeight() / 2.0f + -1.0f * yRange.convertFrom0to1((sample + 1.0f) / 2.0f);
		Point<float> p(x, y);

		if (i == 0)
			staticPath.startNewSubPath(p);
		else
			staticPath.lineTo(p);

		x += dx;
	}

	g.setColour(getLookAndFeel().findColour(Slider::thumbColourId));
	g.strokePath(staticPath, PathStrokeType(4.0f));

	if (processor.noteOnCount > 0)
	{
		Path animatedPath;
		int animatedPathSize = tableSize / 4;
		int animatedPathDelta = getFrameCounter() * 50 % tableSize + tableSize / 2;

		std::vector<int> animatedIndices;
		for (int i = -1 * animatedPathSize / 2; i < animatedPathSize / 2; i++)
		{
			int currentIndex = animatedPathDelta + i;
			animatedIndices.push_back(currentIndex % tableSize);
		}

		for (int i = 0; i < animatedIndices.size(); i++)
		{
			int index = animatedIndices[i];
			float sample = processor.sound->getWavetables().getSample(0, index);
			float y = getHeight() / 2.0f + -1.0f * yRange.convertFrom0to1((sample + 1.0f) / 2.0f);
			x = getWidth() / 2.0f - animationWidth / 2.0f + dx * index;
			Point<float> p(x, y);

			if (i == 0)
				animatedPath.startNewSubPath(p);
			else
				animatedPath.lineTo(p);
		}

		g.setColour(Colours::antiquewhite);
		g.strokePath(animatedPath, PathStrokeType(4.0f));
	}
}

void DragonWaveAudioProcessorEditor::resized()
{
	openButton.setBounds(10, 10, getWidth() - 20, 20);
}

void DragonWaveAudioProcessorEditor::timerCallback()
{
	repaint();
	frameCounter++;
}

int DragonWaveAudioProcessorEditor::getFrameCounter()
{
	return frameCounter;
}
