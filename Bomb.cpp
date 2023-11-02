#include "Bomb.h"
 
Bomb::Bomb(float x, float y, Game* game) :
	Actor("res/asteroide.png", x, y, 50, 51, game) {

}

void Bomb::update() {

	vx = -1;
	x = x + vx;
}
