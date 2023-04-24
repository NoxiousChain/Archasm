#pragma once
#include <vector>
#include <Array.hpp>
#include <String.hpp>
#include "../Common.hpp"

class BlockType {
public:
	struct Range {
		double min;
		double max;

		Range(double min, double max) : min{ min }, max{ max } {}
	};
	struct Conditions {
		Range elevation, humidity, temperature;
	};
private:
	String name;
	size_t index;
	Conditions conditions;

public:
	BlockType(const String& name, size_t index = UINT64_MAX, double elevationMin = 0, double elevationMax = 0, double humidityMin = 0, double humidityMax = 0, double tempMin = 0, double tempMax = 0);
	BlockType(const String& name, size_t index = UINT64_MAX, const std::vector<double>& data = {0,0,0,0,0,0});
	BlockType(const String& name, size_t index = UINT64_MAX, const Array& data);

	Range getElevationRange() const;
	Range getHumidityRange() const;
	Range getTempRange() const;

	Conditions getConditions const;

	size_t getIndex() const;
};