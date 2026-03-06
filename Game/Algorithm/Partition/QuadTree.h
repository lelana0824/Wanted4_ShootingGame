#pragma once

#include "Node.h"

class QuadTree
{
public:
	QuadTree(const Bounds& bounds);
	QuadTree();
	~QuadTree();

	void Insert(QuadTreeNode* node);

	std::vector<QuadTreeNode*> Query(QuadTreeNode* queryNode);

public:
	static const int maxDepth = 5;

private:
	QuadTreeNode* root = nullptr;

};

