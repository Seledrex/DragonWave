/*
  ==============================================================================

    Constants.cpp
    Created: 7 Apr 2019 9:57:26pm
    Author:  Eric

  ==============================================================================
*/

#include "Constants.h"

//==============================================================================
const String Constants::CARRIER_WAVEFORM_TYPE_ID = "carrierWaveformType";
const String Constants::CARRIER_WAVETABLE_PATH_ID = "carrierWavetablePath";
const String Constants::CARRIER_OSC_PITCH_ID = "carrierOscPitch";
const String Constants::CARRIER_OSC_PITCH_NAME = "Pitch";
const String Constants::CARRIER_OSC_VOICES_ID = "carrierOscVoices";
const String Constants::CARRIER_OSC_VOICES_NAME = "Voices";
const String Constants::CARRIER_FILTER_TYPE_ID = "carrierFilterType";
const String Constants::CARRIER_FILTER_TYPE_NAME = "CF Type";
const String Constants::CARRIER_FILTER_CUTOFF_ID = "carrierFilterCutoff";
const String Constants::CARRIER_FILTER_CUTOFF_NAME = "CF Cutoff";
const String Constants::CARRIER_FILTER_Q_ID = "carrierFilterQ";
const String Constants::CARRIER_FILTER_Q_NAME = "CF Q";
const String Constants::CARRIER_ENV_ATTACK_ID = "carrierEnvAttack";
const String Constants::CARRIER_ENV_ATTACK_NAME = "CE Attack";
const String Constants::CARRIER_ENV_DECAY_ID = "carrierEnvDecay";
const String Constants::CARRIER_ENV_DECAY_NAME = "CE Decay";
const String Constants::CARRIER_ENV_SUSTAIN_ID = "carrierEnvSustain";
const String Constants::CARRIER_ENV_SUSTAIN_NAME = "CE Sustain";
const String Constants::CARRIER_ENV_RELEASE_ID = "carrierEnvRelease";
const String Constants::CARRIER_ENV_RELEASE_NAME = "CE Release";
const String Constants::CARRIER_ENV_LEVEL_ID = "carrierEnvLevel";
const String Constants::CARRIER_ENV_LEVEL_NAME = "CE Level";
const String Constants::CARRIER_FILTER_ENV_ATTACK_ID = "carrierFilterEnvAttack";
const String Constants::CARRIER_FILTER_ENV_ATTACK_NAME = "CFE Attack";
const String Constants::CARRIER_FILTER_ENV_DECAY_ID = "carrierFilterEnvDecay";
const String Constants::CARRIER_FILTER_ENV_DECAY_NAME = "CFE Decay";
const String Constants::CARRIER_FILTER_ENV_SUSTAIN_ID = "carrierFilterEnvSustain";
const String Constants::CARRIER_FILTER_ENV_SUSTAIN_NAME = "CFE Sustain";
const String Constants::CARRIER_FILTER_ENV_RELEASE_ID = "carrierFilterEnvRelease";
const String Constants::CARRIER_FILTER_ENV_RELEASE_NAME = "CFE Release";
const String Constants::CARRIER_FILTER_ENV_LEVEL_ID = "carrierFilterEnvLevel";
const String Constants::CARRIER_FILTER_ENV_LEVEL_NAME = "CFE Level";

//==============================================================================
const String Constants::FM_WAVEFORM_TYPE_ID = "fmWaveformType";
const String Constants::FM_WAVEFORM_PATH_ID = "fmWaveformPath";
const String Constants::FM_OSC_FREQUENCY_ID = "fmOscFrequency";
const String Constants::FM_OSC_FREQUENCY_NAME = "FM Freq";
const String Constants::FM_OSC_DEPTH_ID = "fmOscDepth";
const String Constants::FM_OSC_DEPTH_NAME = "FM Depth";
const String Constants::FM_FILTER_TYPE_ID = "fmFilterType";
const String Constants::FM_FILTER_TYPE_NAME = "FMF Type";
const String Constants::FM_FILTER_CUTOFF_ID = "fmFilterCutoff";
const String Constants::FM_FILTER_CUTOFF_NAME = "FMF Cutoff";
const String Constants::FM_FILTER_Q_ID = "fmFilterQ";
const String Constants::FM_FILTER_Q_NAME = "FMF Q";
const String Constants::FM_ENV_ATTACK_ID = "fmEnvAttack";
const String Constants::FM_ENV_ATTACK_NAME = "FME Attack";
const String Constants::FM_ENV_DECAY_ID = "fmEnvDecay";
const String Constants::FM_ENV_DECAY_NAME = "FME Decay";
const String Constants::FM_ENV_SUSTAIN_ID = "fmEnvSustain";
const String Constants::FM_ENV_SUSTAIN_NAME = "FME Sustain";
const String Constants::FM_ENV_RELEASE_ID = "fmEnvRelease";
const String Constants::FM_ENV_RELEASE_NAME = "FME Release";
const String Constants::FM_ENV_LEVEL_ID = "fmEnvLevel";
const String Constants::FM_ENV_LEVEL_NAME = "FME Level";

