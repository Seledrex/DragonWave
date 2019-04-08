/*
  ==============================================================================

    CarrierOscillator.cpp
    Created: 8 Apr 2019 2:31:17am
    Author:  Eric

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "CarrierOscillator.h"
#include "LoadingThread.h"

//==============================================================================
CarrierOscillator::CarrierOscillator(DragonWaveAudioProcessor& p) :
	processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);

	addAndMakeVisible(&openButton);
	openButton.setButtonText("Open wavetable...");
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(&sineButton);
	sineButton.setButtonText("Sine");
	sineButton.addListener(this);

	addAndMakeVisible(&triangleButton);
	triangleButton.setButtonText("Tri");
	triangleButton.addListener(this);

	addAndMakeVisible(&sawtoothButton);
	sawtoothButton.setButtonText("Saw");
	sawtoothButton.addListener(this);

	addAndMakeVisible(&squareButton);
	squareButton.setButtonText("Square");
	squareButton.addListener(this);

	addAndMakeVisible(&noiseButton);
	noiseButton.setButtonText("Noise");
	noiseButton.addListener(this);
}

CarrierOscillator::~CarrierOscillator()
{
}

void CarrierOscillator::paint (Graphics& g)
{
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 20);

	float areaX = (float)Constants::PADDING;
	float areaY = (float)titleArea.getPosition().getY() + (float)titleArea.getHeight() + (float)Constants::PADDING;
	float areaWidth = (float)Constants::COMPONENT_WIDTH - 2 * Constants::PADDING;
	float areaHeight = Constants::COMPONENT_HEIGHT - Constants::PADDING - areaY;
	Rectangle<float> componentArea(areaX, areaY, areaWidth, areaHeight);

	// Fill background
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	// Draw title text
	g.setColour(Colours::white);
	g.drawText(Constants::CARRIER_OSCILLATOR_TITLE, titleArea, Justification::centred);

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(componentArea, 10.0f, 2.0f);
}

void CarrierOscillator::resized()
{
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 20);

	float areaX = (float)Constants::PADDING;
	float areaY = (float)titleArea.getPosition().getY() + (float)titleArea.getHeight() + (float)Constants::PADDING;
	float areaWidth = (float)Constants::COMPONENT_WIDTH - 2 * Constants::PADDING;
	float areaHeight = Constants::COMPONENT_HEIGHT - Constants::PADDING - areaY;
	Rectangle<float> componentArea(areaX, areaY, areaWidth, areaHeight);
	Rectangle<int> componentAreaInt = componentArea.toNearestInt();

	int buttonWidth = (componentAreaInt.getWidth() - Constants::PADDING * 2) / 5;

	componentAreaInt.removeFromTop(Constants::PADDING);
	Rectangle<int> buttonRow1 = componentAreaInt.removeFromTop(20);
	buttonRow1.removeFromLeft(Constants::PADDING);
	buttonRow1.removeFromRight(Constants::PADDING);

	componentAreaInt.removeFromTop(2);
	Rectangle<int> buttonRow2 = componentAreaInt.removeFromTop(20);
	buttonRow2.removeFromLeft(Constants::PADDING);
	buttonRow2.removeFromRight(Constants::PADDING);

	sineButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	triangleButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	sawtoothButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	squareButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	noiseButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	openButton.setBounds(buttonRow2);
}

//==============================================================================
void CarrierOscillator::buttonClicked(Button* button)
{
	WavetableSound::Waveform type;

	// Set waveform type
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

	// Update chosen waveform and notify loading thread
	processor.chosenWaveform = type;
	processor.loadingThread->notify();
}

void CarrierOscillator::buttonStateChanged(Button* /*button*/)
{
}

void CarrierOscillator::openButtonClicked()
{
	FileChooser chooser("Select wavetable...", {}, "*.wav");

	// Open file chooser
	if (chooser.browseForFileToOpen())
	{
		// Update chosen path and notify loading thread
		auto file = chooser.getResult();
		auto path = file.getFullPathName();
		processor.chosenPath.swapWith(path);
		processor.loadingThread->notify();
	}
}
