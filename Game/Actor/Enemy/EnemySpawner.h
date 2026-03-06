#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class EnemySpawner : public Actor
{
	RTTI_DECLARATIONS(EnemySpawner, Actor)

public:
	EnemySpawner();
	inline void StopSpawn() { isStopSpawn = true; };

private:
	virtual void Tick(float deltaTIme) override;

	void SpawnEnemy(float deltaTIme);

private:
	Timer timer;
	bool isStopSpawn = false;
};

