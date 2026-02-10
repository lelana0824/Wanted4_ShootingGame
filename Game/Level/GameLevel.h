#pragma once
#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Util/Timer.h"
#include <sstream>


using namespace Wanted;

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();
	~GameLevel();

private:
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 충돌 판정 처리 함수.
	void ProcessCollisionPlayerBulletAndEnemy();
	void ProcessCollisionPlayerAndEnemyBullet();
	void ProcessCollisionPlayerAndUltraEnemy();

	void ShowScore();
	void ShowUltraEnemy();
	void ClearAllActors();

private:
	// 점수 변수.
	int score = 0;

	// 플레이어가 죽었는지 확인.
	bool isPlayerDead = false;

	// 플레이어가 죽은 위치 (Draw에서 처리하기 위해 Tick에서 저장).
	Vector2 playerDeadPosition;

	// 점수 문자열.
	char scoreString[128] = {};

	// 플레이어 체력
	int health = 20;

	// 대형 유닛 소환을 위한 목표 점수
	int targetScoreForShowingUltraEnemy = 5;

	// 대형 유닛 소환 여부
	bool hasShownUltraEnemy = false;

	bool isGameClear = false;

	std::stringstream ultraEnemyBuffer;

	Timer deadEventTimer;
};

