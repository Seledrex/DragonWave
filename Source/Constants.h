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
	static const String FILTER_TYPE_ID;
	static const String FILTER_TYPE_NAME;
	static const String FILTER_CUTOFF_ID;
	static const String FILTER_CUTOFF_NAME;
	static const String FILTER_Q_ID;
	static const String FILTER_Q_NAME;

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
};