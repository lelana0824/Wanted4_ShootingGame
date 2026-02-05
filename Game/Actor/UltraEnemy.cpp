#include "UltraEnemy.h"
#include "Actor/Enemy.h"
#include "Level/Level.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>


UltraEnemy::UltraEnemy(const char* enemieChars)
    :super("", 0)
{
    std::stringstream ss(enemieChars);
    std::string line;

    // \n 기준으로 한 줄씩 읽기
    int yPosition = 1;
    int defaultXPosition = 3;

    while (std::getline(ss, line)) {
        if (line.empty()) {
            continue;
        }


        for (int i = 0; i < line.length(); i++) {
            const char* t = &line[i];

            enemies.emplace_back(
                new Enemy(
                    t,
                    Vector2(defaultXPosition + i, yPosition
                   )
                )
            );
        }

        yPosition++;
    }
}

void UltraEnemy::Tick(float deltaTime)
{
    super::Tick(deltaTime);

    for (Enemy* const enemy : enemies)
    {
        enemy->Tick(deltaTime);
        GetOwner()->AddNewActor(enemy);
    }

}

void UltraEnemy::OnDamaged()
{
}
