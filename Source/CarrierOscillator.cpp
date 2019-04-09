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

	// Set up slider attachments
	pitchAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::OSCILLATOR_PITCH_ID, pitchSlider);
	voicesAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::OSCILLATOR_VOICES_ID, voicesSlider);

	// Add in buttons
	addAndMakeVisible(openButton);
	openButton.setButtonText(Constants::OPEN_WAVETABLE);
	openButton.onClick = [this] { openButtonClicked(); };

	addAndMakeVisible(sineButton);
	sineButton.setButtonText(Constants::SINE);
	sineButton.addListener(this);

	addAndMakeVisible(triangleButton);
	triangleButton.setButtonText(Constants::TRIANGLE);
	triangleButton.addListener(this);

	addAndMakeVisible(sawtoothButton);
	sawtoothButton.setButtonText(Constants::SAWTOOTH);
	sawtoothButton.addListener(this);

	addAndMakeVisible(squareButton);
	squareButton.setButtonText(Constants::SQUARE);
	squareButton.addListener(this);

	addAndMakeVisible(noiseButton);
	noiseButton.setButtonText(Constants::NOISE);
	noiseButton.addListener(this);

	// Add in sliders
	addAndMakeVisible(pitchSlider);
	pitchSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	pitchSlider.setTextBoxStyle(Slider::TextBoxRight, false, 30, 20);
	pitchSlider.setRange(-24, 24, 1);

	addAndMakeVisible(pitchLabel);
	pitchLabel.setText(Constants::OSCILLATOR_PITCH_NAME, dontSendNotification);
	pitchLabel.setJustificationType(Justification::centredRight);
	pitchLabel.setColour(0, Colours::white);

	addAndMakeVisible(voicesSlider);
	voicesSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	voicesSlider.setTextBoxStyle(Slider::TextBoxRight, false, 30, 20);
	voicesSlider.setRange(1, 32, 1);

	addAndMakeVisible(voicesLabel);
	voicesLabel.setText(Constants::OSCILLATOR_VOICES_NAME, dontSendNotification);
	voicesLabel.setJustificationType(Justification::centredRight);
	voicesLabel.setColour(0, Colours::white);
}

CarrierOscillator::~CarrierOscillator()
{
}

void CarrierOscillator::paint (Graphics& g)
{
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 20);

	// Calculate component area
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
	g.drawRoundedRectangle(componentArea, (float)Constants::PADDING, 2.0f);
}

void CarrierOscillator::resized()
{
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 20);

	// Calculate component area
	float areaX = (float)Constants::PADDING;
	float areaY = (float)titleArea.getPosition().getY() + (float)titleArea.getHeight() + (float)Constants::PADDING;
	float areaWidth = (float)Constants::COMPONENT_WIDTH - 2 * Constants::PADDING;
	float areaHeight = Constants::COMPONENT_HEIGHT - Constants::PADDING - areaY;
	Rectangle<float> componentAreaFloat(areaX, areaY, areaWidth, areaHeight);

	// Convert to integer type and trim off sides
	Rectangle<int> componentArea = componentAreaFloat.toNearestInt().
		withTrimmedLeft(Constants::PADDING).
		withTrimmedRight(Constants::PADDING).
		withTrimmedTop(Constants::PADDING);

	// Create button row 1 area
	auto buttonRow1 = componentArea.removeFromTop(20);

	// Create button row 2 area
	componentArea.removeFromTop(2);
	auto buttonRow2 = componentArea.removeFromTop(20);

	// Set bounds for each button
	int buttonWidth = componentArea.getWidth() / 5;
	sineButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	triangleButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	sawtoothButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	squareButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	noiseButton.setBounds(buttonRow1.removeFromLeft(buttonWidth));
	openButton.setBounds(buttonRow2);

	// Calculate slider area and set bounds
	float percent = 0.40f;
	int labelWidth = roundToInt((float)componentArea.getWidth() * percent / 2.0f);
	int sliderWidth = roundToInt((float)componentArea.getWidth() * (1.0f - percent) / 2.0f);

	auto sliderArea = componentArea.withTrimmedTop(Constants::PADDING).withTrimmedBottom(Constants::PADDING);
	pitchLabel.setBounds(sliderArea.removeFromLeft(labelWidth));
	pitchSlider.setBounds(sliderArea.removeFromLeft(sliderWidth));
	voicesLabel.setBounds(sliderArea.removeFromLeft(labelWidth));
	voicesSlider.setBounds(sliderArea.removeFromLeft(sliderWidth));
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
	FileChooser chooser(Constants::SELECT_WAVETABLE, {}, "*.wav");

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
