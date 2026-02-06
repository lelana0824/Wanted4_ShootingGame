#include "UltraEnemy.h"
#include "Actor/Enemy.h"
#include "Level/Level.h"
#include "Actor/SmallEnemy.h"
#include "Actor/EnemyBullet.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Actor/EnemyDestroyEffect.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>


UltraEnemy::UltraEnemy(const char* enemieChars)
    :super("", 10)
{
    isActive = false;

    timer.SetTargetTime(5.0f);
    moveSpeed = 20.0f;
    direction = MoveDirection::Right;
    moveDirectionChangeTimer.SetTargetTime(3.0f);

    std::stringstream ss(enemieChars);
    std::string line;

    int yPosition = 1;

    while (std::getline(ss, line)) {
        if (line.empty()) {
            continue;
        }

        int defaultXPosition = (Engine::Get().GetWidth() / 2) - (line.size() / 2);
        
        for (int x = 0; x < line.size(); ++x) {
            char token = line[x];

            if (token == ' ') continue;

            std::string tokenStr(1, token);
            int newXPosition = defaultXPosition + x;

            Enemy* enemy = new Enemy(
                tokenStr.c_str(),
                Vector2(newXPosition, yPosition)
            );

            enemy->SetXPosition(static_cast<float>(newXPosition));
            enemies.emplace_back(enemy);

            if (token == '@') {
                enemy->GetTimer().SetTargetTime(0.5f);
                bulletSpawners.emplace_back(enemy);
            }
        }

        yPosition++;
    }

}

void UltraEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    timer.Tick(deltaTime);
    moveDirectionChangeTimer.Tick(deltaTime);

    CreateBody();
    Move(deltaTime);
    Shot(deltaTime);
}

void UltraEnemy::OnDamaged()
{
    health -= 1;

    if (health <= 0) {
        for (Enemy* const enemy : enemies)
        {
            enemy->Destroy();

            GetOwner()->AddNewActor(new EnemyDestroyEffect(enemy->GetPosition()));
        }

        Destroy();
    }

}

void UltraEnemy::CreateBody()
{
    for (Enemy* const enemy : enemies)
    {
        // 몸체 생성
        if (!isActive)
        {
            GetOwner()->AddNewActor(enemy);
        }
    }
    isActive = true;
}

void UltraEnemy::Move(float deltaTime)
{
    if (moveDirectionChangeTimer.IsTimeOut()) {
        direction = static_cast<MoveDirection>(1 - static_cast<int>(direction));
        moveDirectionChangeTimer.Reset();
    }

    for (Enemy* const enemy : enemies)
    {
        // 최초 등장시에는 타이머가 끝날때까지 그자리에서 대기.
        if (!timer.IsTimeOut()) {
            continue;
        }

        enemy->Tick(deltaTime);

        float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;

        enemy->SetXPosition(enemy->GetXPosition() + moveSpeed * dir * deltaTime);

        enemy->SetPosition(
            Vector2(
                static_cast<int>(enemy->GetXPosition()),
                enemy->GetPosition().y
            )
        );
    }
}

void UltraEnemy::Shot(float deltaTime)
{
    for (Enemy* const spawner : bulletSpawners)
    {

        // 발사 타이머 업데이트.
        spawner->GetTimer().Tick(deltaTime);
        if (!spawner->GetTimer().IsTimeOut())
        {
            continue;
        }

        // 타이머 리셋
        spawner->GetTimer().Reset();

        GetOwner()->AddNewActor(
            new EnemyBullet(
                Vector2(spawner->GetPosition().x, spawner->GetPosition().y + 1),
                10.0f
            )
        );
    }
}

