#include "GameLevel.h"

#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/PlayerBullet.h"
#include "Actor/EnemyBullet.h"
#include "Actor/EnemySpawner.h"
#include "Actor/SmallEnemy.h"
#include "Actor/UltraEnemy.h"
#include "Actor/Obstacle.h"
#include "Actor/Item.h"

#include "Render/Renderer.h"
#include "Engine/Engine.h"

#include <string>
#include <iostream>
#include <fstream> 

GameLevel::GameLevel()
{
	std::ifstream file("../Assets/UltraEnemy.txt");

	if (!file.is_open()) {
	}

	ultraEnemyBuffer << file.rdbuf();

	file.close();;

	AddNewActor(new EnemySpawner());
	AddNewActor(new Player());
	AddNewActor(new Obstacle());

	deadEventTimer.SetTargetTime(3.0f);
}

GameLevel::~GameLevel()
{
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	if (isGameClear)
	{
		deadEventTimer.Tick(deltaTime);
	}

	ShowUltraEnemy();

	ProcessCollisionPlayerBulletAndEnemy();
	ProcessCollisionPlayerAndEnemyBullet();
	ProcessCollisionPlayerAndUltraEnemy();
	ProcessCollisionPlayerAndItem();
	ProcessCollisionObstacleAndOther();
}

void GameLevel::Draw()
{
	super::Draw();

	if (deadEventTimer.IsTimeOut())
	{
		ClearAllActors();
		system("cls");
		/*
  ____ _                   _ _
 / ___| | ___  __ _ _ __  | | |
| |   | |/ _ \\/ _` | '__| | | |
| |___| |  __/ (_| | |    |_|_|
 \\____|_|\\___|\\__,_|_|    (_|_)
		*/
		// 플레이어 죽음 메시지 Renderer에 제출.

		Renderer::Get().Submit(
			"###############################################################################\n"
			"###############################################################################\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                       ____ _                   _ _                        ##\n"
			"##                      / ___| | ___  __ _ _ __  | | |                       ##\n"
			"##                     | |   | |/ _ \\/ _` | '__| | | |                       ##\n"
			"##                     | |___| |  __/ (_| | |    |_|_|                       ##\n"
			"##                      \\____|_|\\___|\\__,_|_|    (_|_)                       ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"##                                                                           ##\n"
			"###############################################################################\n"
			"###############################################################################\n",
			Vector2::Zero,
			Color::Blue,
			10
		);

		// 화면에 바로 표시.
		Renderer::Get().PresentImmediately();

		// 프로그램 정지.
		Sleep(5000);

		// 게임 종료.
		Engine::Get().QuitEngine();

		return;
	}

	// game over
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
				UltraEnemy* uEnemy = enemy->As<UltraEnemy>();

				if (!uEnemy->hasAllBodyShown())
				{
					continue;
				}

				for (Enemy* const parts : uEnemy->GetEnemies())
				{
					// !! Enemy는 피격 여부만 확인하고 실제 로직은 uEnemy에 적용
					if (bullet->TestIntersect(parts))
					{
						uEnemy->OnDamaged();
						bullet->Destroy();
						score += 10;

						if (uEnemy->IsDead())
						{
							isGameClear = true;
						}
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
	UltraEnemy* uEnemy = nullptr;
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

		if (!uEnemy && actor->IsTypeOf<UltraEnemy>())
		{
			uEnemy = actor->As<UltraEnemy>();
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
			if (uEnemy->GetCanHitOtherActor() && enemy->TestIntersect(player))
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

void GameLevel::ProcessCollisionPlayerAndItem()
{
	Player* player = nullptr;
	std::vector<Item*> items;

	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (actor->IsTypeOf<Item>())
		{
			items.emplace_back(actor->As<Item>());
			continue;
		}
	}

	if (items.size() == 0 || !player)
	{
		return;
	}

	for (Item* const item : items)
	{
		if (item->TestIntersect(player))
		{
			player->ConsumeItem(item);
			item->Destroy();
		}
	}
}

void GameLevel::ProcessCollisionObstacleAndOther()
{
	Obstacle* obstacle = nullptr;
	std::vector<Actor*> obstacleBody;
	std::vector<Actor*> bullets;
	std::vector<Actor*> enemies;

	Player* player = nullptr;

	// todo: 이 부분은 모든 액터를 찾지 말고
	// 같은 액터만 보도록 수정을 시도한다.
	// 변경 전 후 실제 성능이 차이나는지 체크해본다.
	for (Actor* const actor : actors)
	{
		// todo:: bullet 클래스 추상화하기
		if (actor->IsTypeOf<EnemyBullet>())
		{
			bullets.emplace_back(actor);
			continue;
		}

		if (actor->IsTypeOf<PlayerBullet>())
		{
			bullets.emplace_back(actor);
			continue;
		}

		if (actor->IsTypeOf<SmallEnemy>())
		{
			enemies.emplace_back(actor);
			continue;
		}

		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}

		if (!obstacle && actor->IsTypeOf<Obstacle>())
		{
			obstacle = actor->As<Obstacle>();
			obstacleBody = obstacle->GetBody();
			continue;
		}
	}

	if (obstacleBody.size() == 0)
	{
		return;
	}


	// 충돌판정.
	for (Actor* const obstaclePixel : obstacleBody)
	{
		for (Actor* const bullet : bullets)
		{
			if (bullet->TestIntersect(obstaclePixel))
			{
				bullet->Destroy();
			}
		}

		for (Actor* const enemy : enemies)
		{
			if (enemy->TestIntersect(obstaclePixel))
			{
				enemy->Destroy();
			}
		}

		if (player->TestIntersect(obstaclePixel))
		{
			Vector2 currentDirection = player->GetCurrentDirection();
			player->SetPosition(Vector2(
				player->GetPosition().x + (-currentDirection.x),
				player->GetPosition().y + (-currentDirection.y)
			));
		}
	}
}

void GameLevel::ShowScore()
{
	sprintf_s(scoreString, 128, "Score: %d    Health: %d", score, health);
	Renderer::Get().Submit(
		scoreString,
		Vector2(0, 0)
	);
}

void GameLevel::ShowUltraEnemy()
{
	if (score < targetScoreForShowingUltraEnemy) return;
	if (hasShownUltraEnemy) return;

	hasShownUltraEnemy = true;
	AddNewActor(new UltraEnemy(ultraEnemyBuffer.str().c_str()));

	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<EnemySpawner>())
		{
			actor->As<EnemySpawner>()->StopSpawn();
			break;
		}
	}
}

void GameLevel::ClearAllActors()
{
	for (Actor* const actor : actors)
	{
		actor->Destroy();
	}
}

