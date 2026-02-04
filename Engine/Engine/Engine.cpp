#include "Engine.h"
#include "Level/Level.h"
#include "Core/Input.h"
#include "Util/Util.h"
#include "Render/Renderer.h"

#include <iostream>
#include <Windows.h>

namespace Wanted
{
	// 전역 변수 초기화
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		instance = this;

		// 입력 관리자 생성
		input = new Input();

		LoadSetting();

		renderer = new Renderer(Vector2(setting.width, setting.height));

		Util::SetConsoleTextColor(Color::Green);
		Util::TurnOffCursor();
	}

	Engine::~Engine()
	{
		if (mainLevel)
		{
			delete mainLevel;
			mainLevel = nullptr;
		}

		if (input)
		{
			delete input;
			input = nullptr;
		}

		if (renderer)
		{
			SafeDelete(renderer);
		}
	}

	void Engine::Run()
	{
		// 시계 정밀도
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		__int64 currentTime = 0;
		__int64 previousTime = 0;

		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);

		currentTime = time.QuadPart;
		previousTime = currentTime;

		// 기준 프레임 (단위초)
		//float targetFrameRate = 120.0f;
		setting.framerate
			= setting.framerate == 0.0f ? 60.0f : setting.framerate;

		float oneFrameTime = 1.0f / setting.framerate;

		while (!isQuit)
		{
			QueryPerformanceCounter(&time);
			currentTime = time.QuadPart;
			
			
			// cal frame;
			float deltaTime =
				static_cast<float>(currentTime - previousTime);
			deltaTime = deltaTime / static_cast<float>(frequency.QuadPart);

			// 고정프레임기법
			if (deltaTime >= oneFrameTime)
			{
				input->ProcessInput();

				// 프레임처리
				BeginPlay();
				Tick(deltaTime);
				Draw();

				// 이전 시간 값 갱신.
				previousTime = currentTime;

				input->SavePreviousInputStates();

				// 레벨에 요청된 추가제거 처리
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();
				}
			}
			
		}

		Shutdown();
	}

	void Engine::QuitEngine()
	{
		isQuit = true;
	}

	void Engine::SetNewLevel(Level* newLevel)
	{
		// todo: 임시 코드. 레벨 전환 시 바로 제거 x
		if (mainLevel) {
			delete mainLevel;
			mainLevel = nullptr;
		}

		// set level
		mainLevel = newLevel;
	}

	Engine& Engine::Get()
	{
		if (!instance)
		{
			std::cout << "Error: Engine:Get(). instance is null\n";
			__debugbreak();
		}
		return *instance;
	}

	void Engine::LoadSetting()
	{
		FILE* file = nullptr;
		fopen_s(&file, "../Config/Setting.txt", "rt");
		if (!file)
		{
			std::cout << "Failed to open engine setting file.\n";
			__debugbreak();
			return;
		}

		char buffer[2048] = {};

		size_t readSize = fread(buffer, sizeof(char), 2048, file);

		// 문자열 자르기 (파싱)
		// 첫번째 문자열 분리할때는 첫 파라미터 전달.
		char* context = nullptr;
		char* token = nullptr;
		token = strtok_s(buffer, "\n", &context);

		// 반복해서 자르기
		while (token)
		{
			// 설정 텍스트에서 파라미터 이름만 읽기
			char header[10] = {};

			// 이떄 "%s"로 읽으면 스페이스가 있으면 거기까지 읽음
			sscanf_s(token, "%s", header, 10);

			// 문자열 비교 및 값 읽기
			if (strcmp(header, "framerate") == 0)
			{
				sscanf_s(token, "framerate = %f", &setting.framerate);
			}
			else if (strcmp(header, "width") == 0)
			{
				sscanf_s(token, "width = %d", &setting.width);
			}
			else if (strcmp(header, "height") == 0)
			{
				sscanf_s(token, "height = %d", &setting.height);
			}

			// 개행 문자로 문자열 분리
			token = strtok_s(nullptr, "\n", &context);
		}

		fclose(file);
	}

	void Engine::BeginPlay()
	{
		// 레벨이 있으면 이벤트 전달
		if (!mainLevel) {
			std::cout << "mainLevel is empty. \n";
			return;
		}

		mainLevel->BeginPlay();
	}

	void Engine::Tick(float deltaTime)
	{
		//std::cout << "DeltaTime: " << deltaTime
		//	<< ", FPS: " << (1.0f / deltaTime) << "\n";

		// event to level

		if (!mainLevel)
		{
			std::cout << "Error: Engine::Tick(). mainLevel is empty\n";
			return;
		}

		mainLevel->Tick(deltaTime);
	}

	void Engine::Draw()
	{
		if (!mainLevel)
		{
			std::cout << "Error: Engine::Draw(). mainLevel is empty\n";
			return;
		}

		// 레벨의 모든 액터가 렌더 데이터를 제출.
		mainLevel->Draw();

		// 렌더러에 그리기 명령 전달.
		renderer->Draw();
	}
	void Engine::Shutdown()
	{
		std::cout << "Engine has been shutdown!!\n";
		Util::TurnOnCursor();
	}
}