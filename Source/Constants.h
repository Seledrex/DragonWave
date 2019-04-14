/*
  ==============================================================================

    Constants.h
    Created: 7 Apr 2019 9:57:26pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Constants
{
public:
	static const String WAVEFORM_TYPE_ID;
	static const String WAVETABLE_PATH_ID;
	static const String OSCILLATOR_PITCH_ID;
	static const String OSCILLATOR_PITCH_NAME;
	static const String OSCILLATOR_VOICES_ID;
	static const String OSCILLATOR_VOICES_NAME;
	static const String CARRIER_FILTER_TYPE_ID;
	static const String CARRIER_FILTER_TYPE_NAME;
	static const String CARRIER_FILTER_CUTOFF_ID;
	static const String CARRIER_FILTER_CUTOFF_NAME;
	static const String CARRIER_FILTER_Q_ID;
	static const String CARRIER_FILTER_Q_NAME;
	static const String CARRIER_ENV_ATTACK_ID;
	static const String CARRIER_ENV_ATTACK_NAME;
	static const String CARRIER_ENV_DECAY_ID;
	static const String CARRIER_ENV_DECAY_NAME;
	static const String CARRIER_ENV_SUSTAIN_ID;
	static const String CARRIER_ENV_SUSTAIN_NAME;
	static const String CARRIER_ENV_RELEASE_ID;
	static const String CARRIER_ENV_RELEASE_NAME;
	static const String CARRIER_ENV_LEVEL_ID;
	static const String CARRIER_ENV_LEVEL_NAME;

	//==============================================================================
	static const int EDITOR_WIDTH;
	static const int EDITOR_HEIGHT;
	static const int COMPONENT_WIDTH;
	static const int COMPONENT_HEIGHT;
	static const int PADDING;

	//==============================================================================
	static const String CARRIER_OSCILLOSCOPE_TITLE;
	static const String FM_OSCILLOSCOPE_TITLE;
	static const String CARRIER_OSCILLATOR_TITLE;
	static const String FM_OSCILLATOR_TITLE;
	static const String CARRIER_FILTER_TITLE;
	static const String CARRIER_ENV_TITLE;

	//==============================================================================
	static const String SINE;
	static const String TRIANGLE;
	static const String SAWTOOTH;
	static const String SQUARE;
	static const String NOISE;

	//==============================================================================
	static const String OPEN_WAVETABLE;
	static const String SELECT_WAVETABLE;

	//==============================================================================
	static const String LOWPASS;
	static const String HIGHPASS;
	static const String BANDPASS;
	static const String BANDREJECT;
	static const String ALLPASS;

	//==============================================================================
	static const String FILTER_TYPE;
	static const String FILTER_CUTOFF;
	static const String FILTER_Q;

};