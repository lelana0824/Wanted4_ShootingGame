#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"
#include "Actor/Item.h"

#include <map>

using namespace Wanted;

class Player : public Actor
{
	// 발사 모드.
	enum class FireMode {
		None = -1,
		OneShot,
		Repeat
	};

	enum class Direction {
		Up,
		Down,
		Left,
		Right
	};

	RTTI_DECLARATIONS(Player, Actor)

public:
	std::map< Direction, Vector2> directionMap;

	Player();
	~Player();
	void ConsumeItem(Item* item);
	// Getter
	Vector2& GetCurrentDirection() { return currentDirection; }
private:
	virtual void Tick(float deltaTime) override;

	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();

	void Fire();

	void FireInterval();

	// 발사 가능 여부 확인
	bool CanShoot() const;

private:
	// 발사 모드
	FireMode fireMode = FireMode::None;
	Item::FireDirectionType fireDirection = Item::FireDirectionType::Top;
	int bulletSpawnCount = 1;

	Timer timer = Timer(0.2f);
	Vector2 currentDirection = Vector2::Zero;
};

