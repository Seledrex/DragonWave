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
		return 1.0037418732f* std::tanh(MathConstants<float>::pi * x);
	});

	heavyTable = sampleTransferFunction([](float x) -> float {
		return 1.00016141207f* std::tanh(3.0f * MathConstants<float>::pi / 2.0f * x);
	});

	harshTable = sampleTransferFunction([](float x) -> float {
		return 1.00000697471f * std::tanh(MathConstants<float>::twoPi * x);
	});
}

void Waveshaper::setParameters(TransferFunction function, float dryAmount, float wetAmount)
{
	f = function;
	dry = dryAmount;
	wet = wetAmount;

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
		buffer[i] = processSingleSample(buffer[i]);
	}
}

void Waveshaper::processStereo(float* leftChannel, float* rightChannel, int numSamples)
{
	for (int i = 0; i < numSamples; i++)
	{
		leftChannel[i] = processSingleSample(leftChannel[i]);
		rightChannel[i] = processSingleSample(rightChannel[i]);
	}
}

float Waveshaper::processSingleSample(float sample)
{
	float mappedValue = jmap(sample, -1.0f, 1.0f, 0.0f, (float)size - 1.0f);

	auto indexBefore = (unsigned int)mappedValue;
	auto indexAfter = indexBefore + 1;
	auto frac = mappedValue - (float)indexBefore;

	auto valueBefore = currentTable[indexBefore];
	auto valueAfter = currentTable[indexAfter];
	float currentSample = valueBefore + frac * (valueAfter - valueBefore);

	return currentSample * wet + sample * dry;
}

std::vector<float> Waveshaper::sampleTransferFunction(std::function<float(float)> tf)
{
	float dx = 2.0f / size;
	float x = -1.0f;

	std::vector<float> table(size, 0);
	for (int i = 0; i < size; i++)
	{
		float sample = tf(x);

		if (sample < -1.0f)
			sample = -1.0f;
		else if (sample > 1.0f)
			sample = 1.0f;

		table[i] = sample;
		x += dx;
	}

	return table;
}
