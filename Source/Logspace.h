#pragma once

template<typename T>
class Logspace {
private:
	T curValue, base;

public:
	Logspace(T first, T base) : curValue(first), base(base) {};

	T operator()() {
		T retval = curValue;
		curValue *= base;
		return retval;
	};
};
