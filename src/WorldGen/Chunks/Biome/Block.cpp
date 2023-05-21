#include "Block.hpp"

Block::Block()
	: index{ SIZE_MAX }
{
}

Block::Block(const String& name, size_t index, int64_t hardness, bool surfaceOnly)
	: name{ name }, index{ index }, hardness{ hardness }, surfaceOnly{ surfaceOnly }
{
}

bool Block::isValid() const
{
	return index == SIZE_MAX;
}

String Block::toString() const
{
	return String("Name: ") + name + String("\nIndex: ") + String::num_int64(index) + String("\nHardness: ") + String::num_int64(hardness) + String("\nSurfaceOnly: ") + String(surfaceOnly ? "true" : "false");
}

Ore::Ore()
	: Block()
{
}

Ore::Ore(const String& name, size_t index, int64_t hardness, const RRange<size_t>& clumpRange, const RRange<double>& depthRange, double frequency)
	: Block(name, index, hardness, false), clumpSize{ clumpRange }, depth{ depthRange }, frequency{ frequency }
{
}

String Ore::toString() const
{
	return Block::toString() + \
		String("\nClumpSize: ") + \
		String::num_int64(clumpSize.min) + String("-") + String::num_int64(clumpSize.max) + \
		String("\nDepth: ") + \
		String::num_real(depth.min) + String("-") + String::num_real(depth.max);
}