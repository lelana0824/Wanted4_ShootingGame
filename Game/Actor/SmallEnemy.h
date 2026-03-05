#pragma once
#include "Actor/Enemy.h"
#include "Actor/Player.h"

class SmallEnemy : public Enemy
{
	RTTI_DECLARATIONS(SmallEnemy, Enemy)


public:
	SmallEnemy(
		const char* image = "@",
		int yPosition = 5,
		MoveDirection moveDirection = MoveDirection::Left
	);
	virtual void Tick(float deltaTime) override;
	virtual void OnDamaged() override;

private:
	Timer firstAppearedTimer;
	Player* player;
};

