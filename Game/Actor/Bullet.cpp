#include "Bullet.h"
#include "Engine/Engine.h"


Bullet::Bullet(
	const Vector2& position, float moveSpeed, Color color)
	: super("*", position, color),
	moveSpeed(moveSpeed),
	yPosition(static_cast<float>(position.y))
{
}

void Bullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}
