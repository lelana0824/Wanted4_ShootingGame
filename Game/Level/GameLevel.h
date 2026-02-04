#pragma once
#include "Level/Level.h"

using namespace Wanted;

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

private:
	virtual void Tick(float deltaTime) override;

	// 충돌 판정 처리 함수.
	void ProcessCollisionPlayerBulletAndEnemy();
	void ProcessCollisionPlayerAndEnemyBullet();

private:

};

