#include "Obstacle.h"
#include "Level/Level.h"


Obstacle::Obstacle(int width, int height)
	:super("")
{
	this->width = width;
	this->height = height;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			body.emplace_back(
				new Actor(
					"1",
					Vector2(j + 5, 10 + i),
					Color::White
				)
			);
		}
	}
	
}

Obstacle::~Obstacle()
{
	for (Actor*& actor : body)
	{
		if (actor)
		{
			delete actor;
			actor = nullptr;
		}
	}
}

void Obstacle::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!bodySet)
	{
		CreateBody();
	}
}

void Obstacle::CreateBody()
{
	if (bodySet) return;
	
	bodySet = true;
	for (Actor* actor : body)
	{
		GetOwner()->AddNewActor(actor);
	}
}