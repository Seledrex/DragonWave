/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DragonWaveAudioProcessorEditor::DragonWaveAudioProcessorEditor(DragonWaveAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	setSize(400, 300);
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
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}