#include "UltraEnemy.h"
#include "Level/Level.h"
#include "Actor/Enemy/Enemy.h"
#include "Actor/Enemy/SmallEnemy.h"
#include "Actor/Bullet/EnemyBullet.h"
#include "Util/Util.h"
#include "Engine/Engine.h"
#include "Actor/Effects/EnemyDestroyEffect.h"
#include "Actor/Effects/DamagedEffect.h"

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
    canHitOtherActorTimer.SetTargetTime(3.0f);

    std::stringstream ss(enemieChars);
    std::string line;

    int yPosition = -5;

    while (std::getline(ss, line)) {
        if (line.empty()) {
            continue;
        }

        // 화면 중앙에서 나타나길 원함
        int defaultXPosition = (Engine::Get().GetWidth() / 2) - (line.size() / 2);
        
        for (int x = 0; x < line.size(); ++x) {
            char token = line[x];

            if (token == ' ') continue;

            std::string tokenStr(1, token);
            int newXPosition = defaultXPosition + x;

            Enemy* enemy = new Enemy(
                tokenStr.c_str(),
                Vector2(newXPosition, yPosition),
                Color::Blue
            );

            enemy->SetXPosition(static_cast<float>(newXPosition));
            enemy->SetYPosition(yPosition);
            enemies.emplace_back(enemy);

            // 특정 토큰의 경우는 bulletSpanwer로 지정해 둠.
            if (token == '@') {
                enemy->GetTimer().SetTargetTime(0.5f);
                bulletSpawners.emplace_back(enemy);
            }
        }

        yPosition++;
    }
}

UltraEnemy::~UltraEnemy()
{
    if (IsDead()) return;

    for (Enemy* enemy : enemies)
    {
        SafeDelete(enemy);
    }
    enemies.clear(); 
    bulletSpawners.clear();
}

void UltraEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);
    timer.Tick(deltaTime);
    moveDirectionChangeTimer.Tick(deltaTime);
    canHitOtherActorTimer.Tick(deltaTime);

    CreateBody();
    Move(deltaTime);
    Shot(deltaTime);
}

void UltraEnemy::OnDamaged()
{
    // 모습 완전히 드러내기 전까지는 데미지 안들어감.
    if (!hasAllBodyShown())
    {
        return;
    }

    health -= 1;


    if (health <= 0) {
        for (Enemy* const enemy : enemies)
        {
            enemy->Destroy();

            GetOwner()->AddNewActor(new EnemyDestroyEffect(enemy->GetPosition()));
        }

        Destroy();
    }
    else
    {
        for (Enemy* const enemy : enemies)
        {
            GetOwner()->AddNewActor(
                new DamagedEffect(
                    enemy->GetPosition(),
                    Vector2(
                        direction == MoveDirection::Left ? -1 : 1,
                        0
                    ),
                    moveSpeed
                )
            );
        }
    }

}

bool UltraEnemy::GetCanHitOtherActor()
{
    if (canHitOtherActorTimer.IsTimeOut())
    {
        canHitOtherActorTimer.Reset();
        return true;
    }

    return false;
}


void UltraEnemy::CreateBody()
{
    // 최초 1회만 생성

    if (!isActive)
    {
        for (Enemy* const enemy : enemies)
        {
            GetOwner()->AddNewActor(enemy);
        }
    }
    
    isActive = true;
}

void UltraEnemy::Move(float deltaTime)
{
    // 특정 시간마다 한번씩 방향을 바꿔줌. (현재는 좌우기반)
    if (moveDirectionChangeTimer.IsTimeOut()) {
        direction = static_cast<MoveDirection>(1 - static_cast<int>(direction));
        moveDirectionChangeTimer.Reset();
    }

    for (Enemy* const enemy : enemies)
    {
        enemy->Tick(deltaTime);

        // 최초 등장시에는 타이머 시간만큼 안보이는 곳에서 내려와야 함.
        if (!hasAllBodyShown()) {
            enemy->SetYPosition(enemy->GetYPosition() + (moveSpeed / 8) * 1 * deltaTime);

            enemy->SetPosition(
                Vector2(
                    enemy->GetPosition().x,
                    static_cast<int>(enemy->GetYPosition())
                )
            );

            continue;
        }

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
    if (!hasAllBodyShown())
    {
        return;
    }

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
                10.0f,
                Color::Blue
            )
        );
    }
}
