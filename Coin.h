#pragma once
#include "Game.h"
#include "Actor.h"
class Coin : public Actor
{
public:
	Coin(float x, float y, Game* game);
	void update();

};

