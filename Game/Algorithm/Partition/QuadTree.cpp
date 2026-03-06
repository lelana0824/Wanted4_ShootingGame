#include "QuadTree.h"

QuadTree::QuadTree(const Bounds& bounds) {
	root = new Node(bounds);
}

QuadTree::~QuadTree() {
	SafeDelete(root);
}

void QuadTree::Insert(Node* node) {
	if (!node) return;

	root->Insert(node);
}


std::vector<Node*> QuadTree::Query(Node* queryNode)
{
	if (!queryNode) return {};

	// 겹침 가능성 있는 영역 확인.
	std::vector<Node*> possibleNodes;
	root->Query(queryNode->GetBounds(), possibleNodes);

	// 실제 겹치는 노드들만 확인
	std::vector<Node*> intersects;
	for (Node* const node : possibleNodes)
	{
		for (Node* const point : node->Points())
		{
			if (point->GetBounds().Intersects(queryNode->GetBounds()))
			{
				intersects.emplace_back(point);
				continue;
			}
		}
	}

	return intersects;
}