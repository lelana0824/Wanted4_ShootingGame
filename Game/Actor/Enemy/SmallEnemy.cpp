#include "SmallEnemy.h"
#include "Level/Level.h"
#include "Actor/Effects/EnemyDestroyEffect.h"
#include "Engine/Engine.h"
#include "Actor/Bullet/EnemyBullet.h"
#include "Actor/Bullet/GuidedBullet.h"

#include "Actor/Item.h"

SmallEnemy::SmallEnemy(
	const char* image, int yPosition, 
	MoveDirection direction
)
	:super(image, yPosition)
{
	this->direction = direction;
	this->yPosition = yPosition;


	if (direction == MoveDirection::Left)
	{
		xPosition = static_cast<float>(
			Engine::Get().GetWidth() - width - 1
		);
	}
	else
	{
		xPosition = 0;
	}

	// 이동 방향에 따른 위치 설정.
	SetPosition(
		Vector2(static_cast<int>(xPosition), yPosition)
	);
	// 발사 타이머 목표 시간 설정.
	timer.SetTargetTime(Util::RandomRange(1.0f, 3.0f));
	firstAppearedTimer.SetTargetTime(1.0f);

	// 플레이어 탐색 및 설정
	Player* player = nullptr;

	// todo: 이 부분은 모든 액터를 찾지 말고
	// 같은 액터만 보도록 수정을 시도한다.
	// 변경 전 후 실제 성능이 차이나는지 체크해본다.
	
}

void SmallEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	firstAppearedTimer.Tick(deltaTime);



	// 이동 처리
	float dir = direction == MoveDirection::Left ? -1.0f : 1.0f;
	float appereadTimeOutNumber = firstAppearedTimer.IsTimeOut() ? 0.0f : 1.0f;

	xPosition = xPosition + moveSpeed * dir * (1 - appereadTimeOutNumber) *deltaTime;
	yPosition = yPosition + moveSpeed * appereadTimeOutNumber * deltaTime;

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

	if (yPosition >= Engine::Get().GetHeight())
	{
		Destroy();
		return;
	}


	SetPosition(
		Vector2(static_cast<int>(xPosition), static_cast<int>(yPosition))
	);


	// 발사 타이머 업데이트.
	timer.Tick(deltaTime);
	if (!timer.IsTimeOut())
	{
		return;
	}

	// 타이머 리셋
	timer.Reset();
	Actor* player = 
		GetOwner()->GetActorBy<Player>();

	// 탄약 발사
	GetOwner()->AddNewActor(
		new GuidedBullet(
			Vector2(position.x + width / 2, position.y),
			player->As<Player>(),
			Util::RandomRange(10.0f, 20.0f)
		)
	);
}
void SmallEnemy::OnDamaged()
{
	Destroy();

	int targetNumber = Util::Random(0, 2);

	if (targetNumber == 0)
	{
		int bulletCount = Util::Random(1, 3);
		Item::FireDirectionType fireDirection
			= bulletCount == 1 
			? static_cast<Item::FireDirectionType>(Util::Random(0, 2))
			: Item::FireDirectionType::Top;

		GetOwner()->AddNewActor(new Item(position, bulletCount, fireDirection));
	}

	
	GetOwner()->AddNewActor(new EnemyDestroyEffect(position));
}
