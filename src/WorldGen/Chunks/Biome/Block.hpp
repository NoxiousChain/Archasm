#pragma once

#include "../../../Common.hpp"
#include <RandomNumberGenerator.hpp>
#include <Mutex.hpp>
#include <Range.hpp>

// Threadsafe range structure
template<class T>
class JRange {
private:
	RandomNumberGenerator* rng;
	Mutex* mtx;

public:
	T min, max;

	inline JRange() { init(); }
	inline JRange(T min, T max) : min{ min }, max{ max } { init(); }
	inline ~JRange() { if (rng) rng->free(); if (mtx) mtx->free(); }

	// Difference between min and max
	inline T diff() const { return max - min; }
	// Generates a random number; threadsafe
	inline T rand() { 
		lock();
		T ret = urand();
		unlock();
		return ret;
	}

	// For manual mutex management
	// Use these to generate several random numbers in succession without overhead of mutex lock/unlock
	inline T urand() const {
		return static_cast<T>(rng->randi()) % (diff() + 1) + min;
	}
	// Locks mutex
	inline void lock() {
		mtx->lock();
	}
	// Unlocks mutex
	inline void unlock() {
		mtx->unlock();
	}

private:
	inline void init() {
		rng = RandomNumberGenerator::_new();
		mtx = Mutex::_new();
	}
};

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
	JRange<size_t> clumpSize;
	JRange<double> depth;
	double frequency; // ores are evenly spread out through the world, so biomes don't hold frequency for ores

public:
	Ore();
	Ore(const String& name, size_t index, int64_t hardness, const JRange<size_t>& clumpRange, const JRange<double>& depthRange, double frequency);

	virtual String toString() const override;
};