#include "SmallEnemy.h"
#include "Level/Level.h"
#include "Actor/EnemyDestroyEffect.h"
#include "Engine/Engine.h"
#include "Actor/EnemyBullet.h"

SmallEnemy::SmallEnemy(const char* image, int yPosition)
	:super(image, yPosition)
{
	// 랜덤 (오른쪽 또는 왼쪽으로 이동할지 결정)
	int random = Util::Random(1, 10);

	if (random % 2 == 0)
	{
		direction = MoveDirection::Left;
		xPosition = static_cast<float>(
			Engine::Get().GetWidth() - width - 1
			);
	}
	else
	{
		direction = MoveDirection::Right;
		xPosition = 0;
	}

	// 이동 방향에 따른 위치 설정.
	SetPosition(
		Vector2(static_cast<int>(xPosition), yPosition)
	);
	// 발사 타이머 목표 시간 설정.
	timer.SetTargetTime(Util::RandomRange(1.0f, 3.0f));
}

void SmallEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);


	// 이동 처리
	float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;

	xPosition = xPosition + moveSpeed * dir * deltaTime;

	if (xPosition + width < 0)
	{
		Destroy();
		return;
	}

	if (xPosition >= Engine::Get().GetWidth())
	{
		Destroy();
		return;
	}

	SetPosition(
		Vector2(static_cast<int>(xPosition), position.y)
	);


	// 발사 타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 타이머 리셋
	timer.Reset();

	// 탄약 발사
	GetOwner()->AddNewActor(
		new EnemyBullet(
			Vector2(position.x + width / 2, position.y),
			Util::RandomRange(10.0f, 20.0f)
		)
	);
}
void SmallEnemy::OnDamaged()
{
	Destroy();

	GetOwner()->AddNewActor(new EnemyDestroyEffect(position));
}
