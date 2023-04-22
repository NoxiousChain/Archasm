#include "BlockType.hpp"

BlockType::BlockType(const String& name, size_t index, double elevationMin, double elevationMax, double wetnessMin, double wetnessMax, double tempMin, double tempMax)
{
	this->index = index;
	conditions.elevation = { elevationMin, elevationMax };
	conditions.wetness = { wetnessMin, wetnessMax };
	conditions.temperature = { tempMin, tempMax };
}

BlockType::BlockType(const String& name, size_t index, const std::vector<double>& data)
{
	this->index = index;
	conditions.elevation = { data[0], data[1] };
	conditions.wetness = { data[2], data[3] };
	conditions.temperature = { data[4], data[5] };
}

BlockType::BlockType(const String& name, size_t index, const Array& data)
{
	this->index = index;
	conditions.elevation = { data[0], data[1] };
	conditions.wetness = { data[2], data[3] };
	conditions.temperature = { data[4], data[5] };
}

Range BlockType::getElevationRange() const
{
	return conditions.elevation;
}

Range BlockType::getWetnessRange() const
{
	return conditions.wetness;
}

Range BlockType::getTempRange() const
{
	return conditions.temperature;
}

BlockType::Conditions getConditions() const
{
	return conditions;
}

size_t BlockType::getIndex() const
{
	return index;
}