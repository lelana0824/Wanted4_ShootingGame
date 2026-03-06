#pragma once

#include "Node.h"

class QuadTree
{
public:
	QuadTree(const Bounds& bounds);
	~QuadTree();

	void Insert(Node* node);

	std::vector<Node*> Query(Node* queryNode);

public:
	static const int maxDepth = 5;

private:
	Node* root = nullptr;

};

