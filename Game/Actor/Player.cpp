#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Actor/PlayerBullet.h"
#include "Level/Level.h"
#include "Render/Renderer.h"

Player::Player()
	: super("<-=A=->", Vector2::Zero, Color::Red)
	, fireMode(FireMode::OneShot)
{
	int xPosition = (Engine::Get().GetWidth() / 2) - (width / 2);
	int yPosition = Engine::Get().GetHeight() - 3;
	
	this->SetPosition(Vector2(xPosition, yPosition));

	timer.SetTargetTime(0.2f);
}

Player::~Player()
{
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	// 경과 시간 업데이트
	timer.Tick(deltaTime);


	// 입력 및 이동처리
	if (Input::Get().GetKey(VK_LEFT))
	{
		MoveLeft();
	}
	if (Input::Get().GetKey(VK_RIGHT))
	{
		MoveRight();
	}

	if (fireMode == FireMode::OneShot)
	{
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			Fire();
		}

	}
	else if (fireMode == FireMode::Repeat)
	{
		if (Input::Get().GetKey(VK_SPACE))
		{
			FireInterval();
		}
	}

	// 발사 모드 전환
	if (Input::Get().GetKeyDown('R'))
	{
		fireMode = static_cast<FireMode>(
			1 - static_cast<int>(fireMode)
		);
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

void Player::Fire()
{
	timer.Reset();

	// 위치 설정
	Vector2 bulletPosition(
		position.x + (width / 2),
		position.y
	);

	GetOwner()->AddNewActor(new PlayerBullet(bulletPosition));
}

void Player::FireInterval()
{
	if (!CanShoot()) return;

	Fire();
}

bool Player::CanShoot() const
{
	return timer.IsTimeOut();
}
