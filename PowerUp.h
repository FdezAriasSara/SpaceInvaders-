 
#pragma once
#include "Game.h"
#include "Animation.h"
#include "Actor.h"
	class PowerUp : public Actor
{
public:
	PowerUp(float x, float y, Game* game);
	void update();
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
	void draw() override; // para que se actualice la animación
};

