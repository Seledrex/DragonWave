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
	carrierOscilloscopeGUI(p, Constants::CARRIER_OSCILLOSCOPE_TITLE),
	carrierOscGUI(p)
{
	setSize(Constants::EDITOR_WIDTH, Constants::EDITOR_HEIGHT);
	startTimerHz(60);

	addAndMakeVisible(carrierOscilloscopeGUI);
	addAndMakeVisible(carrierOscGUI);
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
}

//==============================================================================
void DragonWaveAudioProcessorEditor::timerCallback()
{
	frameCounter++;
	carrierOscilloscopeGUI.setSound(ReferenceCountedSound::Ptr(processor.currentSound));
	carrierOscilloscopeGUI.setFrameCount(frameCounter);
	carrierOscilloscopeGUI.repaint();
}

