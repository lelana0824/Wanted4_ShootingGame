#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

	// 이동 방향 열거형.
	enum class MoveDirection
	{
		None = -1,
		Left,
		Right
	};

public:
	Enemy(const char* image = ">@<", int yPosition = 5);
	~Enemy();

	virtual void Tick(float deltaTime) override;

	void OnDamaged();

private:
	MoveDirection direction = MoveDirection::None;

	float xPosition = 0.0f;
	float moveSpeed = 5.0f;

	// 발사 타이머.
	Timer timer;
};

