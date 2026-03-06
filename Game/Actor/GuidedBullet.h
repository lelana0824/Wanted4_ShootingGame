#pragma once
#include "Bullet.h"
#include "../Algorithm/Navigation/Node.h"
#include "../Algorithm/Navigation/AStar.h"
#include "Actor/Player.h"

class GuidedBullet : public Bullet
{
	RTTI_DECLARATIONS(GuidedBullet, Bullet);

public:
	GuidedBullet(
		const Vector2& position,
		Player* targetActor,
		float moveSpeed = 15.0f,
		Color color = Color::White
	);
	~GuidedBullet();

private:
	virtual void Tick(float deltaTime) override;


private:
	Node* startNode = nullptr;
	Node* goalNode = nullptr;

	// ±Ê√£±‚
	AStar aStar;
	std::vector<Node*> path;
	Vector2 nextPosition;
	Player* target;

	bool findedPath = false;
};

