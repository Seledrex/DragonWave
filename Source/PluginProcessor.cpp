/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WavetableVoice.h"
#include "LoadingThread.h"

//==============================================================================
DragonWaveAudioProcessor::DragonWaveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	parameters(*this, nullptr, "PARAMETERS", createParameterLayout()) // Initialize parameters
#endif
{
	// Add voices to the synth
	int numVoices = (int)* parameters.getRawParameterValue(Constants::CARRIER_OSC_VOICES_ID);
	for (auto i = 0; i < numVoices; i++) {
		synth.addVoice(new WavetableVoice());
	}

	// Start the loading thread
	loadingThread = std::unique_ptr<LoadingThread>(new LoadingThread(*this));
}

DragonWaveAudioProcessor::~DragonWaveAudioProcessor()
{
}

//==============================================================================
AudioProcessorValueTreeState::ParameterLayout DragonWaveAudioProcessor::createParameterLayout()
{
	// Define parameters here
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

	//==============================================================================
	// Carrier Oscillator Params
	//==============================================================================
	auto carrierOscPitch = std::make_unique<AudioParameterInt>(
		Constants::CARRIER_OSC_PITCH_ID,
		Constants::CARRIER_OSC_PITCH_NAME,
		-24, 24, 0
		);
	params.push_back(std::move(carrierOscPitch));

	auto carrierOscVoices = std::make_unique<AudioParameterInt>(
		Constants::CARRIER_OSC_VOICES_ID,
		Constants::CARRIER_OSC_VOICES_NAME,
		1, 32, 8
		);
	params.push_back(std::move(carrierOscVoices));

	//==============================================================================
	// Carrier Filter Params
	//==============================================================================
	auto carrierFilterType = std::make_unique<AudioParameterChoice>(
		Constants::CARRIER_FILTER_TYPE_ID,
		Constants::CARRIER_FILTER_TYPE_NAME,
		StringArray(
			Constants::LOWPASS,
			Constants::HIGHPASS,
			Constants::BANDPASS,
			Constants::BANDREJECT,
			Constants::ALLPASS
		),
		0
		);
	params.push_back(std::move(carrierFilterType));

	auto carrierFilterCutoff = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_CUTOFF_ID,
		Constants::CARRIER_FILTER_CUTOFF_NAME,
		NormalisableRange<float>(20.0f, 20000.0f, 0.01f, 0.5f),
		20000.0f
		);
	params.push_back(std::move(carrierFilterCutoff));

	auto carrierFilterQ = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_Q_ID,
		Constants::CARRIER_FILTER_Q_NAME,
		NormalisableRange<float>(0.5f, 10.0f),
		1.0f
		);
	params.push_back(std::move(carrierFilterQ));

	//==============================================================================
	// Carrier Envelope Params
	//==============================================================================
	auto carrierEnvelopeAttack = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_ENV_ATTACK_ID,
		Constants::CARRIER_ENV_ATTACK_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(carrierEnvelopeAttack));

	auto carrierEnvelopeDecay = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_ENV_DECAY_ID,
		Constants::CARRIER_ENV_DECAY_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(carrierEnvelopeDecay));

	auto carrierEnvelopeSustain = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_ENV_SUSTAIN_ID,
		Constants::CARRIER_ENV_SUSTAIN_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.001f, 0.5f),
		1.0f
		);
	params.push_back(std::move(carrierEnvelopeSustain));

	auto carrierEnvelopeRelease = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_ENV_RELEASE_ID,
		Constants::CARRIER_ENV_RELEASE_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(carrierEnvelopeRelease));

	auto carrierEnvelopeLevel = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_ENV_LEVEL_ID,
		Constants::CARRIER_ENV_LEVEL_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.001f, 0.5f),
		1.0f
		);
	params.push_back(std::move(carrierEnvelopeLevel));

	//==============================================================================
	// FM Oscillator Params
	//==============================================================================
	auto fmOscFrequency = std::make_unique<AudioParameterFloat>(
		Constants::FM_OSC_FREQUENCY_ID,
		Constants::FM_OSC_FREQUENCY_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 0.5f),
		0.5f
		);
	params.push_back(std::move(fmOscFrequency));

	auto fmOscDepth = std::make_unique<AudioParameterFloat>(
		Constants::FM_OSC_DEPTH_ID,
		Constants::FM_OSC_DEPTH_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.0001f, 0.5f),
		0.0f
		);
	params.push_back(std::move(fmOscDepth));

	//==============================================================================
	// FM Filter Params
	//==============================================================================
	auto fmFilterType = std::make_unique<AudioParameterChoice>(
		Constants::FM_FILTER_TYPE_ID,
		Constants::FM_FILTER_TYPE_NAME,
		StringArray(
			Constants::LOWPASS,
			Constants::HIGHPASS,
			Constants::BANDPASS,
			Constants::BANDREJECT,
			Constants::ALLPASS
		),
		0
		);
	params.push_back(std::move(fmFilterType));

	auto fmFilterCutoff = std::make_unique<AudioParameterFloat>(
		Constants::FM_FILTER_CUTOFF_ID,
		Constants::FM_FILTER_CUTOFF_NAME,
		NormalisableRange<float>(20.0f, 20000.0f, 0.01f, 0.5f),
		20000.0f
		);
	params.push_back(std::move(fmFilterCutoff));

	auto fmFilterQ = std::make_unique<AudioParameterFloat>(
		Constants::FM_FILTER_Q_ID,
		Constants::FM_FILTER_Q_NAME,
		NormalisableRange<float>(0.5f, 10.0f),
		1.0f
		);
	params.push_back(std::move(fmFilterQ));

	//==============================================================================
	// FM Envelope Params
	//==============================================================================
	auto fmEnvelopeAttack = std::make_unique<AudioParameterFloat>(
		Constants::FM_ENV_ATTACK_ID,
		Constants::FM_ENV_ATTACK_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(fmEnvelopeAttack));

	auto fmEnvelopeDecay = std::make_unique<AudioParameterFloat>(
		Constants::FM_ENV_DECAY_ID,
		Constants::FM_ENV_DECAY_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(fmEnvelopeDecay));

	auto fmEnvelopeSustain = std::make_unique<AudioParameterFloat>(
		Constants::FM_ENV_SUSTAIN_ID,
		Constants::FM_ENV_SUSTAIN_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.001f, 0.5f),
		1.0f
		);
	params.push_back(std::move(fmEnvelopeSustain));

	auto fmEnvelopeRelease = std::make_unique<AudioParameterFloat>(
		Constants::FM_ENV_RELEASE_ID,
		Constants::FM_ENV_RELEASE_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(fmEnvelopeRelease));

	auto fmEnvelopeLevel = std::make_unique<AudioParameterFloat>(
		Constants::FM_ENV_LEVEL_ID,
		Constants::FM_ENV_LEVEL_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.001f, 0.5f),
		1.0f
		);
	params.push_back(std::move(fmEnvelopeLevel));

	//==============================================================================
	// Carrier Filter Envelope Params
	//==============================================================================
	auto carrierFilterEnvelopeAttack = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_ENV_ATTACK_ID,
		Constants::CARRIER_FILTER_ENV_ATTACK_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(carrierFilterEnvelopeAttack));

	auto carrierFilterEnvelopeDecay = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_ENV_DECAY_ID,
		Constants::CARRIER_FILTER_ENV_DECAY_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(carrierFilterEnvelopeDecay));

	auto carrierFilterEnvelopeSustain = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_ENV_SUSTAIN_ID,
		Constants::CARRIER_FILTER_ENV_SUSTAIN_NAME,
		NormalisableRange<float>(0.0f, 1.0f, 0.001f, 0.5f),
		1.0f
		);
	params.push_back(std::move(carrierFilterEnvelopeSustain));

	auto carrierFilterEnvelopeRelease = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_ENV_RELEASE_ID,
		Constants::CARRIER_FILTER_ENV_RELEASE_NAME,
		NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.5f),
		0.001f
		);
	params.push_back(std::move(carrierFilterEnvelopeRelease));

	auto carrierFilterEnvelopeLevel = std::make_unique<AudioParameterFloat>(
		Constants::CARRIER_FILTER_ENV_LEVEL_ID,
		Constants::CARRIER_FILTER_ENV_LEVEL_NAME,
		NormalisableRange<float>(-1.0f, 1.0f, 0.001f, 0.4f, true),
		0.0f
		);
	params.push_back(std::move(carrierFilterEnvelopeLevel));

	//==============================================================================
	// Effect Params
	//==============================================================================
	auto effectType = std::make_unique<AudioParameterChoice>(
		Constants::EFFECT_TYPE_ID,
		Constants::EFFECT_TYPE_NAME,
		StringArray(
			Constants::EQUALIZER,
			Constants::WAVESHAPER,
			Constants::REVERB
		),
		0
		);
	params.push_back(std::move(effectType));

	//==============================================================================
	// Equalizer Params
	//==============================================================================
	auto eqLowShelf = std::make_unique<AudioParameterFloat>(
		Constants::EQ_LOW_SHELF_ID,
		Constants::EQ_LOW_SHELF_NAME,
		NormalisableRange<float>(0.1f, 1.9f),
		1.0f
		);
	params.push_back(std::move(eqLowShelf));

	auto eqBoost = std::make_unique<AudioParameterFloat>(
		Constants::EQ_BOOST_ID,
		Constants::EQ_BOOST_NAME,
		NormalisableRange<float>(0.1f, 1.9f),
		1.0f
		);
	params.push_back(std::move(eqBoost));

	auto eqFrequency = std::make_unique<AudioParameterFloat>(
		Constants::EQ_FREQUENCY_ID,
		Constants::EQ_FREQUENCY_NAME,
		NormalisableRange<float>(250.0f, 2000.0f, 0.001f, 0.6f),
		800.0f
		);
	params.push_back(std::move(eqFrequency));

	auto eqHighShelf = std::make_unique<AudioParameterFloat>(
		Constants::EQ_HIGH_SHELF_ID,
		Constants::EQ_HIGH_SHELF_NAME,
		NormalisableRange<float>(0.1f, 1.9f),
		1.0f
		);
	params.push_back(std::move(eqHighShelf));

	//==============================================================================
	// Reverb Params
	//==============================================================================
	auto reverbSize = std::make_unique<AudioParameterFloat>(
		Constants::REVERB_SIZE_ID,
		Constants::REVERB_SIZE_NAME,
		NormalisableRange<float>(0.0f, 1.0f),
		0.0f
		);
	params.push_back(std::move(reverbSize));

	auto reverbDamp = std::make_unique<AudioParameterFloat>(
		Constants::REVERB_DAMP_ID,
		Constants::REVERB_DAMP_NAME,
		NormalisableRange<float>(0.0f, 1.0f),
		0.0f
		);
	params.push_back(std::move(reverbDamp));

	auto reverbWidth = std::make_unique<AudioParameterFloat>(
		Constants::REVERB_WIDTH_ID,
		Constants::REVERB_WIDTH_NAME,
		NormalisableRange<float>(0.0f, 1.0f),
		0.0f
		);
	params.push_back(std::move(reverbWidth));

	auto reverbMix = std::make_unique<AudioParameterFloat>(
		Constants::REVERB_MIX_ID,
		Constants::REVERB_MIX_NAME,
		NormalisableRange<float>(0.0f, 1.0f),
		0.0f
		);
	params.push_back(std::move(reverbMix));

	//==============================================================================
	// Waveshaper Params
	//==============================================================================
	auto wsShape = std::make_unique<AudioParameterChoice>(
		Constants::WS_SHAPE_ID,
		Constants::WS_SHAPE_NAME,
		StringArray(
			Constants::FAINT,
			Constants::LIGHT,
			Constants::MODERATE,
			Constants::HEAVY,
			Constants::HARSH
		),
		0
		);
	params.push_back(std::move(wsShape));

	auto wsMix = std::make_unique<AudioParameterFloat>(
		Constants::WS_MIX_ID,
		Constants::WS_MIX_NAME,
		NormalisableRange<float>(0.0f, 1.0f),
		0.0f
		);
	params.push_back(std::move(wsMix));

	//==============================================================================
	// Global Params
	//==============================================================================
	auto unison = std::make_unique<AudioParameterInt>(
		Constants::UNISON_ID,
		Constants::UNISON_NAME,
		1, 4, 1
		);
	params.push_back(std::move(unison));

	auto detune = std::make_unique<AudioParameterFloat>(
		Constants::DETUNE_ID,
		Constants::DETUNE_NAME,
		NormalisableRange<float>(0.0f, 0.04166666666f),
		0.0f
		);
	params.push_back(std::move(detune));

	auto spread = std::make_unique<AudioParameterFloat>(
		Constants::SPREAD_ID,
		Constants::SPREAD_NAME,
		NormalisableRange<float>(0.0f, 1.0f),
		0.0f
		);
	params.push_back(std::move(spread));

	auto master = std::make_unique<AudioParameterFloat>(
		Constants::MASTER_ID,
		Constants::MASTER_NAME,
		NormalisableRange<float>(0.0f, 2.0f, 0.001f, 0.5f),
		1.0f
		);
	params.push_back(std::move(master));

	return { params.begin(), params.end() };
}

