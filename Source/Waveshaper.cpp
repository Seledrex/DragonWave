/*
  ==============================================================================

    Waveshaper.cpp
    Created: 19 Apr 2019 8:26:53pm
    Author:  Eric

  ==============================================================================
*/

#include "Waveshaper.h"

Waveshaper::Waveshaper() : currentTable(faintTable)
{
	faintTable = sampleTransferFunction([](float x) -> float {
		return 1.31303528551f * std::tanh(x);
	});

	lightTable = sampleTransferFunction([](float x) -> float {
		return 1.09033141074f * std::tanh(MathConstants<float>::pi / 2.0f * x);
	});

	moderateTable = sampleTransferFunction([](float x) -> float {
		return 1.01812944412f * std::tanh(3.0f * MathConstants<float>::pi / 4.0f * x);
	});

	heavyTable = sampleTransferFunction([](float x) -> float {
		return 1.0037418732f * std::tanh(MathConstants<float>::pi * x);
	});

	harshTable = sampleTransferFunction([](float x) -> float {
		return 1.00016141207f * std::tanh(3.0f * MathConstants<float>::pi / 2.0f * x);
	});
}

void Waveshaper::setParameters(TransferFunction f, float dry, float wet)
{
	this->f = f;
	this->dry = dry;
	this->wet = wet;

	switch (this->f) {
	case FAINT:
		currentTable = faintTable;
		break;
	case LIGHT:
		currentTable = lightTable;
		break;
	case MODERATE:
		currentTable = moderateTable;
		break;
	case HEAVY:
		currentTable = heavyTable;
		break;
	case HARSH:
		currentTable = harshTable;
		break;
	}
}

void Waveshaper::processMono(float* buffer, int numSamples)
{
	for (int i = 0; i < numSamples; i++)
	{
		int mappedValue = roundToInt(jmap(buffer[i], -1.0f, 1.0f, 0.0f, (float)size - 1.0f));
		buffer[i] = currentTable[mappedValue] * wet + buffer[i] * dry;
	}
}

void Waveshaper::processStereo(float* leftChannel, float* rightChannel, int numSamples)
{
	for (int i = 0; i < numSamples; i++)
	{
		int mappedValueLeft = roundToInt(jmap(leftChannel[i], -1.0f, 1.0f, 0.0f, (float)size - 1.0f));
		int mappedValueRight = roundToInt(jmap(rightChannel[i], -1.0f, 1.0f, 0.0f, (float)size - 1.0f));
		leftChannel[i] = currentTable[mappedValueLeft] * wet + leftChannel[i] * dry;
		rightChannel[i] = currentTable[mappedValueRight] * wet + rightChannel[i] * dry;
	}
}

float Waveshaper::processSingleSample(float sample)
{
	int mappedValue = roundToInt(jmap(sample, -1.0f, 1.0f, 0.0f, (float)size - 1.0f));
	return currentTable[mappedValue] * wet + sample * dry;
}

std::vector<float> Waveshaper::sampleTransferFunction(std::function<float(float)> f)
{
	float dx = 2.0f / size;
	float x = -1.0f;

	std::vector<float> table(size, 0);
	for (int i = 0; i < size; i++)
	{
		float sample = f(x);

		if (sample < -1.0f)
			sample = -1.0f;
		else if (sample > 1.0f)
			sample = 1.0f;

		table[i] = sample;
		x += dx;
	}

	return table;
}
