#include "PlayerBullet.h"
#include "Engine/Engine.h"

PlayerBullet::PlayerBullet(
	const Vector2& position,
	const Vector2& sideDirection
) :super(position, 30.0f, Color::Green)
{
	this->xPosition = (static_cast<float>(position.x));
	this->yPosition = (static_cast<float>(position.y));
	this->sideDirection = sideDirection;

}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (sideDirection.x == 0)
	{
		yPosition -= moveSpeed * deltaTime;
	} 
	else if (sideDirection.x == -1)
	{
		xPosition -= moveSpeed * deltaTime;
	} 
	else if (sideDirection.x == 1)
	{
		xPosition += moveSpeed * deltaTime;
	}


	// 좌표검사
	if (yPosition < 0.0f) {
		Destroy();
		return;
	}

	if (xPosition < 0.0f) {
		Destroy();
		return;
	}

	if (xPosition >= Engine::Get().GetWidth()) {
		Destroy();
		return;
	}


	// 액터의 위치로 변환.
	Vector2 newPosition = GetPosition();
	newPosition.x = static_cast<int>(xPosition);
	newPosition.y = static_cast<int>(yPosition);


	// 위치 갱신
	SetPosition(newPosition);
}
