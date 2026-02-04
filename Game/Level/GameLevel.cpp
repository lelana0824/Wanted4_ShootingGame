#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/PlayerBullet.h"
#include "Actor/EnemyBullet.h"
#include "Actor/EnemySpawner.h"

GameLevel::GameLevel()
{
	// player 액터 추가.
	AddNewActor(new Player());
	AddNewActor(new EnemySpawner());
}

GameLevel::~GameLevel()
{

}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	ProcessCollisionPlayerBulletAndEnemy();
	ProcessCollisionPlayerAndEnemyBullet();
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// 플레이어 탄약과 적 액터 필터링.
	std::vector<Actor*> bullets;
	std::vector<Enemy*> enemies;

	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<PlayerBullet>())
		{
			bullets.emplace_back(actor);
			continue;
		}

		if (actor->IsTypeOf<Enemy>())
		{
			enemies.emplace_back(actor->As<Enemy>());
		}
	}

	// 판정 안해도 되는지 확인.
	if (bullets.size() == 0 || enemies.size() == 0)
	{
		return;
	}

	// 충돌판정
	for (Actor* const bullet : bullets)
	{
		for (Enemy* const enemy : enemies)
		{
			// AABB 겹침 판정.
			if (bullet->TestIntersect(enemy))
			{
				enemy->OnDamaged();
				bullet->Destroy();

				// todo: 점수추가
				continue;
			}
		}
	}
}

void GameLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	// 액터 필터링을 위한 변수.
	Player* player = nullptr;
	std::vector<Actor*> bullets;

	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (actor->IsTypeOf<EnemyBullet>())
		{
			bullets.emplace_back(actor);
		}
	}

	if (bullets.size() == 0 || !player)
	{
		return;
	}


	// 충돌판정.
	for (Actor* const bullet : bullets)
	{
		if (bullet->TestIntersect(player))
		{
			player->Destroy();
			bullet->Destroy();
			break;
		}
	}
}
