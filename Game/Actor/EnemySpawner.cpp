#include "EnemySpawner.h"
#include "Actor/SmallEnemy.h"
#include "Util/Util.h"
#include "Level/Level.h"
#include "Engine/Engine.h"

// 적 생성할 때 사용할 글자 값.
// 여기에서 static은 private.
static const char* enemyType[] =
{
	";:^:;",
	"zZwZz",
	"oO@Oo",
	"<-=->",
	")qOp(",
};

EnemySpawner::EnemySpawner()
{
	// 적 생성 타이머 설정.
	timer.SetTargetTime(Util::RandomRange(2.0, 3.0));
}

void EnemySpawner::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!isStopSpawn) {
		SpawnEnemy(deltaTime);
	}
}

void EnemySpawner::SpawnEnemy(float deltaTime)
{
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	timer.Reset();

	// 적 생성
	// 적 개수 파악.
	static int length = sizeof(enemyType) / sizeof(enemyType[0]);

	int index = Util::Random(0, length - 1);

	int yPosition = Util::Random(1, 10);
	int enemyCount = Util::Random(3, 6);

	float newXPosition = Util::RandomRange(3, Engine::Get().GetWidth() - 2);
	float defaultMoveSpeed = Util::RandomRange(5.0f, 10.0f);
	SmallEnemy::MoveDirection direction = static_cast<SmallEnemy::MoveDirection>(Util::Random(0, 1));

	for (int i = 0; i < enemyCount; i++)
	{
		SmallEnemy* enemy = new SmallEnemy(
			enemyType[index],
			yPosition++,
			direction
		);
		enemy->SetMoveSpeed(defaultMoveSpeed + (i * 2));
		enemy->SetXPosition(newXPosition);

		GetOwner()->AddNewActor(enemy);
	}

}
