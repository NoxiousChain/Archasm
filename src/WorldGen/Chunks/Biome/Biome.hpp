#pragma once
#include <vector>
#include <Dictionary.hpp>
#include <String.hpp>
#include "../../../Common.hpp"
#include "Block.hpp"

class Biome {
public:
	struct Conditions {
		JRange<double> elevation, humidity, temperature;

		inline static String toString(const JRange<double>& what) {
			return String::num_real(what.min) + String("-") + String::num_real(what.max);
		}
	};

private:
	String name;
	Conditions conditions;
	// List of all blocks per biome. Not worried about duplicate memory because there are few biomes.
	// block name string maps to frequency
	vector<pair<String, double>>* blocks; 

public:
	Biome();
	Biome(const String& name, const Dictionary& data);

	JRange<double> getElevationRange() const;
	JRange<double> getHumidityRange() const;
	JRange<double> getTempRange() const;
	const Conditions& getConditions() const;
	const vector<pair<String, double>>* getBlocks();

	void addBlock(const String& name, double freq);

	String toString() const;

	bool isValid() const;
};