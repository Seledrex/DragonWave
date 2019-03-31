#include "Util.h"

std::vector<float> Util::linspace(float start, float end, int num)
{
	std::vector<float> linspaced;

	if (num == 0)
	{
		return linspaced;
	}
	else if (num == 1)
	{
		linspaced.push_back(start);
		return linspaced;
	}

	double delta = (end - start) / (num - 1);

	for (int i = 0; i < num - 1; ++i)
	{
		linspaced.push_back(start + delta * i);
	}

	linspaced.push_back(end);

	return linspaced;
}

std::vector<float> Util::logspace(float start, float stop, int num, float base = 10) {
	float realStart = pow(base, start);
	float realBase = pow(base, (stop - start) / num);

	std::vector<float> retval;
	retval.reserve(num);
	std::generate_n(std::back_inserter(retval), num, Logspace<float>(realStart, realBase));
	return retval;
}

long long Util::getTimeMs()
{
	milliseconds ms = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
		);
	return ms.count();
}

int Util::toNearestEven(float x)
{
	return roundToInt(x / 2) * 2;
}

void Util::printToFile(std::vector<float> v, std::string filename)
{
	std::ofstream file;
	file.open(filename);

	for (auto it = v.begin(); it != v.end(); it++) {
		file << *it << " ";
	}

	file.close();
}

void Util::tableToFile(std::vector<std::vector<float>> v, std::string filename)
{
	std::ofstream file;
	file.open(filename);

	for (auto it1 = v.begin(); it1 != v.end(); it1++) {
		for (auto it2 = it1->begin(); it2 != it1->end(); it2++) {
			file << *it2 << " ";
		}
		file << "\n";
	}

	file.close();
}

std::vector<float> Util::genWindow(int size, float percent)
{
	int smoothed = toNearestEven(size * (1 - percent));

	std::vector<float> a = Functional::reverse(
		Functional::map(
			logspace(-4, 0, smoothed / 2),
			[](float val) {
				return 1 - val;
			},
			std::vector<float>()
				)
	);

	std::vector<float> b(size - smoothed, 1);

	std::vector<float> c = Functional::map(
		logspace(-4, 0, smoothed / 2),
		[](float val) {
			return 1 - val;
		},
		std::vector<float>()
			);

	std::vector<float> v(size, 0);
	int index = 0;

	for (auto it = a.begin(); it != a.end(); it++) {
		v[index] = *it;
		index++;
	}

	for (auto it = b.begin(); it != b.end(); it++) {
		v[index] = *it;
		index++;
	}

	for (auto it = c.begin(); it != c.end(); it++) {
		v[index] = *it;
		index++;
	}

	return v;
}

std::vector<float> Util::duplicateTable(int times, std::vector<float> table)
{
	std::vector<float> newTable(table.size() * times);
	for (int i = 0; i < times; i++) {
		for (int j = 0; j < table.size(); j++) {
			newTable[j + table.size() * i] = table[j];
		}
	}
	return newTable;
}