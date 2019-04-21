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

	//==============================================================================
	static const String CARRIER_WAVEFORM_TYPE_ID;
	static const String CARRIER_WAVETABLE_PATH_ID;
	static const String CARRIER_OSC_PITCH_ID;
	static const String CARRIER_OSC_PITCH_NAME;
	static const String CARRIER_OSC_VOICES_ID;
	static const String CARRIER_OSC_VOICES_NAME;
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
	static const String CARRIER_FILTER_ENV_ATTACK_ID;
	static const String CARRIER_FILTER_ENV_ATTACK_NAME;
	static const String CARRIER_FILTER_ENV_DECAY_ID;
	static const String CARRIER_FILTER_ENV_DECAY_NAME;
	static const String CARRIER_FILTER_ENV_SUSTAIN_ID;
	static const String CARRIER_FILTER_ENV_SUSTAIN_NAME;
	static const String CARRIER_FILTER_ENV_RELEASE_ID;
	static const String CARRIER_FILTER_ENV_RELEASE_NAME;
	static const String CARRIER_FILTER_ENV_LEVEL_ID;
	static const String CARRIER_FILTER_ENV_LEVEL_NAME;
	
	//==============================================================================
	static const String FM_WAVEFORM_TYPE_ID;
	static const String FM_WAVEFORM_PATH_ID;
	static const String FM_OSC_FREQUENCY_ID;
	static const String FM_OSC_FREQUENCY_NAME;
	static const String FM_OSC_DEPTH_ID;
	static const String FM_OSC_DEPTH_NAME;
	static const String FM_FILTER_TYPE_ID;
	static const String FM_FILTER_TYPE_NAME;
	static const String FM_FILTER_CUTOFF_ID;
	static const String FM_FILTER_CUTOFF_NAME;
	static const String FM_FILTER_Q_ID;
	static const String FM_FILTER_Q_NAME;
	static const String FM_ENV_ATTACK_ID;
	static const String FM_ENV_ATTACK_NAME;
	static const String FM_ENV_DECAY_ID;
	static const String FM_ENV_DECAY_NAME;
	static const String FM_ENV_SUSTAIN_ID;
	static const String FM_ENV_SUSTAIN_NAME;
	static const String FM_ENV_RELEASE_ID;
	static const String FM_ENV_RELEASE_NAME;
	static const String FM_ENV_LEVEL_ID;
	static const String FM_ENV_LEVEL_NAME;

	//==============================================================================
	static const String EFFECT_TYPE_ID;
	static const String EFFECT_TYPE_NAME;
	static const String EQ_LOW_SHELF_ID;
	static const String EQ_LOW_SHELF_NAME;
	static const String EQ_BOOST_ID; 
	static const String EQ_BOOST_NAME;
	static const String EQ_FREQUENCY_ID;
	static const String EQ_FREQUENCY_NAME;
	static const String EQ_HIGH_SHELF_ID;
	static const String EQ_HIGH_SHELF_NAME;
	static const String WS_SHAPE_ID;
	static const String WS_SHAPE_NAME;
	static const String WS_MIX_ID;
	static const String WS_MIX_NAME;
	static const String REVERB_SIZE_ID;
	static const String REVERB_SIZE_NAME;
	static const String REVERB_DAMP_ID;
	static const String REVERB_DAMP_NAME;
	static const String REVERB_WIDTH_ID;
	static const String REVERB_WIDTH_NAME;
	static const String REVERB_MIX_ID;
	static const String REVERB_MIX_NAME;

	//==============================================================================
	static const String UNISON_ID;
	static const String UNISON_NAME;
	static const String DETUNE_ID;
	static const String DETUNE_NAME;
	static const String SPREAD_ID;
	static const String SPREAD_NAME;
	static const String MASTER_ID;
	static const String MASTER_NAME;

	//==============================================================================
	static const int EDITOR_WIDTH;
	static const int EDITOR_HEIGHT;
	static const int COMPONENT_WIDTH;
	static const int COMPONENT_HEIGHT;
	static const int PADDING;

	//==============================================================================
	static const String CARRIER_OSCILLOSCOPE_TITLE;
	static const String CARRIER_OSCILLATOR_TITLE;
	static const String CARRIER_FILTER_TITLE;
	static const String CARRIER_ENV_TITLE;
	static const String CARRIER_FILTER_ENV_TITLE;
	static const String FM_OSCILLOSCOPE_TITLE;
	static const String FM_OSCILLATOR_TITLE;
	static const String FM_FILTER_TITLE;
	static const String FM_ENV_TITLE;
	static const String MENU_TITLE;
	static const String EFFECTS_TITLE;
	static const String GLOBAL_TITLE;

	//==============================================================================
	static const String SINE;
	static const String TRIANGLE;
	static const String SAWTOOTH;
	static const String SQUARE;
	static const String NOISE;

	//==============================================================================
	static const String OPEN_WAVETABLE;
	static const String SELECT_WAVETABLE;
	static const String OPEN;
	static const String SAVE;
	static const String RESET;
	static const String NEW_PATCH;

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

	//==============================================================================
	static const String ATTACK;
	static const String DECAY;
	static const String SUSTAIN;
	static const String RELEASE;
	static const String LEVEL;
	static const String DEPTH;

	//==============================================================================
	static const String EQUALIZER;
	static const String WAVESHAPER;
	static const String REVERB;

	//==============================================================================
	static const String LOW_SHELF;
	static const String BOOST;
	static const String FREQUENCY;
	static const String HIGH_SHELF;
	static const String FAINT;
	static const String LIGHT;
	static const String MODERATE;
	static const String HEAVY;
	static const String HARSH;
	static const String SHAPE;
	static const String DRYWET;
	static const String SIZE;
	static const String DAMP;
	static const String WIDTH;

};