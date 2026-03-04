#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class Bullet : public Actor
{
	RTTI_DECLARATIONS(Bullet, Actor)

public:
	Bullet(
		const Vector2& position,
		float moveSpeed = 15.0f,
		Color color = Color::Red
	);

protected:
	virtual void Tick(float deltaTime) override;


protected:
	// 초당 몇칸 이동하는지 나타냄.
	float moveSpeed = 30.0f;

	// 위치 갱신을 할 때 소수점처리를 위한 변수.
	float yPosition = 0.0f;
	float xPosition = 0.0f;

	Vector2 sideDirection;
};

