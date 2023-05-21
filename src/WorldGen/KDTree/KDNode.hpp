#pragma once
#include "../Chunks/Biome/Biome.hpp"

class KDNode {
public:
	Biome* biome;
	KDNode *left, *right;
	int axis;

	inline KDNode(Biome* biome, int axis) : biome{ biome }, axis{ axis }, left{ nullptr }, right{ nullptr } {}
	~KDNode() { biome = nullptr; }
};