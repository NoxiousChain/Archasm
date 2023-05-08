#pragma once
#include "../Chunks/Blocks/BlockType.hpp"

class KDNode {
public:
	BlockType block;
	KDNode *left, *right;
	int axis;

	inline KDNode(BlockType block, int axis) : block{ block }, axis{ axis }, left{ nullptr }, right{ nullptr } {}
};