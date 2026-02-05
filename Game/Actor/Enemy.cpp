#include "Enemy.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Actor/EnemyBullet.h"
#include "Level/Level.h"
#include "Actor/EnemyDestroyEffect.h"

Enemy::Enemy(const char* image, int yPosition)
	:super(image)
{
	// 이동 방향에 따른 위치 설정.
	SetPosition(
		Vector2(static_cast<int>(xPosition), yPosition)
	);
}

Enemy::Enemy(const char* image, Vector2 position)
	:super(image, position)
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