#include "BlockType.hpp"

BlockType::BlockType(const String& name, size_t index, double elevationMin, double elevationMax, double humidityMin, double humidityMax, double tempMin, double tempMax)
{
	this->index = index;
	conditions.elevation = { elevationMin, elevationMax };
	conditions.humidity = { humidityMin, humidityMax };
	conditions.temperature = { tempMin, tempMax };
}

BlockType::BlockType(const String& name, size_t index, const std::vector<double>& data)
{
	this->index = index;
	conditions.elevation = { data[0], data[1] };
	conditions.humidity = { data[2], data[3] };
	conditions.temperature = { data[4], data[5] };
}

BlockType::BlockType(const String& name, size_t index, const Array& data)
{
	this->index = index;
	conditions.elevation = { data[0], data[1] };
	conditions.humidity = { data[2], data[3] };
	conditions.temperature = { data[4], data[5] };
}

BlockType::Range& BlockType::getElevationRange() const
{
	return (Range&)conditions.elevation;
}

BlockType::Range& BlockType::getHumidityRange() const
{
	return (Range&)conditions.humidity;
}

BlockType::Range& BlockType::getTempRange() const
{
	return (Range&)conditions.temperature;
}

BlockType::Conditions& BlockType::getConditions() const
{
	return (Conditions&)conditions;
}

size_t BlockType::getIndex() const
{
	return index;
}