//==============================================================================
const String Constants::EFFECT_TYPE_ID = "effectType";
const String Constants::EFFECT_TYPE_NAME = "Effect Type";
const String Constants::EQ_LOW_SHELF_ID = "eqLowShelf";
const String Constants::EQ_LOW_SHELF_NAME = "EQ Low Shelf";
const String Constants::EQ_BOOST_ID = "eqBoost";
const String Constants::EQ_BOOST_NAME = "EQ Boost";
const String Constants::EQ_FREQUENCY_ID = "eqFrequency";
const String Constants::EQ_FREQUENCY_NAME = "EQ Freq";
const String Constants::EQ_HIGH_SHELF_ID = "eqHighShelf";
const String Constants::EQ_HIGH_SHELF_NAME = "EQ High Shelf";
const String Constants::WS_SHAPE_ID = "wsShape";
const String Constants::WS_SHAPE_NAME = "WS Shape";
const String Constants::WS_MIX_ID = "wsMix";
const String Constants::WS_MIX_NAME = "WS Dry/Wet";
const String Constants::REVERB_SIZE_ID = "reverbSize";
const String Constants::REVERB_SIZE_NAME = "Reverb Size";
const String Constants::REVERB_DAMP_ID = "reverbDamp";
const String Constants::REVERB_DAMP_NAME = "Reverb Damp";
const String Constants::REVERB_WIDTH_ID = "reverbWidth";
const String Constants::REVERB_WIDTH_NAME = "Reverb Width";
const String Constants::REVERB_MIX_ID = "reverbMix";
const String Constants::REVERB_MIX_NAME = "Reverb Dry/Wet";

//==============================================================================
const String Constants::UNISON_ID = "unison";
const String Constants::UNISON_NAME = "Unison";
const String Constants::DETUNE_ID = "detune";
const String Constants::DETUNE_NAME = "Detune";
const String Constants::SPREAD_ID = "spread";
const String Constants::SPREAD_NAME = "Spread";
const String Constants::MASTER_ID = "master";
const String Constants::MASTER_NAME = "Master";

//==============================================================================
const int Constants::EDITOR_WIDTH = 900;
const int Constants::EDITOR_HEIGHT = 700;
const int Constants::COMPONENT_WIDTH = EDITOR_WIDTH / 3;
const int Constants::COMPONENT_HEIGHT = EDITOR_HEIGHT / 4;
const int Constants::PADDING = 10;

//==============================================================================
const String Constants::CARRIER_OSCILLOSCOPE_TITLE = "Carrier Oscillator";
const String Constants::CARRIER_OSCILLATOR_TITLE = "Carrier Oscillator Settings";
const String Constants::CARRIER_FILTER_TITLE = "Carrier Filter";
const String Constants::CARRIER_ENV_TITLE = "Carrier Envelope";
const String Constants::CARRIER_FILTER_ENV_TITLE = "Carrier Filter Envelope";
const String Constants::FM_OSCILLOSCOPE_TITLE = "FM Oscillator";
const String Constants::FM_OSCILLATOR_TITLE = "FM Oscillator Settings";
const String Constants::FM_FILTER_TITLE = "FM Filter";
const String Constants::FM_ENV_TITLE = "FM Envelope";
const String Constants::MENU_TITLE = "Dragon Wave";
const String Constants::EFFECTS_TITLE = "Effects";
const String Constants::GLOBAL_TITLE = "Global Settings";

//==============================================================================
const String Constants::SINE = "Sin";
const String Constants::TRIANGLE = "Tri";
const String Constants::SAWTOOTH = "Saw";
const String Constants::SQUARE = "Square";
const String Constants::NOISE = "Noise";

//==============================================================================
const String Constants::OPEN_WAVETABLE = "Open wavetable...";
const String Constants::SELECT_WAVETABLE = "Select wavetable...";
const String Constants::OPEN = "Open...";
const String Constants::SAVE = "Save...";
const String Constants::RESET = "Reset";
const String Constants::NEW_PATCH = "New Patch";

//==============================================================================
const String Constants::LOWPASS = "Lowpass";
const String Constants::HIGHPASS = "Highpass";
const String Constants::BANDPASS = "Bandpass";
const String Constants::BANDREJECT = "Bandreject";
const String Constants::ALLPASS = "Allpass";

//==============================================================================
const String Constants::FILTER_TYPE = "Type";
const String Constants::FILTER_CUTOFF = "Cutoff";
const String Constants::FILTER_Q = "Q";

//==============================================================================
const String Constants::ATTACK = "Attack";
const String Constants::DECAY = "Decay";
const String Constants::SUSTAIN = "Sustain";
const String Constants::RELEASE = "Release";
const String Constants::LEVEL = "Level";
const String Constants::DEPTH = "Depth";

//==============================================================================
const String Constants::EQUALIZER = "Equalizer";
const String Constants::WAVESHAPER = "Waveshaper";
const String Constants::REVERB = "Reverb";

//==============================================================================
const String Constants::LOW_SHELF = "Low Shelf";
const String Constants::BOOST = "Boost";
const String Constants::FREQUENCY = "Frequency";
const String Constants::HIGH_SHELF = "High Shelf";
const String Constants::FAINT = "Faint";
const String Constants::LIGHT = "Light";
const String Constants::MODERATE = "Moderate";
const String Constants::HEAVY = "Heavy";
const String Constants::HARSH = "Harsh";
const String Constants::SHAPE = "Shape";
const String Constants::DRYWET = "Dry/Wet";
const String Constants::SIZE = "Size";
const String Constants::DAMP = "Damp";
const String Constants::WIDTH = "Width";