//==============================================================================
const String DragonWaveAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool DragonWaveAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool DragonWaveAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool DragonWaveAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double DragonWaveAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int DragonWaveAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int DragonWaveAudioProcessor::getCurrentProgram()
{
	return 0;
}

void DragonWaveAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const String DragonWaveAudioProcessor::getProgramName(int /*index*/)
{
	return {};
}

void DragonWaveAudioProcessor::changeProgramName(int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void DragonWaveAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);
	reverb.setSampleRate(getSampleRate());
	reverb.reset();
}

void DragonWaveAudioProcessor::releaseResources()
{
	synth.allNotesOff(0, false);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DragonWaveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void DragonWaveAudioProcessor::processBlock(AudioBuffer<float> & buffer, MidiBuffer & midiMessages)
{
	// Add or remove voices from the synth
	int numVoices = (int)* parameters.getRawParameterValue(Constants::CARRIER_OSC_VOICES_ID);
	if (numVoices < synth.getNumVoices())
	{
		for (int i = synth.getNumVoices() - 1; i >= numVoices; i--)
			synth.removeVoice(i);
	}
	else if (numVoices > synth.getNumVoices())
	{
		int numVoicesToAdd = numVoices - synth.getNumVoices();
		for (int i = 0; i < numVoicesToAdd; i++)
			synth.addVoice(new WavetableVoice());
	}

	// Update synth voices with parameters
	WavetableVoice* voice;
	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		voice = dynamic_cast<WavetableVoice*>(synth.getVoice(i));

		if (voice != nullptr)
		{
			voice->setGlobalParams(
				parameters.getRawParameterValue(Constants::UNISON_ID),
				parameters.getRawParameterValue(Constants::DETUNE_ID),
				parameters.getRawParameterValue(Constants::SPREAD_ID)
			);

			voice->setCarrierPitchShift(
				parameters.getRawParameterValue(Constants::CARRIER_OSC_PITCH_ID)
			);

			voice->setCarrierFilterParams(
				*parameters.getRawParameterValue(Constants::CARRIER_FILTER_TYPE_ID),
				*parameters.getRawParameterValue(Constants::CARRIER_FILTER_CUTOFF_ID),
				*parameters.getRawParameterValue(Constants::CARRIER_FILTER_Q_ID)
			);

			voice->setCarrierEnvParams(
				parameters.getRawParameterValue(Constants::CARRIER_ENV_ATTACK_ID),
				parameters.getRawParameterValue(Constants::CARRIER_ENV_DECAY_ID),
				parameters.getRawParameterValue(Constants::CARRIER_ENV_SUSTAIN_ID),
				parameters.getRawParameterValue(Constants::CARRIER_ENV_RELEASE_ID),
				parameters.getRawParameterValue(Constants::CARRIER_ENV_LEVEL_ID)
			);

			voice->setCarrierFilterEnvParams(
				parameters.getRawParameterValue(Constants::CARRIER_FILTER_ENV_ATTACK_ID),
				parameters.getRawParameterValue(Constants::CARRIER_FILTER_ENV_DECAY_ID),
				parameters.getRawParameterValue(Constants::CARRIER_FILTER_ENV_SUSTAIN_ID),
				parameters.getRawParameterValue(Constants::CARRIER_FILTER_ENV_RELEASE_ID),
				parameters.getRawParameterValue(Constants::CARRIER_FILTER_ENV_LEVEL_ID)
			);

			voice->setFmOscParams(
				parameters.getRawParameterValue(Constants::FM_OSC_FREQUENCY_ID),
				parameters.getRawParameterValue(Constants::FM_OSC_DEPTH_ID)
			);

			voice->setFmFilterParams(
				parameters.getRawParameterValue(Constants::FM_FILTER_TYPE_ID),
				parameters.getRawParameterValue(Constants::FM_FILTER_CUTOFF_ID),
				parameters.getRawParameterValue(Constants::FM_FILTER_Q_ID)
			);

			voice->setFmEnvParams(
				parameters.getRawParameterValue(Constants::FM_ENV_ATTACK_ID),
				parameters.getRawParameterValue(Constants::FM_ENV_DECAY_ID),
				parameters.getRawParameterValue(Constants::FM_ENV_SUSTAIN_ID),
				parameters.getRawParameterValue(Constants::FM_ENV_RELEASE_ID),
				parameters.getRawParameterValue(Constants::FM_ENV_LEVEL_ID)
			);
		}
	}

	// Render synth audio
	buffer.clear();
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	// Get waveshaper params
	auto wsShape = static_cast<Waveshaper::TransferFunction>((int)* parameters.getRawParameterValue(Constants::WS_SHAPE_ID));
	float wsMix = (float)* parameters.getRawParameterValue(Constants::WS_MIX_ID);
	waveshaper.setParameters(wsShape, 1.0f - wsMix, wsMix);

	// Apply waveshaping
	waveshaper.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());

	// Get reverb params
	float reverbSize = *parameters.getRawParameterValue(Constants::REVERB_SIZE_ID);
	float reverbDamp = *parameters.getRawParameterValue(Constants::REVERB_DAMP_ID);
	float reverbWidth = *parameters.getRawParameterValue(Constants::REVERB_WIDTH_ID);
	float reverbMix = *parameters.getRawParameterValue(Constants::REVERB_MIX_ID);

	// Apply reverb
	reverb.setParameters({ reverbSize, reverbDamp, reverbMix, (1.0f - reverbMix), reverbWidth, 0.0f });
	reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());

	// Get eq params
	float eqLowGain = *parameters.getRawParameterValue(Constants::EQ_LOW_SHELF_ID);
	float eqMidGain = *parameters.getRawParameterValue(Constants::EQ_BOOST_ID);
	float eqMidFreq = *parameters.getRawParameterValue(Constants::EQ_FREQUENCY_ID);
	float eqHighGain = *parameters.getRawParameterValue(Constants::EQ_HIGH_SHELF_ID);

	// Set eq coefficients
	lowFilterL.setCoefficients(IIRCoefficients::makeLowShelf(getSampleRate(), 250.0, 0.5, eqLowGain));
	lowFilterR.setCoefficients(IIRCoefficients::makeLowShelf(getSampleRate(), 250.0, 0.5, eqLowGain));;
	midFilterL.setCoefficients(IIRCoefficients::makePeakFilter(getSampleRate(), eqMidFreq, 0.5, eqMidGain));
	midFilterR.setCoefficients(IIRCoefficients::makePeakFilter(getSampleRate(), eqMidFreq, 0.5, eqMidGain));
	highFilterL.setCoefficients(IIRCoefficients::makeHighShelf(getSampleRate(), 2000.0, 0.5, eqHighGain));
	highFilterR.setCoefficients(IIRCoefficients::makeHighShelf(getSampleRate(), 2000.0, 0.5, eqHighGain));

	// Eq that shizz
	lowFilterL.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
	lowFilterR.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());
	midFilterL.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
	midFilterR.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());
	highFilterL.processSamples(buffer.getWritePointer(0), buffer.getNumSamples());
	highFilterR.processSamples(buffer.getWritePointer(1), buffer.getNumSamples());

	// Get master gain
	currentGain = *parameters.getRawParameterValue(Constants::MASTER_ID);

	// Apply gain
	if (currentGain == previousGain)
		buffer.applyGain(currentGain);
	else
	{
		buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
		previousGain = currentGain;
	}
}

