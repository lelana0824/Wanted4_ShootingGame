#pragma once
#include "Engine/Engine.h"

#include <vector>

using namespace Wanted;

enum class State
{
	GamePlay = 0,
	Menu = 1,
	Length,
};

class Game : public Engine
{
public:
	Game();
	virtual ~Game();

	// 메뉴/게임 레벨을 전환하는 함수
	void ToggleMenu();

	static Game& Get();
private:
	// 게임에서 관리하는 레벨을 저장할 배열.
	std::vector<Level*> levels;
	State state = State::GamePlay;

	static Game* instance;
};

