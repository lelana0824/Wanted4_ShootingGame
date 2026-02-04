#pragma once
#include "Math/Vector2.h"
#include <Windows.h>

namespace Wanted
{
	/*
		더블 버퍼링에 사용할 Console Output 핸들을 관리하는 클래스.
	*/
	class ScreenBuffer
	{
	public:
		ScreenBuffer(const Vector2& screenSize);
		~ScreenBuffer();

		void Clear();

		void Draw(CHAR_INFO* charInfo);

		inline HANDLE GetBuffer() const { return buffer;  }

	private:
		// 콘솔 출력 핸들
		HANDLE buffer = nullptr;

		// 화면 크기
		Vector2 screenSize;
	};

}

