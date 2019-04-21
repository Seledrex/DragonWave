/*
  ==============================================================================

    Menu.cpp
    Created: 15 Apr 2019 10:40:38pm
    Author:  Eric

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Menu.h"

//==============================================================================
Menu::Menu(DragonWaveAudioProcessor& p) :
	processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);

	addAndMakeVisible(openButton);
	openButton.setButtonText(Constants::OPEN);
	openButton.addListener(this);

	addAndMakeVisible(saveButton);
	saveButton.setButtonText(Constants::SAVE);
	saveButton.addListener(this);

	addAndMakeVisible(resetButton);
	resetButton.setButtonText(Constants::RESET);
	resetButton.addListener(this);

	addAndMakeVisible(presetLabel);
	presetLabel.setText(Constants::RESET, dontSendNotification);
	presetLabel.setJustificationType(Justification::centred);
	presetLabel.setColour(0, Colours::white);
}

Menu::~Menu()
{
}

void Menu::paint (Graphics& g)
{
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 50);

	// Calculate component area
	float areaX = (float)Constants::PADDING;
	float areaY = (float)titleArea.getPosition().getY() + (float)titleArea.getHeight() + (float)Constants::PADDING;
	float areaWidth = (float)Constants::COMPONENT_WIDTH - 2 * Constants::PADDING;
	float areaHeight = Constants::COMPONENT_HEIGHT - Constants::PADDING - areaY;
	Rectangle<float> componentArea(areaX, areaY, areaWidth, areaHeight);

	// Fill background
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

	// Draw title text
	g.setFont(g.getCurrentFont().italicised());
	g.setFont(30);
	g.setColour(Colours::white);
	g.drawFittedText(Constants::MENU_TITLE, titleArea, Justification::centred, 1);

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(componentArea, (float)Constants::PADDING, 2.0f);
}

void Menu::resized()
{
	Rectangle<int> titleArea(0, Constants::PADDING, getWidth(), 50);

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

	int split = componentArea.getHeight() / 2;
	auto buttonArea = componentArea.removeFromTop(split);
	auto labelArea = componentArea;

	int buttonWidth = buttonArea.getWidth() / 3;
	openButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
	saveButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
	resetButton.setBounds(buttonArea.removeFromLeft(buttonWidth));

	presetLabel.setBounds(labelArea);
}

void Menu::buttonClicked(Button* button)
{
	ignoreUnused(button);
}

void Menu::buttonStateChanged(Button* button)
{
	ignoreUnused(button);
}
