#include "Biome.hpp"

Biome::Biome() : name{}, conditions{}, blocks{ new vector<pair<String, double>> }
{
}

Biome::Biome(const String& name, const Dictionary& data)
	: name{ name },
	conditions {
		{ data["elevation_min"], data["elevation_max"] },
		{ data["humidity_min"], data["humidity_max"] },
		{ data["temperature_min"], data["temperature_max"] }
	},
	blocks{ new vector<pair<String, double>> }
{
}

JRange<double> Biome::getElevationRange() const
{
	return conditions.elevation;
}

JRange<double> Biome::getHumidityRange() const
{
	return conditions.humidity;
}

JRange<double> Biome::getTempRange() const
{
	return conditions.temperature;
}

const Biome::Conditions& Biome::getConditions() const
{
	return conditions;
}

const vector<pair<String, double>>* Biome::getBlocks()
{
	return blocks;
}

void Biome::addBlock(const String& name, double freq)
{
	blocks->emplace_back(make_pair(name, freq));
}

String Biome::toString() const
{
	return String(String("Name: ") + name + String("\nConditions:\n\tElevation: ") + Conditions::toString(conditions.elevation) + "\n\tHumidity: " + Conditions::toString(conditions.humidity) + "\n\tTemperature: " + Conditions::toString(conditions.temperature));
}

bool Biome::isValid() const
{
	return name.empty();
}