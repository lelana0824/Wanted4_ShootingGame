#pragma once

#include "Node.h"

class QuadTree
{
public:
	QuadTree();
	QuadTree(const Bounds& bounds);
	~QuadTree();

	void Insert(QuadTreeNode* node);

	std::vector<QuadTreeNode*> Query(const Bounds& bounds);

public:
	static const int maxDepth = 5;
	QuadTreeNode* root = nullptr;


private:

};

