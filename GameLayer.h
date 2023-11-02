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
	Text* textLives;//Ampliación: Jugador tiene vidas.
	int points;
	int projectilesLeft;//Ampliación : Disparos limitados.
	int newEnemyTime = 0;
	int newBombTime = 0;//Ampliación: Bombs
	int newCoinTime = 0;//Ampliación: Monedas
	int newPowerUpTime = 0;//Ampliación:Disparos limitados
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundLives;//Ampliación: Jugador tiene vidas.

	list<Coin*> coins;//Ampliación: Monedas
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<PowerUp*> powerups;//Ampliación: Disparos limitados	
	list<Bomb*> bombs;//Ampliación: Bombas
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


};

