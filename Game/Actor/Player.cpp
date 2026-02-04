#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"

Player::Player(): super("<-=A=->", Vector2::Zero, Color::Red)
{
	int xPosition = (Engine::Get().GetWidth() / 2) - (width / 2);
	int yPosition = Engine::Get().GetHeight() - 3;
	
	this->SetPosition(Vector2(xPosition, yPosition));
}

Player::~Player()
{
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 및 이동처리
	if (Input::Get().GetKey(VK_LEFT))
	{
		MoveLeft();
	}
	if (Input::Get().GetKey(VK_RIGHT))
	{
		MoveRight();
	}
}

void Player::MoveRight()
{
	position.x += 1;

	// "<A>" 문자열 왼쪽을 좌표 기준점으로 삼음.
	// x + length - 1;
	// 문자열 길이값을 알고 마지막 기준으로 넘어갔는지 확인
	if (position.x + width > Engine::Get().GetWidth())
	{
		position.x -= 1;
	}
}

void Player::MoveLeft()
{
	position.x -= 1;

	// 좌표검사
	if (position.x < 0)
	{
		position.x = 0;
	}
}
