#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)


public:
	// 이동 방향 열거형.
	enum class MoveDirection
	{
		None = -1,
		Left,
		Right
	};

public:
	Enemy(const char* image = "@", int yPosition = 5);
	Enemy(const char* image = "@", Vector2 position = Vector2::Zero, Color color = Color::Green);
	~Enemy();

	virtual void Tick(float deltaTime) override;
	virtual void OnDamaged();
	virtual bool TestIntersect(const Actor* const other) override;

	inline float GetXPosition() { return xPosition;  }
	inline void SetXPosition(float inXPosition) { 
		xPosition = inXPosition;
		return; 
	}

	inline float GetYPosition() { return yPosition; }
	inline void SetYPosition(float inYPosition) {
		yPosition = inYPosition;
		return;
	}

	inline const char* GetImage() const { return image; }
	inline Timer& GetTimer() { return timer; }
	inline bool GetCanHitOtherActor() { return canHitOtherActor; }
	inline void SetMoveSpeed(float newMoveSpeed) { moveSpeed = newMoveSpeed; }

protected:
	MoveDirection direction = MoveDirection::None;

	float xPosition = 0.0f;
	float yPosition = 0.0f;
	float moveSpeed = 5.0f;
	bool canHitOtherActor = true;

	Timer timer;
};

