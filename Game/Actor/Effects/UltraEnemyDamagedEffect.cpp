#include "UltraEnemyDamagedEffect.h"
#include "Actor/Enemy/Enemy.h"
#include "Render/Renderer.h"

UltraEnemyDamagedEffect::UltraEnemyDamagedEffect(const std::vector<Enemy*>& parts, float duration)
	: lifeTimer(duration)
{
	// 보스 부위들의 현재 위치 정보를 복사 (보스가 움직여도 이펙트는 그 자리에 남게 하거나 따라가게 선택 가능)
	// 여기서는 보스를 따라다니도록 부위들의 포인터를 직접 쓰지 않고 상대적 오프셋을 쓰거나 
	// 단순하게 현재 시점의 위치들만 저장합니다.
	for (Enemy* part : parts)
	{
		if (part) targetPositions.push_back(part->GetPosition());
	}
	
	sortingOrder = 15; // 보스보다 위로
}

void UltraEnemyDamagedEffect::Tick(float deltaTime)
{
	lifeTimer.Tick(deltaTime);
	if (lifeTimer.IsTimeOut())
	{
		Destroy();
	}
}

void UltraEnemyDamagedEffect::Draw()
{
	// 액터 1개가 루프를 돌며 수백 개의 픽셀을 한 번에 Renderer에 제출
	// 엔진 입장에서 액터는 1개이므로 부하가 매우 적음
	for (const Vector2& pos : targetPositions)
	{
		Renderer::Get().Submit("*", pos, effectColor, sortingOrder);
	}
}
