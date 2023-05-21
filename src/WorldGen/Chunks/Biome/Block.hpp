#pragma once

#include "../../../Common.hpp"
#include "../../Helpers/RRange.hpp"
#include <RandomNumberGenerator.hpp>
#include <Mutex.hpp>

class Block {
protected:
	String name;
	size_t index;
	int64_t hardness;
	bool surfaceOnly;

public:
	Block();
	Block(const String& name, size_t index, int64_t hardness, bool surfaceOnly);

	bool isValid() const;
	virtual String toString() const;
};

class Ore : public Block {
protected:
	RRange<size_t> clumpSize;
	RRange<double> depth;
	double frequency; // ores are evenly spread out through the world, so biomes don't hold frequency for ores

public:
	Ore();
	Ore(const String& name, size_t index, int64_t hardness, const RRange<size_t>& clumpRange, const RRange<double>& depthRange, double frequency);

	virtual String toString() const override;
};