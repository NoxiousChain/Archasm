#pragma once
#include "../../Common.hpp"
#include <RandomNumberGenerator.hpp>
#include <Mutex.hpp>

// Threadsafe range structure
template<class T>
class RRange {
private:
	RandomNumberGenerator* rng;
	Mutex* mtx;

public:
	T min, max;

	inline RRange() { init(); }
	inline RRange(T min, T max) : min{ min }, max{ max } { init(); }
	inline ~RRange() { if (rng) rng->free(); if (mtx) mtx->free(); }

	// Difference between min and max
	inline T diff() const { return max - min; }

	// Generates a random number
	inline T rand() {
		lock();
		T ret = urand();
		unlock();
		return ret;
	}

	// Generates a weighted random number within the range; assumes weight is between [0, 1]
	inline T wrand(float weight) {
		lock();
		T ret = uwrand(weight);
		unlock();
		return ret;
	}

	// For manual mutex management
	// Use these to generate several random numbers in succession without overhead of mutex lock/unlock
	inline T urand() const {
		return static_cast<T>(rng->randi()) % (diff() + 1) + min;
	}

	inline T uwrand(float weight) const {
		// Calculate split point based on weight
		T split = min + static_cast<T>(diff() * weight);
		// Get a random value and see which side of the split it lands on
		T randVal = static_cast<T>(rng->randf_range(0.f, 1.f));

		if (randVal <= weight) // Generate a value in lower portion of range
			return static_cast<T>(rng->randi()) % (split - min + 1) + min;
		else // Generate a value in upper portion of range
			return static_cast<T>(rng->randi()) % (max - split) + split + 1;
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