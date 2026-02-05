#pragma once
#include "Actor/Enemy.h"

class SmallEnemy : public Enemy
{
	RTTI_DECLARATIONS(SmallEnemy, Enemy)


public:
	SmallEnemy(const char* image = "@", int yPosition = 5);
	virtual void Tick(float deltaTime) override;
	virtual void OnDamaged() override;

private:


};

