/*
  ==============================================================================

    Effects.h
    Created: 18 Apr 2019 12:56:12am
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================

class Effects    : public Component, public ComboBox::Listener
{
public:

	//==============================================================================
    Effects(DragonWaveAudioProcessor&);
    ~Effects();

	//==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	DragonWaveAudioProcessor& processor;

	//==============================================================================
	Slider lowShelfSlider;
	Slider boostSlider;
	Slider frequencySlider;
	Slider highShelfSlider;

	Label lowShelfLabel;
	Label boostLabel;
	Label frequencyLabel;
	Label highShelfLabel;

	//==============================================================================
	Slider roomSizeSlider;
	Slider dampingSlider;
	Slider widthSlider;
	Slider reverbMixSlider;

	Label roomSizeLabel;
	Label dampingLabel;
	Label widthLabel;
	Label reverbMixLabel;

	//==============================================================================
	ComboBox waveshaperMenu;
	Slider waveshaperMixSlider;

	Label waveshaperMenuLabel;
	Label waveshaperMixLabel;

	//==============================================================================
	ComboBox effectMenu;

	//==============================================================================
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> lowShelfAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> boostAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> frequencyAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> highShelfAttach;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> roomSizeAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> dampingAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> widthAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> reverbMixAttach;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> waveshaperMixAttach;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> waveshaperMenuAttach;

	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> effectMenuAttach;

	//==============================================================================
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void toggleEqVisible(bool visible);
	void toggleWsVisible(bool visible);
	void toggleReverbVisible(bool visible);

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Effects)
};
