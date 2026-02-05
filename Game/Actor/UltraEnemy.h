#pragma once
#include "Actor/Actor.h"
#include "Actor/Enemy.h"

#include <vector>

using namespace Wanted;

class UltraEnemy : public Enemy
{
	RTTI_DECLARATIONS(UltraEnemy, Enemy)

public:
	UltraEnemy(const char* enemies);

	virtual void Tick(float deltaTime) override;
	virtual void OnDamaged() override;

private:
	std::vector<Enemy*> enemies;
	Vector2 position = Vector2(5, 5);
};

