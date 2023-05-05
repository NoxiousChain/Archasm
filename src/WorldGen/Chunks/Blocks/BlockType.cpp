#include "BlockType.hpp"

BlockType::BlockType() : name{}, index{ SIZE_MAX }, conditions{}
{
}

BlockType::BlockType(const String& name, const Dictionary& data, size_t index)
	: name{ name }, index { index },
	conditions {
		{ data["elevation_min"], data["elevation_max"] },
		{ data["humidity_min"], data["humidity_max"] },
		{ data["temperature_min"], data["temperature_max"] }
	}
{
}

BlockType::Range BlockType::getElevationRange() const
{
	return conditions.elevation;
}

BlockType::Range BlockType::getHumidityRange() const
{
	return conditions.humidity;
}

BlockType::Range BlockType::getTempRange() const
{
	return conditions.temperature;
}

BlockType::Conditions BlockType::getConditions() const
{
	return conditions;
}

size_t BlockType::getIndex() const
{
	return index;
}

bool BlockType::isValid() const
{
	return index != SIZE_MAX;
}

String BlockType::toString() const
{
	return String(String("Name: ") + name + String("\nIndex: ") + String::num_int64(index) + String("\nConditions:\n\tElevation: ") + Conditions::toString(conditions.elevation) + "\n\tHumidity: " + Conditions::toString(conditions.humidity) + "\n\tTemperature: " + Conditions::toString(conditions.temperature));
}