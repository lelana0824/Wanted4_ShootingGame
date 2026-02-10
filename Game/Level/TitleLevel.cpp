#include "TitleLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include <sstream>

#include <iostream>

TitleLevel::TitleLevel()
{
	// 메뉴 아이템 생성.
	items.emplace_back(
		new MenuItem(
			"Resume Game",
			[]()
			{
				Game::Get().ToggleMenu();
			}
		)
	);
	items.emplace_back(
		new MenuItem(
			"Quit Game",
			[]()
			{
				Game::Get().QuitEngine();
			}
		)
	);
}

TitleLevel::~TitleLevel()
{
	for (MenuItem*& item : items)
	{
		delete item;
		item = nullptr;
	}

	items.clear();
}

void TitleLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	static int length = static_cast<int>(items.size());

	// 방향키 입력처리 (위/아래/엔터,ESC)
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 돌리기.
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		items[currentIndex]->onSelected();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ToggleMenu();

		// 인덱스 초기화
		currentIndex = 0;
	}
}

/*
 _____ _            ____        _
|_   _| |__   ___  |  _ \ _   _| |_ _   _
  | | | '_ \ / _ \ | | | | | | | __| | | |
  | | | | | |  __/ | |_| | |_| | |_| |_| |
  |_| |_| |_|\___| |____/ \__,_|\__|\__, |
									|___/
*/
void TitleLevel::Draw()
{
	super::Draw();

	Renderer::Get().Submit(
		"####################################################\n"
		"####################################################\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##    _____ _            ____        _            ##\n"
		"##   |_   _| |__   ___  |  _ \\ _   _| |_ _   _    ##\n"
		"##     | | | '_ \\ / _ \\ | | | | | | | __| | | |   ##\n"
		"##     | | | | | |  __/ | |_| | |_| | |_| |_| |   ##\n"
		"##     |_| |_| |_|\\___| |____/ \\__,_|\\__|\\__, |   ##\n"
		"##                                        |___/   ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                  Press to play                 ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"##                                                ##\n"
		"####################################################\n"
		"####################################################\n",
		Vector2::Zero, 
		Color::Blue
	);

	// 메뉴 아이템 출력
	for (int i = 0; i < static_cast<int>(items.size()); ++i)
	{
		// 아이템 색상 확인
		Color textColor = (i == currentIndex)
			? selectedColor : unselectedColor;
	}
}
