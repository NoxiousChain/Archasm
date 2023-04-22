#pragma once
#include <vector>
#include <Array.hpp>

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
	String name;
	size_t index;
	Conditions conditions;

public:
	BlockType(const String& name, size_t index = UINT64_MAX, double elevationMin = 0, double elevationMax = 0, double wetnessMin = 0, double wetnessMax = 0, double tempMin = 0, double tempMax = 0);
	BlockType(const String& name, size_t index = UINT64_MAX, const std::vector<double>& data = {0,0,0,0,0,0});
	BlockType(const String& name, size_t index = UINT64_MAX, const Array& data);

	Range getElevationRange() const;
	Range getWetnessRange() const;
	Range getTempRange() const;

	Conditions getConditions const;

	size_t getIndex() const;
};