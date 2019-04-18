/*
  ==============================================================================

    Envelope.cpp
    Created: 12 Apr 2019 3:36:04pm
    Author:  Eric

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"

//==============================================================================
Envelope::Envelope(DragonWaveAudioProcessor& p, String title,
	String attackId, String decayId, String sustainId, String releaseId,
	String levelId, bool isFilterEnvelope) :
	processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);
	this->title = title;

	int textBoxWidth = 0;
	int textBoxHeight = 0;

	//==============================================================================
	addAndMakeVisible(attackSlider);
	attackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	attackSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);

	addAndMakeVisible(decaySlider);
	decaySlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	decaySlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);

	addAndMakeVisible(sustainSlider);
	sustainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	sustainSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);

	addAndMakeVisible(releaseSlider);
	releaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	releaseSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);

	addAndMakeVisible(levelSlider);
	levelSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	levelSlider.setTextBoxStyle(Slider::NoTextBox, false, textBoxWidth, textBoxHeight);

	if (isFilterEnvelope)
		levelSlider.setLookAndFeel(&midpointSliderLAF);

	//==============================================================================
	addAndMakeVisible(attackLabel);
	attackLabel.setText(Constants::ATTACK, dontSendNotification);
	attackLabel.setJustificationType(Justification::centredBottom);

	addAndMakeVisible(decayLabel);
	decayLabel.setText(Constants::DECAY, dontSendNotification);
	decayLabel.setJustificationType(Justification::centredBottom);

	addAndMakeVisible(sustainLabel);
	sustainLabel.setText(Constants::SUSTAIN, dontSendNotification);
	sustainLabel.setJustificationType(Justification::centredBottom);

	addAndMakeVisible(releaseLabel);
	releaseLabel.setText(Constants::RELEASE, dontSendNotification);
	releaseLabel.setJustificationType(Justification::centredBottom);

	addAndMakeVisible(levelLabel);
	levelLabel.setJustificationType(Justification::centredBottom);

	if (isFilterEnvelope)
		levelLabel.setText(Constants::DEPTH, dontSendNotification);
	else
		levelLabel.setText(Constants::LEVEL, dontSendNotification);
	
	//==============================================================================
	attackAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, attackId, attackSlider);
	decayAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, decayId, decaySlider);
	sustainAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, sustainId, sustainSlider);
	releaseAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, releaseId, releaseSlider);
	levelAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, levelId, levelSlider);
}

Envelope::~Envelope()
{
}

void Envelope::paint (Graphics& g)
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
	g.drawText(title, titleArea, Justification::centred);

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(componentArea, (float)Constants::PADDING, 2.0f);
}

void Envelope::resized()
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
		withTrimmedTop(Constants::PADDING).
		withTrimmedBottom(Constants::PADDING);

	auto labelArea = componentArea.removeFromTop(20);
	int labelWidth = componentArea.getWidth() / 5;
	attackLabel.setBounds(labelArea.removeFromLeft(labelWidth));
	decayLabel.setBounds(labelArea.removeFromLeft(labelWidth));
	sustainLabel.setBounds(labelArea.removeFromLeft(labelWidth));
	releaseLabel.setBounds(labelArea.removeFromLeft(labelWidth));
	levelLabel.setBounds(labelArea.removeFromLeft(labelWidth));

	auto sliderArea = componentArea;
	int sliderWidth = sliderArea.getWidth() / 5;
	attackSlider.setBounds(componentArea.removeFromLeft(sliderWidth));
	decaySlider.setBounds(componentArea.removeFromLeft(sliderWidth));
	sustainSlider.setBounds(componentArea.removeFromLeft(sliderWidth));
	releaseSlider.setBounds(componentArea.removeFromLeft(sliderWidth));
	levelSlider.setBounds(componentArea.removeFromLeft(sliderWidth));
}
