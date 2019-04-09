/*
  ==============================================================================

    Filter.cpp
    Created: 8 Apr 2019 2:32:20am
    Author:  Eric

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Filter.h"

//==============================================================================
Filter::Filter(DragonWaveAudioProcessor& p, String title, String cutoffId, String qId, String typeId) :
	processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);
	this->title = title;

	int i = 0;
	addAndMakeVisible(filterMenu);
	filterMenu.addItem(Constants::LOWPASS, ++i);
	filterMenu.addItem(Constants::HIGHPASS, ++i);
	filterMenu.addItem(Constants::BANDPASS, ++i);
	filterMenu.addItem(Constants::BANDREJECT, ++i);
	filterMenu.addItem(Constants::ALLPASS, ++i);
	filterMenu.setJustificationType(Justification::centredLeft);

	addAndMakeVisible(filterMenuLabel);
	filterMenuLabel.setText(Constants::FILTER_TYPE, dontSendNotification);
	filterMenuLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(filterCutoff);
	filterCutoff.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	filterCutoff.setRange(20.0f, 20000.0f);
	filterCutoff.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(filterCutoffLabel);
	filterCutoffLabel.setText(Constants::FILTER_CUTOFF, dontSendNotification);
	filterCutoffLabel.setJustificationType(Justification::centredRight);

	addAndMakeVisible(filterQ);
	filterQ.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	filterQ.setRange(0.5f, 10.0f);
	filterQ.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(filterQLabel);
	filterQLabel.setText(Constants::FILTER_Q, dontSendNotification);
	filterQLabel.setJustificationType(Justification::centredRight);

	filterCutoffAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, cutoffId, filterCutoff);
	filterQAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, qId, filterQ);
	filterMenuAttach = std::make_unique <AudioProcessorValueTreeState::ComboBoxAttachment>(
		processor.parameters, typeId, filterMenu);
}

Filter::~Filter()
{
}

//==============================================================================
void Filter::paint (Graphics& g)
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

void Filter::resized()
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

	filterMenuLabel.setBounds(componentArea.removeFromTop(20));
	filterMenu.setBounds(componentArea.removeFromTop(20));
	componentArea.withTrimmedTop(Constants::PADDING);

	float percent = 0.40f;
	int labelWidth = roundToInt((float)componentArea.getWidth() * percent / 2.0f);
	int rotaryWidth = roundToInt((float)componentArea.getWidth() * (1.0f - percent) / 2.0f);
	
	filterCutoffLabel.setBounds(componentArea.removeFromLeft(labelWidth));
	filterCutoff.setBounds(componentArea.removeFromLeft(rotaryWidth));
	filterQLabel.setBounds(componentArea.removeFromLeft(labelWidth));
	filterQ.setBounds(componentArea.removeFromLeft(rotaryWidth));
}
