/*
  ==============================================================================

    Global.cpp
    Created: 20 Apr 2019 10:04:12pm
    Author:  Eric

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Global.h"

//==============================================================================
Global::Global(DragonWaveAudioProcessor& p)
	: processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);

	//==============================================================================
	unisonAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::UNISON_ID, unisonSlider);
	detuneAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::DETUNE_ID, detuneSlider);
	spreadAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::SPREAD_ID, spreadSlider);
	masterAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::MASTER_ID, masterSlider);

	//==============================================================================
	addAndMakeVisible(unisonSlider);
	unisonSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	unisonSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 30, 20);
	unisonSlider.setRange(1, 8, 1);

	addAndMakeVisible(unisonLabel);
	unisonLabel.setText(Constants::UNISON_NAME, dontSendNotification);
	unisonLabel.setJustificationType(Justification::centredBottom);

	//==============================================================================
	addAndMakeVisible(detuneSlider);
	detuneSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	detuneSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(detuneLabel);
	detuneLabel.setText(Constants::DETUNE_NAME, dontSendNotification);
	detuneLabel.setJustificationType(Justification::centredBottom);

	//==============================================================================
	addAndMakeVisible(spreadSlider);
	spreadSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	spreadSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(spreadLabel);
	spreadLabel.setText(Constants::SPREAD_NAME, dontSendNotification);
	spreadLabel.setJustificationType(Justification::centredBottom);

	//==============================================================================
	addAndMakeVisible(masterSlider);
	masterSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	masterSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(masterLabel);
	masterLabel.setText(Constants::MASTER_NAME, dontSendNotification);
	masterLabel.setJustificationType(Justification::centredBottom);
}

Global::~Global()
{
}

void Global::paint (Graphics& g)
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
	g.drawText(Constants::GLOBAL_TITLE, titleArea, Justification::centred);

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(componentArea, (float)Constants::PADDING, 2.0f);
}

void Global::resized()
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

	auto labelArea = componentArea.removeFromTop(30);
	auto sliderArea = componentArea.withTrimmedBottom(20);
	int width = labelArea.getWidth() / 4;

	unisonLabel.setBounds(labelArea.removeFromLeft(width));
	detuneLabel.setBounds(labelArea.removeFromLeft(width));
	spreadLabel.setBounds(labelArea.removeFromLeft(width));
	masterLabel.setBounds(labelArea.removeFromLeft(width));

	unisonSlider.setBounds(sliderArea.removeFromLeft(width));
	detuneSlider.setBounds(sliderArea.removeFromLeft(width));
	spreadSlider.setBounds(sliderArea.removeFromLeft(width));
	masterSlider.setBounds(sliderArea.removeFromLeft(width));
}
