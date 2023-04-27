#include "BlockType.hpp"

BlockType::BlockType() : name{}, index{ UINT64_MAX }, conditions{}
{
	Godot::print("BlockType constructed");
}

BlockType::BlockType(const String& name, size_t index, const Array& data)
{
	Godot::print("BlockType constructed");
	this->index = index;
	conditions.elevation = { data[0], data[1] };
	conditions.humidity = { data[2], data[3] };
	conditions.temperature = { data[4], data[5] };
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
	return index != UINT64_MAX;
}