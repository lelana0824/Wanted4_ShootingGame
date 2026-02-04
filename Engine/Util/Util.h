#pragma once

#include "Math/Vector2.h"
#include "Math/Color.h"
#include <time.h>

using namespace Wanted;

namespace Util
{
	// 콘솔 커서 위치 이동(설정)하는 함수.
	inline void SetConsolePosition(const Vector2& position)
	{
		SetConsoleCursorPosition(
			GetStdHandle(STD_OUTPUT_HANDLE),
			static_cast<COORD>(position)
		);
	}

	// 콘솔 텍스트 설정 함수.
	inline void SetConsoleTextColor(Color color)
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			static_cast<unsigned short>(color)
		);
	}

	// 커서 끄기
	inline void TurnOffCursor()
	{
		CONSOLE_CURSOR_INFO info = {};

		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = false;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}


	// 커서 켜기
	inline void TurnOnCursor()
	{
		CONSOLE_CURSOR_INFO info = {};

		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = true;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}

	inline void SetRandomSeed()
	{
		srand(static_cast<unsigned int>(time(nullptr)));
	}

	inline int Random(int min, int max)
	{
		int diff = (max - min) + 1;
		return ((diff * rand()) / (RAND_MAX + 1)) + min;
	}

	inline float RandomRange(float min, float max)
	{
		// 0~1 사이의 float 기반 랜덤 값.

		float random = static_cast<float>(rand())
			/ static_cast<float>(RAND_MAX);

		float diff = (max - min);
		return (random * diff) + min;
	}
}

template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

template<typename T>
void SafeDeleteArray(T*& t)
{
	if (t)
	{
		delete[] t;
		t = nullptr;
	}
}

