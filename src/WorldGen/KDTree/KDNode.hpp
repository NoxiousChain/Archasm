#pragma once
#include "../Chunks/Blocks/BlockType.hpp"
#include <memory>

class KDNode {
public:
	BlockType block;
	std::unique_ptr<KDNode> left, right;
	int axis;

	KDNode(BlockType block, int axis) : block{ block }, axis{ axis } {}
};