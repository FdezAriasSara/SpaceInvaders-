#include "PowerUp.h"

PowerUp::PowerUp(float x, float y, Game* game) :
	Actor("res/icono_recolectable.png", x, y, 40, 40, game) {
	aMoving = new Animation("res/recolectable.png", width, height,
		 256,32 , 1, 8, game);
	animation = aMoving;
 
}

void PowerUp::update() {

	// Actualizar la animaci�n
	animation->update();

	vx = -1;
	x = x + vx;

}

void PowerUp::draw() {
	animation->draw(x, y);
}
