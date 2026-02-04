#include "Input.h"
#include <Windows.h>
#include <iostream>

namespace Wanted
{
	// 전역 변수 초기화.
	Input* Input::instance = nullptr;

	Input::Input()
	{
		if (instance) {
			// 엔진이 만들어질때만 보장되면
			// 1개가 보장됨.
			// 원래 싱글턴은 1개를 보장해야 함.
		}
		
		instance = this;
	}
	Input::~Input()
	{
	}

	bool Input::GetKeyDown(int keyCode)
	{
		return keyStates[keyCode].isKeyDown &&
			!keyStates[keyCode].wasKeyDown;
	}
	bool Input::GetKeyUp(int keyCode)
	{
		return !keyStates[keyCode].isKeyDown &&
			keyStates[keyCode].wasKeyDown;;
	}

	bool Input::GetKey(int keyCode)
	{
		return keyStates[keyCode].isKeyDown;
	}
	Input& Input::Get()
	{
		// 싱글턴(Singleton). 프로젝트에서 객체가 1개만 존재
		// 이 함수는 콘텐츠 프로젝트에서 접근함
		// 따라서 엔진은 이미 초기화 완료 상태.
		if (!instance)
		{
			std::cout << "Error: Input::Get(). instance is null\n";
			__debugbreak();
		}

		// Lazy-pattern.
		// 이펙티브 cpp에 나옴. 엔진에서 안씀.

		return *instance; // 포인터가 가르키는 값을 지정해주면 참조로 넘어감
	}
	void Input::ProcessInput()
	{
		// 키 마다의 입력 읽기.
		// 운영체제가 제공하는 기능을 사용할수 밖에 없음.
		for (int i = 0; i < 255; i++)
		{
			keyStates[i].isKeyDown
				= (GetAsyncKeyState(i) & 0x8000) > 0 ? true : false;
		}
	}
	void Input::SavePreviousInputStates()
	{
		// 현재 입력 값을 이전 입력 값으로 저장
		// 키 마다의 입력 읽기.
		// 운영체제가 제공하는 기능을 사용할수 밖에 없음.
		for (int i = 0; i < 255; i++)
		{
			keyStates[i].wasKeyDown = keyStates[i].isKeyDown;
		}
	}
}
