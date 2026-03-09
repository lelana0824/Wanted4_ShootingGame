#include "GameLevel.h"

#include "Actor/Enemy/Enemy.h"
#include "Actor/Bullet/PlayerBullet.h"
#include "Actor/Bullet/EnemyBullet.h"
#include "Actor/Enemy/EnemySpawner.h"
#include "Actor/Enemy/SmallEnemy.h"
#include "Actor/Enemy/UltraEnemy.h"
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

	grid.assign(
		Engine::Get().GetWidth(),
		std::vector<int>(Engine::Get().GetHeight(), 0)
	);
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
	if (player && player->GetIsPlayerDead())
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

std::vector<std::vector<int>>& GameLevel::Grid()
{
	

	std::vector<Actor*> obstacleBody;
	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<Obstacle>())
		{

			obstacleBody = actor->
				As<Obstacle>()->GetBody();
			continue;
		}
	}

	for (Actor* obstacle : obstacleBody)
	{
		Vector2 position = obstacle->GetPosition();
		grid[position.y][position.x] = 1;
	}

	return grid;
}

void GameLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// 플레이어 탄약과 적 액터 필터링.
	std::vector<Actor*> bullets;

	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<PlayerBullet>())
		{
			bullets.emplace_back(actor);
			continue;
		}
	}

	// 판정 안해도 되는지 확인.
	if (bullets.size() == 0)
	{
		return;
	}

	// 충돌판정
	// Enemy와 총알 수 늘려서 개선전과 개선 후 비교하기.
	for (Actor* const bullet : bullets)
	{
		Vector2 bulletPosition = bullet->GetPosition();
		
		std::vector<Actor*> list = Query(
			Bounds(bulletPosition.x, bulletPosition.y));

		for (Actor* const enemy : list)
		{
			if (enemy->IsTypeOf<SmallEnemy>()) {
				if (bullet->TestIntersect(enemy))
				{
					enemy->As<SmallEnemy>()->OnDamaged();
					bullet->Destroy();

					score += 1;
					continue;
				}
			}
			// 대형 유닛의 일부와 AABB 겹침 판정.
			// SmallEnemy가 아닌 Enemy는 현재로써는 
			// 대형 유닛의 body로 판단.
			else if (enemy->IsTypeOf<Enemy>())
			{
				if (!ultraEnemy || !ultraEnemy->hasAllBodyShown())
				{
					continue;
				}
				if (bullet->TestIntersect(enemy)) 
				{
					ultraEnemy->OnDamaged();
					bullet->Destroy();
					score += 10;

					if (ultraEnemy->IsDead())
					{
						isGameClear = true;
					}
				}
			}
		}
	}
}

void GameLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	for (Actor* const actor : actors)
	{
		if (!player && actor->IsTypeOf<Player>())
		{
			player = actor->As<Player>();
			continue;
		}
	}

	if (!player)
	{
		return;
	}

	// 충돌판정.
	Vector2 playerPosition = player->GetPosition();
	for (Actor* const bullet : 
		Query(Bounds(playerPosition.x, playerPosition.y)))
	{
		if (bullet->IsTypeOf<EnemyBullet>() && 
			bullet->TestIntersect(player))
		{
			player->SetHealth(player->GetHealth() - 1);

			if (player->GetIsPlayerDead())
			{
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
				player->SetHealth(player->GetHealth() - 1);

				if (player->GetIsPlayerDead())
				{
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
	if (!player) return;

	sprintf_s(scoreString, 128, "Score: %d    Health: %d", score, player->GetHealth());
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
	ultraEnemy = new UltraEnemy(ultraEnemyBuffer.str().c_str());
	AddNewActor(ultraEnemy);


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

