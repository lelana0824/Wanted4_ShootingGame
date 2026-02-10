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
	virtual bool GetCanHitOtherActor() override;

	void CreateBody();
	void Move(float deltaTime);
	void Shot(float deltaTime);
	
	inline std::vector<Enemy*> GetEnemies() {
		return enemies;
	}
	inline bool hasAllBodyShown() { return timer.IsTimeOut(); }
	inline bool IsDead() { return health <= 0; }

private:
	std::vector<Enemy*> enemies;
	std::vector<Enemy*> bulletSpawners;

	int health = 10;
	Vector2 position = Vector2(5, 5);

	Timer moveDirectionChangeTimer;
	Timer canHitOtherActorTimer;
};

