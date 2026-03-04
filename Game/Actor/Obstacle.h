#pragma once
#include "Actor/Actor.h"

#include <vector>

using namespace Wanted;

class Obstacle : public Actor
{
	RTTI_DECLARATIONS(Obstacle, Actor);
public:
	Obstacle(int width = 30, int height = 5);
	~Obstacle();

	void CreateBody();
	virtual void Tick(float deltaTime) override;

	// Getter
	std::vector<Actor*> GetBody() { return body; }
private:
	int width;
	int height;
	bool bodySet = false;

	std::vector<Actor*> body;
};