//==============================================================================
bool DragonWaveAudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* DragonWaveAudioProcessor::createEditor()
{
	return new DragonWaveAudioProcessorEditor(*this);
}

//==============================================================================
void DragonWaveAudioProcessor::getStateInformation(MemoryBlock & destData)
{
	// Copy state and convert to XML
	auto state = parameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());

	// Include waveform choices
	int enumVal = chosenCarrierWaveform;
	xml->setAttribute(Constants::CARRIER_WAVEFORM_TYPE_ID, enumVal);
	enumVal = chosenFMWaveform;
	xml->setAttribute(Constants::FM_WAVEFORM_TYPE_ID, enumVal);

	// Include carrier wavetable file path
	if (currentCarrierWavetable != nullptr)
		xml->setAttribute(Constants::CARRIER_WAVETABLE_PATH_ID, currentCarrierWavetable->getPath());
	else
		xml->setAttribute(Constants::CARRIER_WAVETABLE_PATH_ID, "");

	// Include FM wavetable file path
	if (currentFMWavetable != nullptr)
		xml->setAttribute(Constants::FM_WAVEFORM_PATH_ID, currentFMWavetable->getPath());
	else
		xml->setAttribute(Constants::FM_WAVEFORM_PATH_ID, "");

	// Store parameters in the memory block
	copyXmlToBinary(*xml, destData);
}

// Restores parameters from this memory block,
// whose contents will have been created by the getStateInformation() call.
void DragonWaveAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// Read memory block into XML
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(parameters.state.getType()))
		{
			// Replace parameter state with that of the XML
			parameters.replaceState(ValueTree::fromXml(*xmlState));

			// Get the chosen carrier waveform and file path for the wavetable
			int chosenWavefromIndex = std::stoi(xmlState->getStringAttribute(Constants::CARRIER_WAVEFORM_TYPE_ID).toStdString());
			chosenCarrierWaveform = static_cast<Wavetable::Waveform>(chosenWavefromIndex);
			chosenCarrierPath = xmlState->getStringAttribute(Constants::CARRIER_WAVETABLE_PATH_ID);

			// Get the chosen FM waveform and file path for the wavetable
			chosenWavefromIndex = std::stoi(xmlState->getStringAttribute(Constants::FM_WAVEFORM_TYPE_ID).toStdString());
			chosenFMWaveform = static_cast<Wavetable::Waveform>(chosenWavefromIndex);
			chosenFMPath = xmlState->getStringAttribute(Constants::FM_WAVEFORM_PATH_ID);

			// Notify the thread to load
			loadingThread->notify();
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new DragonWaveAudioProcessor();
}