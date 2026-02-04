#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"

#include <iostream>
#include <Windows.h>

namespace Wanted
{
	Actor::Actor(
		const char* image, 
		const Vector2& position, Color color)
		:position(position),
		color(color)
	{
		// 문자열 동적 할당
		width = static_cast<int>(strlen(image));
		this->image = new char[width + 1];
		strcpy_s(this->image, width + 1, image);
	}

	Actor::~Actor()
	{
		SafeDeleteArray(image);
	}

	void Actor::BeginPlay()
	{
		hasBeganPlay = true;
	}
	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::Draw()
	{
		//Renderer::Draw(position, color, image);
		
		// 렌더러에 데이터 제출.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 변경하려는 위치가 현재 위치와 같으면 건너뜀.
		if (position == newPosition)
		{
			return;
		}

		// 새로운 위치 설정
		position = newPosition;
	}
}

