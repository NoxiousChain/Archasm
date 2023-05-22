#pragma once
#include <vector>
#include <Dictionary.hpp>
#include <String.hpp>
#include "../../../Common.hpp"
#include "../../Helpers/RRange.hpp"

class Biome {
public:
	struct Conditions {
		RRange<double> elevation, humidity, temperature;

		inline static String toString(const RRange<double>& what) {
			return String::num_real(what.min) + String("-") + String::num_real(what.max);
		}
	};

private:
	String name;
	Conditions conditions;
	// List of all blocks per biome. Not worried about duplicate memory because there are few biomes.
	// block name string maps to frequency
	vector<pair<String, double>> blocks; 

public:
	Biome();
	Biome(String name, const Dictionary& data);
	~Biome();

	RRange<double> getElevationRange() const;
	RRange<double> getHumidityRange() const;
	RRange<double> getTempRange() const;
	Conditions getConditions() const;
	vector<pair<String, double>> getBlocks();
	String getName() const { return name; }

	void addBlock(String name, double freq);

	String toString() const;

	bool isValid() const;
};