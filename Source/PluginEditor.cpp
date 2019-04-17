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
DragonWaveAudioProcessorEditor::DragonWaveAudioProcessorEditor(DragonWaveAudioProcessor& p) :
	AudioProcessorEditor(&p), processor(p),
	carrierOscilloscopeGUI(p,
		Constants::CARRIER_OSCILLOSCOPE_TITLE),
	fmOscilloscopeGUI(p,
		Constants::FM_OSCILLOSCOPE_TITLE),
	carrierOscGUI(p),
	fmOscGUI(p),
	carrierFilterGUI(p,
		Constants::CARRIER_FILTER_TITLE,
		Constants::CARRIER_FILTER_CUTOFF_ID,
		Constants::CARRIER_FILTER_Q_ID,
		Constants::CARRIER_FILTER_TYPE_ID),
	fmFilterGUI(p,
		Constants::FM_FILTER_TITLE,
		Constants::FM_FILTER_CUTOFF_ID,
		Constants::FM_FILTER_Q_ID,
		Constants::FM_FILTER_TYPE_ID
	),
	carrierEnvGUI(p,
		Constants::CARRIER_ENV_TITLE,
		Constants::CARRIER_ENV_ATTACK_ID,
		Constants::CARRIER_ENV_DECAY_ID,
		Constants::CARRIER_ENV_SUSTAIN_ID,
		Constants::CARRIER_ENV_RELEASE_ID,
		Constants::CARRIER_ENV_LEVEL_ID),
	fmEnvGUI(p,
		Constants::FM_ENV_TITLE,
		Constants::FM_ENV_ATTACK_ID,
		Constants::FM_ENV_DECAY_ID,
		Constants::FM_ENV_SUSTAIN_ID,
		Constants::FM_ENV_RELEASE_ID,
		Constants::FM_ENV_LEVEL_ID),
	menuGUI(p),
	carrierFilterEnvGUI(p,
		Constants::CARRIER_FILTER_ENV_TITLE,
		Constants::CARRIER_FILTER_ENV_ATTACK_ID,
		Constants::CARRIER_FILTER_ENV_DECAY_ID,
		Constants::CARRIER_FILTER_ENV_SUSTAIN_ID,
		Constants::CARRIER_FILTER_ENV_RELEASE_ID,
		Constants::CARRIER_FILTER_ENV_LEVEL_ID)
{
	setSize(Constants::EDITOR_WIDTH, Constants::EDITOR_HEIGHT);
	startTimerHz(5);

	addAndMakeVisible(carrierOscilloscopeGUI);
	addAndMakeVisible(carrierOscGUI);
	addAndMakeVisible(carrierFilterGUI);
	addAndMakeVisible(carrierEnvGUI);

	addAndMakeVisible(fmOscilloscopeGUI);
	addAndMakeVisible(fmOscGUI);
	addAndMakeVisible(fmFilterGUI);
	addAndMakeVisible(fmEnvGUI);

	addAndMakeVisible(menuGUI);
	addAndMakeVisible(carrierFilterEnvGUI);
}

DragonWaveAudioProcessorEditor::~DragonWaveAudioProcessorEditor()
{
}

//==============================================================================
void DragonWaveAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void DragonWaveAudioProcessorEditor::resized()
{
	auto column1 = getLocalBounds().withTrimmedRight(Constants::COMPONENT_WIDTH * 2);
	auto column2 = getLocalBounds().withTrimmedLeft(Constants::COMPONENT_WIDTH).withTrimmedRight(Constants::COMPONENT_WIDTH);
	auto column3 = getLocalBounds().withTrimmedLeft(Constants::COMPONENT_WIDTH * 2);

	// Set up column one components
	carrierOscilloscopeGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierOscGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierFilterGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierEnvGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));

	// Set up column two components
	fmOscilloscopeGUI.setBounds(column2.removeFromTop(Constants::COMPONENT_HEIGHT));
	fmOscGUI.setBounds(column2.removeFromTop(Constants::COMPONENT_HEIGHT));
	fmFilterGUI.setBounds(column2.removeFromTop(Constants::COMPONENT_HEIGHT));
	fmEnvGUI.setBounds(column2.removeFromTop(Constants::COMPONENT_HEIGHT));

	// Set up column three components
	menuGUI.setBounds(column3.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierFilterEnvGUI.setBounds(column3.removeFromTop(Constants::COMPONENT_HEIGHT));
}

//==============================================================================
void DragonWaveAudioProcessorEditor::timerCallback()
{
	carrierOscilloscopeGUI.setSound(ReferenceCountedWavetable::Ptr(processor.currentCarrierWavetable));
	fmOscilloscopeGUI.setSound(ReferenceCountedWavetable::Ptr(processor.currentFMWavetable));

	carrierOscilloscopeGUI.repaint();
	fmOscilloscopeGUI.repaint();
}

