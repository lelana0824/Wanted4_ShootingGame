#include "DamagedEffect.h"
#include "Engine/Engine.h"


DamagedEffect::DamagedEffect(const Vector2& position, const Vector2& moveDirection, float moveSpeed)
	: super("a", position, Color::Red), moveDirection(moveDirection), moveSpeed(moveSpeed)
{
	xPosition = position.x;
	yPosition = position.y;

	int effectFrameImageLength = 1;
	float oneFrameTime = Engine::Get().GetOneFrame();


	// 애니메이션 시퀀스 개수 구하기.
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	for (DamagedEffect::EffectFrame& ani : sequence)
	{
		ani.playTime = oneFrameTime / effectSequenceCount;
	}

	// 다음 애니메이션까지 대기할 시간.
	timer.SetTargetTime(sequence[0].playTime);
	ChangeImage(sequence[0].frame);

	// 색상 설정.
	color = sequence[0].color;
}

DamagedEffect::~DamagedEffect()
{
}

void DamagedEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	timer.Tick(deltaTime);

	if (!timer.IsTimeOut())
	{
		return;
	}

	// 애니메이션 재생 끝났는지 확인.
	// 끝났으면 삭제.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// 타이머 리셋.
	timer.Reset();

	// 이펙트 프레임 업데이트.
	++currentSequenceIndex;

	// 다음 시퀀스에서 재생할 시간으로 타이머 재설정.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// 애니메이션 프레임에 사용할 문자열을 액터에 복사.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// 색상 설정.
	color = sequence[currentSequenceIndex].color;


	float dirX = moveDirection.x < 0 ? -1.0f : 1.0f;
	float dirY = moveDirection.y < 0 ? -1.0f : 1.0f;

	xPosition = xPosition + moveSpeed * dirX * deltaTime;
	yPosition = yPosition + moveSpeed * dirY * deltaTime;


	// 원 타겟을 따라 이동하기.
	SetPosition(Vector2(
		static_cast<int>(xPosition),
		position.y
	));

};