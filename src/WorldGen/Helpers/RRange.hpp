#pragma once
#include "../../Common.hpp"
#include <RandomNumberGenerator.hpp>
#include <Mutex.hpp>

// Basic range structure
template<class T>
class SRange {
public:
	T min, max;

	SRange() {}
	SRange(T min, T max) : min{ min }, max{ max } {}

	inline T diff() const { return max - min; }
	inline T mid() const { return (max + min) / (T)2; }
};

// Range structure with RNG
template<class T>
class RRange : public SRange<T> {
private:
	Ref<RandomNumberGenerator> rng;

public:
	RRange() { init(); }
	RRange(T min, T max) : SRange<T>(min, max) { init(); }

	inline void init() {
		rng.instance();
	}

	// Generates a random number within the range
	inline T rand() {
		return static_cast<T>(rng->randi()) % (diff() + 1) + min;
	}

	// Generates a weighted random number within the range; assumes weight is between [0, 1]
	inline T wrand(float weight) {
		// Calculate split point based on weight
		T split = min + static_cast<T>(diff() * weight);
		// Get a random value and see which side of the split it lands on
		T randVal = static_cast<T>(rng->randf_range(0.f, 1.f));

		if (randVal <= weight) // Generate a value in lower portion of range
			return static_cast<T>(rng->randi()) % (split - min + 1) + min;
		else // Generate a value in upper portion of range
			return static_cast<T>(rng->randi()) % (max - split) + split + 1;
	}
};