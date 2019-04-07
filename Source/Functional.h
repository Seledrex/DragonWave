/*
  ==============================================================================

	Functional.h
	Created: 24 Mar 2019 11:57:29pm
	Author:  Eric

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

// Useful functional style C++ methods
class Functional
{
public:
	template<typename In, typename Out, typename Lamda>
	static std::vector<Out> map(std::vector<In> v, Lamda op, std::vector<Out> u) {
		u.reserve(v.size());
		std::transform(v.begin(), v.end(), std::back_inserter(u), op);
		return u;
	}

	template<typename Collection>
	static Collection reverse(Collection v) {
		std::reverse(v.begin(), v.end());
		return v;
	}
private:
	Functional() {};
};
