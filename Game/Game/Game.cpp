#include "Game.h"
#include "Level/GameLevel.h"
#include "Level/TitleLevel.h"

#include <iostream>

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	// 두 레벨 생성 및 배열에 추가.
	levels.emplace_back(new TitleLevel());
	levels.emplace_back(new GameLevel());

	// 시작 상태 설정
	state = State::GamePlay;
	mainLevel = levels[0];
}

Game::~Game()
{
	// 중복 제거 방지.
	mainLevel = nullptr;

	// 모든 레벨 삭제.
	for (Level*& level : levels)
	{
		delete level;
		level = nullptr;
	}

	levels.clear();
}

void Game::ToggleMenu()
{
	// 화면 지우기.
	system("cls");

	// 변경할 인덱스 계산.
	// 현재 활성 레벨 인덱스가 1이면 -> 0으로
	// 현재 활성 레벨 인덱스가 0이면 -> 1로
	// 마법의 공식 -> (1 - x) -> OneMinus;
	int stateIndex = (int)state; //static_cast<int>;
	int nextState = 1 - stateIndex;

	state = (State)nextState;
	mainLevel = levels[nextState];
}

Game& Game::Get()
{
	if (!instance)
	{
		std::cerr << "Game::Get() - instance is null\n";
		__debugbreak();
	}

	return *instance;
}
