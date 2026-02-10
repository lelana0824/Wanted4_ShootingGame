#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class PlayerBullet : public Actor
{
	RTTI_DECLARATIONS(PlayerBullet, Actor)


public:
	PlayerBullet(
		const Vector2& position, 
		const Vector2& sideDirection = Vector2::Zero
	);
	~PlayerBullet();

	virtual void Tick(float deltatime) override;

private:
	// 초당 몇칸 이동하는지 나타냄.

	float moveSpeed = 30.0f;

	// 위치 갱신을 할 때 소수점처리를 위한 변수.
	float yPosition = 0.0f;
	float xPosition = 0.0f;

	Vector2 sideDirection;
};

