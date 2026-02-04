#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class EnemyBullet : public Actor
{
	RTTI_DECLARATIONS(EnemyBullet, Actor)

public:
	EnemyBullet(
		const Vector2& position,
		float moveSpeed = 15.0f
	);

private:
	virtual void Tick(float deltaTime) override;


private:
	float moveSpeed = 0.0f;

	float yPosition = 0.0f;
};

