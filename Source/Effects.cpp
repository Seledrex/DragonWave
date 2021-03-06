/*
  ==============================================================================

    Effects.cpp
    Created: 18 Apr 2019 12:56:12am
    Author:  Eric

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Effects.h"

//==============================================================================
Effects::Effects(DragonWaveAudioProcessor& p)
	: processor(p)
{
	setSize(Constants::COMPONENT_WIDTH, Constants::COMPONENT_HEIGHT);

	int i = 0;
	addAndMakeVisible(effectMenu);
	effectMenu.addItem(Constants::EQUALIZER, ++i);
	effectMenu.addItem(Constants::WAVESHAPER, ++i);
	effectMenu.addItem(Constants::REVERB, ++i);
	effectMenu.setJustificationType(Justification::centredLeft);
	effectMenu.addListener(this);

	//==============================================================================
	// Equalizer
	//==============================================================================
	addAndMakeVisible(lowShelfSlider);
	lowShelfSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	lowShelfSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(lowShelfLabel);
	lowShelfLabel.setText(Constants::LOW_SHELF, dontSendNotification);
	lowShelfLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(boostSlider);
	boostSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	boostSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(boostLabel);
	boostLabel.setText(Constants::BOOST, dontSendNotification);
	boostLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(frequencySlider);
	frequencySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	frequencySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(frequencyLabel);
	frequencyLabel.setText(Constants::FREQUENCY, dontSendNotification);
	frequencyLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(highShelfSlider);
	highShelfSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	highShelfSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(highShelfLabel);
	highShelfLabel.setText(Constants::HIGH_SHELF, dontSendNotification);
	highShelfLabel.setJustificationType(Justification::centred);

	//==============================================================================
	// Waveshaper
	//==============================================================================
	i = 0;
	addAndMakeVisible(waveshaperMenu);
	waveshaperMenu.addItem(Constants::FAINT, ++i);
	waveshaperMenu.addItem(Constants::LIGHT, ++i);
	waveshaperMenu.addItem(Constants::MODERATE, ++i);
	waveshaperMenu.addItem(Constants::HEAVY, ++i);
	waveshaperMenu.addItem(Constants::HARSH, ++i);
	waveshaperMenu.setJustificationType(Justification::centredLeft);

	addAndMakeVisible(waveshaperMenuLabel);
	waveshaperMenuLabel.setText(Constants::SHAPE, dontSendNotification);
	waveshaperMenuLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(waveshaperMixSlider);
	waveshaperMixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	waveshaperMixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(waveshaperMixLabel);
	waveshaperMixLabel.setText(Constants::DRYWET, dontSendNotification);
	waveshaperMixLabel.setJustificationType(Justification::centred);

	//==============================================================================
	// Reverb
	//==============================================================================
	addAndMakeVisible(roomSizeSlider);
	roomSizeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	roomSizeSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(roomSizeLabel);
	roomSizeLabel.setText(Constants::SIZE, dontSendNotification);
	roomSizeLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(dampingSlider);
	dampingSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	dampingSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(dampingLabel);
	dampingLabel.setText(Constants::DAMP, dontSendNotification);
	dampingLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(widthSlider);
	widthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	widthSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(widthLabel);
	widthLabel.setText(Constants::WIDTH, dontSendNotification);
	widthLabel.setJustificationType(Justification::centred);

	//==============================================================================
	addAndMakeVisible(reverbMixSlider);
	reverbMixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	reverbMixSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(reverbMixLabel);
	reverbMixLabel.setText(Constants::DRYWET, dontSendNotification);
	reverbMixLabel.setJustificationType(Justification::centred);

	//==============================================================================
	// Attachments
	//==============================================================================
	lowShelfAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::EQ_LOW_SHELF_ID, lowShelfSlider);
	boostAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::EQ_BOOST_ID, boostSlider);
	frequencyAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::EQ_FREQUENCY_ID, frequencySlider);
	highShelfAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::EQ_HIGH_SHELF_ID, highShelfSlider);

	roomSizeAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::REVERB_SIZE_ID, roomSizeSlider);
	dampingAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::REVERB_DAMP_ID, dampingSlider);
	widthAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::REVERB_WIDTH_ID, widthSlider);
	reverbMixAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::REVERB_MIX_ID, reverbMixSlider);

	waveshaperMenuAttach = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
		processor.parameters, Constants::WS_SHAPE_ID, waveshaperMenu);
	waveshaperMixAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		processor.parameters, Constants::WS_MIX_ID, waveshaperMixSlider);

	effectMenuAttach = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(
		processor.parameters, Constants::EFFECT_TYPE_ID, effectMenu);
}

Effects::~Effects()
{
}

void Effects::paint (Graphics& g)
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
	g.drawText(Constants::EFFECTS_TITLE, titleArea, Justification::centred);

	// Draw border
	g.setColour(Colours::white);
	g.drawRoundedRectangle(componentArea, (float)Constants::PADDING, 2.0f);
}

void Effects::resized()
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

	effectMenu.setBounds(componentArea.removeFromTop(20));

	//==============================================================================
	// Equalizer
	//==============================================================================
	auto eqArea = Rectangle<int>(componentArea.withTrimmedTop(Constants::PADDING));
	auto eqLabelArea = eqArea.removeFromTop(20);
	auto eqSliderArea = eqArea;

	int eqLabelWidth = eqLabelArea.getWidth() / 4;
	int eqSliderWidth = eqSliderArea.getWidth() / 4;

	lowShelfLabel.setBounds(eqLabelArea.removeFromLeft(eqLabelWidth));
	boostLabel.setBounds(eqLabelArea.removeFromLeft(eqLabelWidth));
	frequencyLabel.setBounds(eqLabelArea.removeFromLeft(eqLabelWidth));
	highShelfLabel.setBounds(eqLabelArea.removeFromLeft(eqLabelWidth));

	lowShelfSlider.setBounds(eqSliderArea.removeFromLeft(eqSliderWidth));
	boostSlider.setBounds(eqSliderArea.removeFromLeft(eqSliderWidth));
	frequencySlider.setBounds(eqSliderArea.removeFromLeft(eqSliderWidth));
	highShelfSlider.setBounds(eqSliderArea.removeFromLeft(eqSliderWidth));

	//==============================================================================
	// Waveshaper
	//==============================================================================
	auto wsArea = Rectangle<int>(componentArea.withTrimmedTop(Constants::PADDING));
	auto wsLabelArea = wsArea.removeFromTop(20);
	auto wsComponentArea = wsArea;

	int wsLabelWidth = wsLabelArea.getWidth() / 2;
	int wsComponentWidth = wsComponentArea.getWidth() / 2;

	waveshaperMenuLabel.setBounds(wsLabelArea.removeFromLeft(wsLabelWidth));
	waveshaperMixLabel.setBounds(wsLabelArea.removeFromLeft(wsLabelWidth));

	waveshaperMenu.setBounds(wsComponentArea.removeFromLeft(wsComponentWidth).removeFromTop(20));
	waveshaperMixSlider.setBounds(wsComponentArea.removeFromLeft(wsComponentWidth));

	//==============================================================================
	// Reverb
	//==============================================================================
	auto reverbArea = Rectangle<int>(componentArea.withTrimmedTop(Constants::PADDING));
	auto reverbLabelArea = reverbArea.removeFromTop(20);
	auto reverbSliderArea = reverbArea;

	int reverbLabelWidth = reverbLabelArea.getWidth() / 4;
	int reverbSliderWidth = reverbSliderArea.getWidth() / 4;

	roomSizeLabel.setBounds(reverbLabelArea.removeFromLeft(reverbLabelWidth));
	dampingLabel.setBounds(reverbLabelArea.removeFromLeft(reverbLabelWidth));
	widthLabel.setBounds(reverbLabelArea.removeFromLeft(reverbLabelWidth));
	reverbMixLabel.setBounds(reverbLabelArea.removeFromLeft(reverbLabelWidth));

	roomSizeSlider.setBounds(reverbSliderArea.removeFromLeft(reverbSliderWidth));
	dampingSlider.setBounds(reverbSliderArea.removeFromLeft(reverbSliderWidth));
	widthSlider.setBounds(reverbSliderArea.removeFromLeft(reverbSliderWidth));
	reverbMixSlider.setBounds(reverbSliderArea.removeFromLeft(reverbSliderWidth));
}

void Effects::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	int selectedId = comboBoxThatHasChanged->getSelectedId();

	switch (selectedId)
	{
	case 1:
		toggleEqVisible(true);
		toggleWsVisible(false);
		toggleReverbVisible(false);
		break;
	case 2:
		toggleEqVisible(false);
		toggleWsVisible(true);
		toggleReverbVisible(false);
		break;
	case 3:
		toggleEqVisible(false);
		toggleWsVisible(false);
		toggleReverbVisible(true);
		break;
	default:
		return;
	}
}

void Effects::toggleEqVisible(bool visible)
{
	lowShelfLabel.setVisible(visible);
	boostLabel.setVisible(visible);
	frequencyLabel.setVisible(visible);
	highShelfLabel.setVisible(visible);
	lowShelfSlider.setVisible(visible);
	boostSlider.setVisible(visible);
	frequencySlider.setVisible(visible);
	highShelfSlider.setVisible(visible);
}

void Effects::toggleWsVisible(bool visible)
{
	waveshaperMenuLabel.setVisible(visible);
	waveshaperMixLabel.setVisible(visible);
	waveshaperMenu.setVisible(visible);
	waveshaperMixSlider.setVisible(visible);
}

void Effects::toggleReverbVisible(bool visible)
{
	roomSizeLabel.setVisible(visible);
	dampingLabel.setVisible(visible);
	widthLabel.setVisible(visible);
	reverbMixLabel.setVisible(visible);
	roomSizeSlider.setVisible(visible);
	dampingSlider.setVisible(visible);
	widthSlider.setVisible(visible);
	reverbMixSlider.setVisible(visible);
}
