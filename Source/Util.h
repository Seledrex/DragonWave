#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Logspace.h"
#include "Functional.h"

#include <iostream>
#include <fstream>
#include <chrono>

using namespace std::chrono;

class Util
{
public:
	static std::vector<float> linspace(float start, float end, int num);
	static std::vector<float> logspace(float start, float stop, int num, float base);
	static long long getTimeMs();
	static int toNearestEven(float x);
	static void printToFile(std::vector<float> v, std::string filename);
	static void tableToFile(std::vector<std::vector<float>> table, std::string filename);
	static std::vector<float> genWindow(int size, float percent);
	static std::vector<float> duplicateTable(int times, std::vector<float> table);

	template<typename T>
	static void printVector(std::vector<T> v) {
		for (auto it = v.begin(); it != v.end(); it++) {
			if (it == v.begin())
				std::cout << "[" << *it;
			else
				std::cout << ", " << *it;
		}
		std::cout << "]" << std::endl;
	}

private:
	Util() {}
};
