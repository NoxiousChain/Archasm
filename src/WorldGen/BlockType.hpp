#pragma once
#include <vector>

class BlockType {
public:
	struct Range {
		double min;
		double max;

		Range(double min, double max) : min{ min }, max{ max } {}
	};
	struct Conditions {
		Range elevation, wetness, temperature;
	};
private:
	size_t index;
	Conditions conditions;

public:
	BlockType(size_t index = UINT64_MAX, double elevationMin = 0, double elevationMax = 0, double wetnessMin = 0, double wetnessMax = 0, double tempMin = 0, double tempMax = 0);
	BlockType(size_t index = UINT64_MAX, std::vector<double> data = {0,0,0,0,0,0});

	Range getElevationRange() const;
	Range getWetnessRange() const;
	Range getTempRange() const;

	Conditions getConditions const;

	size_t getIndex() const;
};