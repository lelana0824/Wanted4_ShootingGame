#pragma once
#include "Common/Common.h"

namespace Wanted
{
	class Input;
	class Level;
	class Renderer;

	// Main game engine class;
	class WANTED_API Engine
	{
		// 엔진 설정 구조체.
		struct EngineSetting
		{
			// 프레임 속도
			float framerate = 0.0f;
			
			int width = 0;
			int height = 0;
		};

	public:
		Engine();
		virtual ~Engine();

		// engine loop (game loop);
		void Run();
		void QuitEngine();
		
		// add new level;
		void SetNewLevel(class Level* newLevel);

		static Engine& Get();

		inline int GetWidth() const { return setting.width; }
		inline int GetHeight() const { return setting.height; }

	protected:
		// 설정파일 로드 함수
		void LoadSetting();

		// start game play
		void BeginPlay();

		void Tick(float deltaTime);

		void Draw();

	protected:
		// 정리 함수
		void Shutdown();

		// engine out flag;
		bool isQuit = false;

		// 엔진 설정 값.
		EngineSetting setting;

		// 입력 관리자
		Input* input = nullptr;
		
		// 렌더러 객체
		Renderer* renderer = nullptr;

		// main level
		class Level* mainLevel = nullptr;

		// 전환할 레벨을 임시 저장하는 변수.
		class Level* nextLevel = nullptr;

		static Engine* instance;
		
	};
}


