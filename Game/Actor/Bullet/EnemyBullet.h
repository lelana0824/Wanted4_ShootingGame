#pragma once
#include "Bullet.h"

using namespace Wanted;

class EnemyBullet : public Bullet
{
	RTTI_DECLARATIONS(EnemyBullet, Bullet)

public:
	EnemyBullet(
		const Vector2& position,
		float moveSpeed = 15.0f,
		Color color = Color::Red
	);

private:
	virtual void Tick(float deltaTime) override;
};

