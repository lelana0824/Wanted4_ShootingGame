#include "EnemyBullet.h"
#include "Engine/Engine.h"

EnemyBullet::EnemyBullet(
	const Vector2& position, float moveSpeed, Color color)
	: super("!", position, color),
	moveSpeed(moveSpeed),
	yPosition(static_cast<float>(position.y))
{
	//sortingOrder = 5;
}

void EnemyBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// yPosition 업데이트
	yPosition = yPosition + moveSpeed * deltaTime;

	if (yPosition >= Engine::Get().GetHeight())
	{
		Destroy();
		return;
	}

	SetPosition(
		Vector2(
			position.x,
			static_cast<int>(yPosition)
		)
	);
}
