#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Coin.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h" 
#include "PowerUp.h" 
#include "Bomb.h"
#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);

	Audio* audioBackground;
	Text* textPoints;
	Text* textLives;//Ampliaci�n: Jugador tiene vidas.
	int points;
	int projectilesLeft;//Ampliaci�n : Disparos limitados.
	int newEnemyTime = 0;
	int newBombTime = 0;//Ampliaci�n: Bombs
	int newCoinTime = 0;//Ampliaci�n: Monedas
	int newPowerUpTime = 0;//Ampliaci�n:Disparos limitados
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundLives;//Ampliaci�n: Jugador tiene vidas.

	list<Coin*> coins;//Ampliaci�n: Monedas
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<PowerUp*> powerups;//Ampliaci�n: Disparos limitados	
	list<Bomb*> bombs;//Ampliaci�n: Bombas
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


};

