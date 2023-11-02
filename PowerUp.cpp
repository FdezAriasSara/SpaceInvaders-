#include "PowerUp.h"

PowerUp::PowerUp(float x, float y, Game* game) :
	Actor("res/icono_recolectable.png", x, y, 40, 40, game) {
	aMoving = new Animation("res/recolectable.png", width, height,
		32, 256, 6, 3, game);
	animation = aMoving;
}

void PowerUp::update() {

	// Actualizar la animación
	animation->update();

	vx = -1;
	x = x + vx;

}

void PowerUp::draw() {
	animation->draw(x, y);
}
