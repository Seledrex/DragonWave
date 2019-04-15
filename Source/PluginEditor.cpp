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
	carrierOscGUI(p),
	carrierFilterGUI(p,
		Constants::CARRIER_FILTER_TITLE,
		Constants::CARRIER_FILTER_CUTOFF_ID,
		Constants::CARRIER_FILTER_Q_ID,
		Constants::CARRIER_FILTER_TYPE_ID),
	carrierEnvGUI(p,
		Constants::CARRIER_ENV_TITLE,
		Constants::CARRIER_ENV_ATTACK_ID,
		Constants::CARRIER_ENV_DECAY_ID,
		Constants::CARRIER_ENV_SUSTAIN_ID,
		Constants::CARRIER_ENV_RELEASE_ID,
		Constants::CARRIER_ENV_LEVEL_ID)
{
	setSize(Constants::EDITOR_WIDTH, Constants::EDITOR_HEIGHT);
	startTimerHz(60);

	addAndMakeVisible(carrierOscilloscopeGUI);
	addAndMakeVisible(carrierOscGUI);
	addAndMakeVisible(carrierFilterGUI);
	addAndMakeVisible(carrierEnvGUI);
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
	Rectangle<int> column1 = getLocalBounds().withTrimmedRight(Constants::COMPONENT_WIDTH * 2);

	carrierOscilloscopeGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierOscGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierFilterGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
	carrierEnvGUI.setBounds(column1.removeFromTop(Constants::COMPONENT_HEIGHT));
}

//==============================================================================
void DragonWaveAudioProcessorEditor::timerCallback()
{
	carrierOscilloscopeGUI.setSound(ReferenceCountedSound::Ptr(processor.currentSound));
	carrierOscilloscopeGUI.incrementFrameCount();
	carrierOscilloscopeGUI.repaint();
}

