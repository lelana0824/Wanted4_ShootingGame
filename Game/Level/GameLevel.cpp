#include "GameLevel.h"
#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/PlayerBullet.h"
#include "Actor/EnemyBullet.h"
#include "Actor/EnemySpawner.h"
#include "Actor/SmallEnemy.h"
#include "Actor/UltraEnemy.h"
#include "Render/Renderer.h"
#include "Engine/Engine.h"

#include <string>
#include <iostream>
#include <fstream> 
#include <sstream>

GameLevel::GameLevel()
{
	std::ifstream file("../Assets/UltraEnemy.txt");

	if (!file.is_open()) {
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();;

	AddNewActor(new Player());
	AddNewActor(new EnemySpawner());
	AddNewActor(new UltraEnemy(buffer.str().c_str()));
}

GameLevel::~GameLevel()
{

}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	ProcessCollisionPlayerBulletAndEnemy();
	ProcessCollisionPlayerAndEnemyBullet();
	ProcessCollisionPlayerAndUltraEnemy();
}

void GameLevel::Draw()
{
	super::Draw();

	if (isPlayerDead)
	{
		// 플레이어 죽음 메시지 Renderer에 제출.
		Renderer::Get().Submit("!Dead!", playerDeadPosition);

		// 점수 보여주기.
		ShowScore();

		// 화면에 바로 표시.
		Renderer::Get().PresentImmediately();

		// 프로그램 정지.
		Sleep(2000);

		// 게임 종료.
		Engine::Get().QuitEngine();
	}

	// 점수 보여주기.
	ShowScore();
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

		if (actor->IsTypeOf<SmallEnemy>())
		{
			enemies.emplace_back(actor->As<SmallEnemy>());
		}

		if (actor->IsTypeOf<UltraEnemy>())
		{
			enemies.emplace_back(actor->As<UltraEnemy>());
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
			// 대형 유닛의 일부와 AABB 겹침 판정.
			if (enemy->IsTypeOf<UltraEnemy>())
			{
				for (Enemy* const parts : enemy->As<UltraEnemy>()->GetEnemies())
				{
					if (bullet->TestIntersect(parts))
					{
						enemy->OnDamaged();
						bullet->Destroy();
						score += 10;
						continue;
					}
				}
			}
			else if (enemy->IsTypeOf<SmallEnemy>()) {
				if (bullet->TestIntersect(enemy))
				{
					enemy->OnDamaged();
					bullet->Destroy();

					score += 1;
					continue;
				}
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
			health -= 1;

			if (health <= 0)
			{
				isPlayerDead = true;

				playerDeadPosition = player->GetPosition();

				player->Destroy();
				break;
			}

			bullet->Destroy();
		}
	}
}

void GameLevel::ProcessCollisionPlayerAndUltraEnemy()
{
	Player* player = nullptr;
	std::vector<Enemy*> enemies;

	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<SmallEnemy>())
		{
			continue;
		}

		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (actor->IsTypeOf<Enemy>())
		{
			enemies.emplace_back(actor->As<Enemy>());
		}
	}

	if (enemies.size() == 0 || !player)
	{
		return;
	}


	// 충돌판정.
	for (Enemy* const enemy : enemies)
	{
		if (enemy->GetCanHitOtherActor())
		{
			if (enemy->TestIntersect(player))
			{
				health -= 1;

				if (health <= 0)
				{
					isPlayerDead = true;

					playerDeadPosition = player->GetPosition();

					player->Destroy();
					break;
				}
			}
		}
	}
}

void GameLevel::ShowScore()
{
	sprintf_s(scoreString, 128, "Score: %d    Health: %d", score, health);
	Renderer::Get().Submit(
		scoreString,
		Vector2(0, Engine::Get().GetHeight() - 1)
	);
}