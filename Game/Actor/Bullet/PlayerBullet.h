#pragma once
#include "Bullet.h"

using namespace Wanted;

class PlayerBullet : public Bullet
{
	RTTI_DECLARATIONS(PlayerBullet, Bullet)


public:
	PlayerBullet(
		const Vector2& position, 
		const Vector2& sideDirection = Vector2::Zero
	);
	~PlayerBullet();

	virtual void Tick(float deltatime) override;

private:

};

