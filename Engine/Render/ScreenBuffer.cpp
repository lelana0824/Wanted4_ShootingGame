#include "ScreenBuffer.h"
#include "Renderer.h"
#include <iostream>

namespace Wanted
{

	ScreenBuffer::ScreenBuffer(const Vector2& screenSize)
		:screenSize(screenSize)
	{
		buffer = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr,
			CONSOLE_TEXTMODE_BUFFER,
			nullptr
		);

		if (buffer == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(
				nullptr,
				"ScreenBuffer - Failed to create buffer.",
				"Buffer creation error",
				MB_OK
			);
			__debugbreak();
		}

		// 1. 폰트 강제 설정 (이전 단계에서 적용한 코드 유지)
		CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 16; // 폰트 크기를 줄여서 공간 확보 시도
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(buffer, FALSE, &cfi);

		// [정석 추가] 2. 하드웨어 한계 검사 (이게 핵심입니다)
		// 현재 폰트와 모니터 해상도에서 띄울 수 있는 최대 콘솔 크기를 구함
		COORD maxWindow = GetLargestConsoleWindowSize(buffer);

		if (screenSize.x > maxWindow.X || screenSize.y > maxWindow.Y)
		{
			std::cerr << "Error: Screen size too large for this monitor.\n";
			std::cerr << "Requested: " << screenSize.x << "x" << screenSize.y << "\n";
			std::cerr << "Maximum Allowed: " << maxWindow.X << "x" << maxWindow.Y << "\n";
			// 여기서 멈춰야 원인을 알 수 있음
			__debugbreak();
		}


		// STEP 1. 윈도우 크기를 최소(1x1)로 구겨 넣습니다.
	// 이 작업이 없으면 STEP 2에서 "현재 윈도우가 버퍼보다 큽니다"라며 에러가 납니다.
		SMALL_RECT minimalRect = { 0, 0, 1, 1 };
		if (!SetConsoleWindowInfo(buffer, TRUE, &minimalRect))
		{
			std::cerr << "Step 1 Failed: Minimum Window\n";
			__debugbreak();
		}

		// STEP 2. 버퍼 크기를 목표 크기(40x50)로 설정합니다.
		// 윈도우가 1x1 상태이므로, 버퍼를 40x50으로 줄이든 늘리든 충돌하지 않습니다.
		COORD newSize;
		newSize.X = static_cast<short>(screenSize.x);
		newSize.Y = static_cast<short>(screenSize.y);

		if (!SetConsoleScreenBufferSize(buffer, newSize))
		{
			std::cerr << "Step 2 Failed: Buffer Size\n";
			__debugbreak();
		}

		// STEP 3. 윈도우 크기를 목표 크기(40x50)로 확장합니다.
		// 버퍼가 이미 40x50으로 확보되었으므로 안전하게 확장이 가능합니다.
		SMALL_RECT rect;
		rect.Left = 0;
		rect.Top = 0;
		rect.Right = static_cast<short>(screenSize.x - 1);
		rect.Bottom = static_cast<short>(screenSize.y - 1);

		if (!SetConsoleWindowInfo(buffer, TRUE, &rect))
		{
			std::cerr << "Step 3 Failed: Final Window. Error: " << GetLastError() << "\n";
			__debugbreak();
		}

		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(buffer, &info);

		info.bVisible = false;
		SetConsoleCursorInfo(buffer, &info);
	}

	ScreenBuffer::~ScreenBuffer()
	{
		if (buffer)
		{
			CloseHandle(buffer);
		}
	}

	void ScreenBuffer::Clear()
	{
		DWORD writtenCount = 0;

		FillConsoleOutputCharacterA(
			buffer,
			' ',
			screenSize.x * screenSize.y,
			Vector2::Zero,
			&writtenCount
		);
	}

	void ScreenBuffer::Draw(CHAR_INFO* charInfo)
	{
		SMALL_RECT writeRegion = {};
		writeRegion.Left = 0;
		writeRegion.Top = 0;
		writeRegion.Right = static_cast<short>(screenSize.x - 1);
		writeRegion.Bottom = static_cast<short>(screenSize.y - 1);

		WriteConsoleOutputA(
			buffer,
			charInfo,
			screenSize,
			Vector2::Zero,
			&writeRegion
		);
	}

}