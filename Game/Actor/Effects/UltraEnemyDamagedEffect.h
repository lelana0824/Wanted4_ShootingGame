#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"
#include <vector>

using namespace Wanted;

class UltraEnemyDamagedEffect : public Actor
{
	RTTI_DECLARATIONS(UltraEnemyDamagedEffect, Actor)

public:
	UltraEnemyDamagedEffect(const std::vector<class Enemy*>& parts, float duration = 0.2f);
	
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	struct Particle {
		Vector2 offset;
		char image;
	};

	std::vector<Vector2> targetPositions;
	Timer lifeTimer;
	Color effectColor = Color::White;
};
