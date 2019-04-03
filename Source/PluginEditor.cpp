/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LoadingThread.h"

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
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(&sineButton);
	sineButton.setButtonText("Sine");
	sineButton.addListener(this);

	addAndMakeVisible(&triangleButton);
	triangleButton.setButtonText("Triangle");
	triangleButton.addListener(this);

	addAndMakeVisible(&sawtoothButton);
	sawtoothButton.setButtonText("Sawtooth");
	sawtoothButton.addListener(this);

	addAndMakeVisible(&squareButton);
	squareButton.setButtonText("Square");
	squareButton.addListener(this);

	addAndMakeVisible(&noiseButton);
	noiseButton.setButtonText("Noise");
	noiseButton.addListener(this);
}

DragonWaveAudioProcessorEditor::~DragonWaveAudioProcessorEditor()
{
}

//==============================================================================
void DragonWaveAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	ReferenceCountedSound::Ptr retainedCurrentSound(processor.currentSound);

	if (retainedCurrentSound == nullptr)
		return;

	auto* sound = retainedCurrentSound->getSound();
	
	Path staticPath;
	float x = getWidth() / 2.0f - animationWidth / 2.0f;
	float dx = animationWidth / (float)tableSize;
	float yOffset = (getHeight() / 2.0f) + 20.0f;

	for (int i = 0; i < tableSize; i++)
	{
		float sample = sound->getWavetables().getSample(0, i);
		float y = yOffset + -1.0f * yRange.convertFrom0to1((sample + 1.0f) / 2.0f);
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
			float sample = sound->getWavetables().getSample(0, index);
			float y = yOffset + -1.0f * yRange.convertFrom0to1((sample + 1.0f) / 2.0f);
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
	int buttonWidth = (getWidth() - 20) / 5;

	openButton.setBounds(10, 10, getWidth() - 20, 20);
	sineButton.setBounds(10, 32, buttonWidth, 20);
	triangleButton.setBounds(10 + buttonWidth, 32, buttonWidth, 20);
	sawtoothButton.setBounds(10 + buttonWidth * 2, 32, buttonWidth, 20);
	squareButton.setBounds(10 + buttonWidth * 3, 32, buttonWidth, 20);
	noiseButton.setBounds(10 + buttonWidth * 4, 32, buttonWidth, 20);
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

void DragonWaveAudioProcessorEditor::openButtonClicked()
{
	FileChooser chooser("Select wavetable...", {}, "*.wav");

	if (chooser.browseForFileToOpen())
	{
		auto file = chooser.getResult();
		auto path = file.getFullPathName();
		processor.chosenPath.swapWith(path);
		processor.loadingThread->notify();
	}
}

void DragonWaveAudioProcessorEditor::buttonClicked(Button* button)
{
	WavetableSound::Waveform type;

	if (button == &sineButton)
		type = WavetableSound::Waveform::Sine;
	else if (button == &triangleButton)
		type = WavetableSound::Waveform::Triangle;
	else if (button == &sawtoothButton)
		type = WavetableSound::Waveform::Sawtooth;
	else if (button == &squareButton)
		type = WavetableSound::Waveform::Square;
	else
		type = WavetableSound::Waveform::Noise;

	processor.chosenWaveform = type;
	processor.loadingThread->notify();
}

void DragonWaveAudioProcessorEditor::buttonStateChanged(Button* /*button*/)
{
}
