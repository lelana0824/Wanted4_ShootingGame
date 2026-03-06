#include "QuadTree.h"

QuadTree::QuadTree() {
	root = new QuadTreeNode(Bounds(0,0));
}

QuadTree::QuadTree(const Bounds& bounds) {
	root = new QuadTreeNode(bounds);
}

QuadTree::~QuadTree() {
	SafeDelete(root);
}

void QuadTree::Insert(QuadTreeNode* node) {
	if (!node) return;

	root->Insert(node);
}


std::vector<QuadTreeNode*> QuadTree::Query(QuadTreeNode* queryNode)
{
	if (!queryNode) return {};

	// 겹침 가능성 있는 영역 확인.
	std::vector<QuadTreeNode*> possibleNodes;
	root->Query(queryNode->GetBounds(), possibleNodes);

	// 실제 겹치는 노드들만 확인
	std::vector<QuadTreeNode*> intersects;
	for (QuadTreeNode* const node : possibleNodes)
	{
		for (QuadTreeNode* const point : node->Points())
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