#include "Biome.hpp"

Biome::Biome() : name{}, conditions{}
{
}

Biome::Biome(const String& name, const Dictionary& data)
	: name{ name },
	conditions {
		{ (double)data["elevation_min"], (double)data["elevation_max"] },
		{ (double)data["humidity_min"], (double)data["humidity_max"] },
		{ (double)data["temperature_min"], (double)data["temperature_max"] }
	}
{
}

Biome::~Biome()
{
}

RRange<double> Biome::getElevationRange() const
{
	return conditions.elevation;
}

RRange<double> Biome::getHumidityRange() const
{
	return conditions.humidity;
}

RRange<double> Biome::getTempRange() const
{
	return conditions.temperature;
}

Biome::Conditions Biome::getConditions() const
{
	return conditions;
}

vector<pair<String, double>> Biome::getBlocks()
{
	return blocks;
}

void Biome::addBlock(const String& name, double freq)
{
	blocks.push_back(make_pair(name, freq));
}

String Biome::toString() const
{
	return String(String("Name: ") + name + String("\nConditions:\n\tElevation: ") + Conditions::toString(conditions.elevation) + "\n\tHumidity: " + Conditions::toString(conditions.humidity) + "\n\tTemperature: " + Conditions::toString(conditions.temperature));
}

bool Biome::isValid() const
{
	return !name.empty();
}