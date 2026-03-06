#pragma once
#include "Math/Vector2.h"
#include "Util/Util.h"
using namespace Wanted;

class Node
{

public:
	Node(int x, int y, Node* parentNode = nullptr)
		:position(x, y), parentNode(parentNode)
	{

	}
	
	Vector2 operator-(const Node& other) const
	{
		return Vector2(position.x - other.position.x,
			position.y - other.position.y);
	}

	bool operator==(const Node& other) const
	{
		return position.x == other.position.x
			&& position.y == other.position.y;
	}

public:
	Vector2 position;

	float gCost = 0.0f;

	float hCost = 0.0f;

	float fCost = 0.0f;

	Node* parentNode = nullptr;
};