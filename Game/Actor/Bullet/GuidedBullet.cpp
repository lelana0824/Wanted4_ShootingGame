#include "GuidedBullet.h"
#include "EnemyBullet.h"
#include "Level/Level.h"
#include "Util/Util.h"
#include "Engine/Engine.h"

GuidedBullet::GuidedBullet(
	const Vector2& position,
	Player* target,
	float moveSpeed,
	Color color)
	:super(position, moveSpeed, color), target(target)
{
	startNode = new Node(position.x, position.y);
	
	// 타겟 위치를 받아야 함.
	goalNode = new Node(target->GetPosition().x, 
		target->GetPosition().y);

	
}

GuidedBullet::~GuidedBullet()
{
	
}

void GuidedBullet::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!findedPath)
	{
		auto& grid = GetOwner()->Grid();
		path = aStar.FindPath(startNode, goalNode, grid);

		if (path.size() > 0)
		{
			// startNode가 맨 처음 있으므로 그건 빼줘야함.
			path.erase(path.begin() + 0); 

			nextPosition = path[0]->position;
			path.erase(path.begin() + 0);
		}
		findedPath = true;
	}

	if (nextPosition == Vector2::Zero)
	{
		// 충돌 처리 여부 확인.
		// 얘는 또 여기있네. 근데 굳이 트리로 계산할 필요는 없다.

		if (TestIntersect(target->As<Actor>()))
		{
			target->SetHealth(target->GetHealth() - 1);
			Destroy();
			return;
		}

		Destroy();
		return;
	}

	if (position == nextPosition)
	{
		if (path.size() > 0) 
		{
			nextPosition = path[0]->position;
			path.erase(path.begin() + 0);
		}
		else
		{
			nextPosition = Vector2::Zero;
		}
	}
	else
	{
		// 다르면 nextPosition을 향해 이동해야함.

		// A* 알고리즘 적용

		xPosition = nextPosition.x + moveSpeed * deltaTime;
		yPosition = nextPosition.y + moveSpeed * deltaTime;

		if (yPosition >= Engine::Get().GetHeight()
			|| xPosition >= Engine::Get().GetWidth())
		{
			Destroy();
			return;
		}

		SetPosition(
			Vector2(
				static_cast<int>(xPosition),
				static_cast<int>(yPosition)
			)
		);
	}
}