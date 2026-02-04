#pragma once
#include "Common/Common.h"
#include <Windows.h>

namespace Wanted
{
	class WANTED_API Vector2
	{
	public:
		Vector2();
		Vector2(int x, int y);
		~Vector2();

		const char* ToString();

		// 연산자 오버로딩
		Vector2 operator+(const Vector2 other) const;
		Vector2 operator-(const Vector2 other) const;

		bool operator==(const Vector2 other) const;
		bool operator!=(const Vector2 other) const;

		operator COORD() const;

		static Vector2 Zero;
		static Vector2 One;
		static Vector2 Up;
		static Vector2 Right;
	public:
		// x,y 좌표
		int x = 0;
		int y = 0;

	private:
		// 벡터 값을 문자열로 변환할 때 사용
		char* string = nullptr;
	};

}
