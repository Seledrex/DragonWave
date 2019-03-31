/*
  ==============================================================================

	Wavetable.cpp
	Created: 25 Mar 2019 6:30:06pm
	Author:  Eric

  ==============================================================================
*/

#include "Wavetable.h"

Wavetable::Wavetable(std::vector<float> inTable, int samplingRate)
{
	// Calculate constants
	wavetableSize = (int)inTable.size();
	int fftOrder = findFFTOrder(samplingRate);
	int fftSize = (int)std::pow(2, fftOrder);
	int nyquist = samplingRate / 2;
	float baseFrequency = (float)fftSize / (float)wavetableSize;

	// Duplicate table
	inTable = Util::duplicateTable(3, inTable);

	// Create FFT object
	dsp::FFT fftObject(fftOrder);

	// Convert the input wavetable into the frequency domain
	std::vector<float> inTableFrequencyDomain(fftObject.getSize() * 2, 0);
	inTableFrequencyDomain.insert(inTableFrequencyDomain.begin(), inTable.begin(), inTable.end());
	fftObject.performRealOnlyForwardTransform(&inTableFrequencyDomain[0]);

	// Calculate partials needed for each table frequency
	auto partials = Functional::map(
		tableFrequencies,
		[nyquist](float x) -> float {
			return (float)nyquist / x - 1;
		},
		std::vector<float>()
			);

	// Calculate cutoff frequencies for each table frequency
	auto cutoffFrequencies = Functional::map(
		partials,
		[baseFrequency](float x) -> int {
			return roundToInt(x * baseFrequency);
		},
		std::vector<int>()
			);

	// For each cutoff frequency create its corresponding frequency domain
	auto frequencyDomains = Functional::map(
		cutoffFrequencies,
		[inTableFrequencyDomain](int frequency) -> std::vector<float> {
			std::vector<float> outTableFrequencyDomain(inTableFrequencyDomain.size(), 0.0);
			for (int i = 0; i < frequency; i++) {
				outTableFrequencyDomain[i * 2] = inTableFrequencyDomain[i * 2];
				outTableFrequencyDomain[i * 2 + 1] = inTableFrequencyDomain[i * 2 + 1];
			}
			return outTableFrequencyDomain;
		},
		std::vector<std::vector<float>>()
			);

	// Revert back to the time domain for each frequency domain
	auto outTables = Functional::map(
		frequencyDomains,
		[this, &fftObject](std::vector<float> frequencyDomain) -> std::vector<float> {
			fftObject.performRealOnlyInverseTransform(&frequencyDomain[0]);
			std::vector<float> outTable(wavetableSize);
			for (int i = 0; i < wavetableSize; i++)
				outTable[i] = frequencyDomain[i + wavetableSize];
			return outTable;
		},
		std::vector<std::vector<float>>()
			);

	// Set buffer size
	tables.setSize((int)tableFrequencies.size() + 2, wavetableSize + 1);

	// Copy inTable to the first channel of the buffer
	float lowestFrequency = 20.0f;
	auto * samples = tables.getWritePointer(0);
	for (int i = 0; i < wavetableSize; i++)
		samples[i] = inTable[i];
	samples[wavetableSize] = samples[0];

	// Each subsequent channel is another wavetable
	for (int i = 0; i < outTables.size(); i++) {
		samples = tables.getWritePointer(i + 1);
		for (int j = 0; j < wavetableSize; j++)
			samples[j] = outTables[i][j];
		samples[wavetableSize] = samples[0];
	}

	// Make the last channel a sine wave
	float highestFrequency = 8000.0f;
	samples = tables.getWritePointer(tables.getNumChannels() - 1);
	float angleDelta = MathConstants<float>::twoPi / wavetableSize;
	float currentAngle = 0.0;
	for (int i = 0; i < wavetableSize; i++) {
		samples[i] = std::sin(currentAngle);
		currentAngle += angleDelta;
	}
	samples[wavetableSize] = samples[0];

	// Put all the indexes into the table map with their corresponding frequency
	for (int i = 0; i < tableFrequencies.size(); i++)
		tableMap.insert(std::pair<float, int>(tableFrequencies[i], i + 1));

	// Set the input wavetable to be the lowest wavetable
	tableMap.insert(std::pair<float, int>(lowestFrequency, 0));
	tableFrequencies.insert(tableFrequencies.begin(), lowestFrequency);

	// Set highest wavetable to be a sine wave
	tableMap.insert(std::pair<float, int>(highestFrequency, tables.getNumChannels() - 1));
	tableFrequencies.push_back(highestFrequency);
}

std::pair<float, float> Wavetable::getBoundingFrequencies(float targetFrequency)
{
	return findNearest(targetFrequency);
}

std::pair<int, int> Wavetable::getBoundingIndexes(std::pair<float, float> targetFrequencies)
{
	auto lo = tableMap.find(targetFrequencies.first);
	auto hi = tableMap.find(targetFrequencies.second);
	return std::pair<int, int>(lo->second, hi->second);
}

AudioSampleBuffer* Wavetable::getTables()
{
	return &tables;
}

int Wavetable::getSize()
{
	return wavetableSize;
}

int Wavetable::findFFTOrder(int samplingRate)
{
	int order = 1;

	// Find the power of two which is more than the sampling rate
	while (std::pow(2, order) < samplingRate)
		order += 1;

	return order;
}

std::pair<float, float> Wavetable::findNearest(float target)
{
	int n = (int)tableFrequencies.size();

	if (target <= tableFrequencies[0])
		return std::pair<float, float>(
			tableFrequencies[0],
			tableFrequencies[0]
			);
	if (target >= tableFrequencies[n - 1])
		return std::pair<float, float>(
			tableFrequencies[n - 1],
			tableFrequencies[n - 1]
			);

	int low = 0, high = n, mid = 0;

	while (low < high) {
		mid = (low + high) / 2;

		if (tableFrequencies[mid] == target)
			return std::pair<float, float>(
				tableFrequencies[mid],
				tableFrequencies[mid]
				);

		if (target < tableFrequencies[mid]) {
			if (mid > 0 && target > tableFrequencies[mid - 1])
				return std::pair<float, float>(
					tableFrequencies[mid - 1],
					tableFrequencies[mid]
					);
			high = mid;
		}
		else {
			if (mid < n - 1 && target < tableFrequencies[mid + 1])
				return std::pair<float, float>(
					tableFrequencies[mid],
					tableFrequencies[mid + 1]
					);
			low = mid + 1;
		}
	}

	return std::pair<float, float>(
		tableFrequencies[mid],
		tableFrequencies[mid]
		);
}