#include "Enemy.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Actor/EnemyBullet.h"
#include "Level/Level.h"
#include "Actor/EnemyDestroyEffect.h"

Enemy::Enemy(const char* image, int yPosition)
	:super(image, Vector2(static_cast<int>(xPosition), yPosition), Color::Red), yPosition(yPosition)
{
	
}

Enemy::Enemy(const char* image, Vector2 position, Color color)
	:super(image, position, color)
{
	SetPosition(position);
}


Enemy::~Enemy()
{
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

}

void Enemy::OnDamaged()
{

}

bool Enemy::TestIntersect(const Actor* const other)
{
	bool hasIntersected = super::TestIntersect(other);

	if (hasIntersected)
	{
		canHitOtherActor = false;
	}
	else {
		canHitOtherActor = true;
	}

	return hasIntersected;
}
