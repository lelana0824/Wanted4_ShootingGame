#pragma once
#include "Common/Common.h"

namespace Wanted
{
	class WANTED_API Input
	{
		friend class Engine;

		// data.
		// 키다운, 업, 키
		struct KeyState
		{
			// 현재 키가 눌렸는지
			bool isKeyDown = false;

			// 이전에 키가 눌렸는지
			bool wasKeyDown = false;
		};
	private:
		Input();
		~Input();

	public:

		// 입력 확인 함수

		// 이전 입력 x, 현재 입력 o
		bool GetKeyDown(int keyCode);
		// 이전 입력 o, 현재 입력 x
		bool GetKeyUp(int keyCode);
		// 눌려있음
		bool GetKey(int keyCode);

		static Input& Get();
	private:
		void ProcessInput();

		// 기존 입력 저장 함수
		void SavePreviousInputStates();

	private:
		// 키 상태 저장용 배열.
		KeyState keyStates[255] = {};

		// 전역적으로 접근하도록 만들기 위한 전역 변수
		static Input* instance;
	};
}