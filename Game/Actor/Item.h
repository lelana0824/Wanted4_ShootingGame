#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class Item : public Actor
{
	RTTI_DECLARATIONS(Item, Actor)

public:
	enum class FireDirectionType
	{
		None = -1,
		Top,
		Side,
		All
	};

	Item(Vector2& position = Vector2::Zero,
		int bulletCount = 1,
		FireDirectionType fireDirection = FireDirectionType::Top
	);
	inline int GetBulletCount() { return bulletCount; }
	inline FireDirectionType GetFireDirectionType() { return fireDirection; }

private:
	int bulletCount = 1;
	FireDirectionType fireDirection = FireDirectionType::Top;
};

