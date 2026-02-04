#include "GameLevel.h"
#include "Actor/Player.h"

GameLevel::GameLevel()
{
	// player 액터 추가.
	AddNewActor(new Player());
}

GameLevel::~GameLevel()
{

}
