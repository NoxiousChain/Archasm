#pragma once
#include <vector>
#include <Dictionary.hpp>
#include <String.hpp>
#include "../../../Common.hpp"

class BlockType {
public:
	struct Range {
		double min;
		double max;

		Range() {}
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
	BlockType();
	BlockType(const String& name, const Dictionary& data, size_t index = SIZE_MAX);

	Range getElevationRange() const;
	Range getHumidityRange() const;
	Range getTempRange() const;

	Conditions getConditions() const;

	size_t getIndex() const;
	bool isValid() const;
};