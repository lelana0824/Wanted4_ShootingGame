#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class Player : public Actor
{
	// 발사 모드.
	enum class FireMode {
		None = -1,
		OneShot,
		Repeat
	};

	RTTI_DECLARATIONS(Player, Actor)

public:
	Player();
	~Player();

private:
	virtual void Tick(float deltaTime) override;

	void MoveRight();
	void MoveLeft();

	void Fire();

	void FireInterval();

	// 발사 가능 여부 확인
	bool CanShoot() const;
private:
	// 발사 모드
	FireMode fireMode = FireMode::None;

	Timer timer = Timer(0.2f);
};

