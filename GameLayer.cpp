#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();
	
	points = 0;
	projectilesLeft = 10;
	textPoints = new Text("0", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	player = new Player(50, 50, game);

	//Ampliación:Jugador tiene vidas.
	textLives = new Text("3", WIDTH * 0.77, HEIGHT * 0.04, game);
	textLives->content = to_string(player->lives);

	 
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundLives= new Actor("res/corazon.png",
		WIDTH * 0.70, HEIGHT * 0.05, 24, 24, game);//Ampliación:Jugador tiene vidas.
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	coins.clear();// Ampliación :Monedas
	powerups.clear();//Ampliación: Disparos limitados
	bombs.clear();//Ampliación : Bombas

	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game));
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	 
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}



}

void GameLayer::update() {
	background->update();

	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}
	for (auto const& coin : coins) {//Ampliación: Monedas.
		coin->update();
	}
	for (auto const& bomb : bombs) {//Ampliación:Bombas
		bomb->update();
	}
	for (auto const& powerup :powerups) {//Ampliación: Disparos limitados
		powerup->update();
	}
	player->update();
	// Generar enemigos
	newEnemyTime--;
	newBombTime--;
	newPowerUpTime--;
	newCoinTime--;
	int rX, rY;
	if (newEnemyTime <= 0) {
		  rX = (rand() % (600 - 500)) + 1 + 500;
		  rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = 70;
	}if(newCoinTime<=0){
		//-Ampliación: Monedas.
		rX = (rand() % (600 - 500)) + 1 + 300;
		rY = (rand() % (310 - 50)) + 1 + 40;
		coins.push_back(new Coin(rX, rY, game));
		newCoinTime = 160;
		//-Ampliación: Disparos limitados - powerups
	}if(newPowerUpTime<=0){
		rX = (rand() % (500 - 200)) + 1 + 300;
		rY = (rand() % (270 - 40)) + 1 + 40;
		powerups.push_back(new PowerUp(rX, rY, game));
		newPowerUpTime =540;
	 }
	//Ampliación:Bombas
	if (newBombTime <= 0) {
		rX = (rand() % (600 - 500)) + 1 + 200;
		rY = (rand() % (300 - 60)) + 1 + 20;
		bombs.push_back(new Bomb(rX, rY, game));
		newBombTime = 600;
	}

	// Colisiones -Ampliación: Monedas.
	list<Coin*> deleteCoins;
	for (auto const& coin : coins) {
		bool cInList = std::find(deleteCoins.begin(),
			deleteCoins.end(),
			coin) != deleteCoins.end();
		if (player->isOverlap(coin)) {
			if  (!cInList) {
				points++;
				textPoints->content = to_string(points);
				deleteCoins.push_back(coin);
			}
		}
	}
	// Colisiones -Ampliación: Jugador tiene vidas.
 	list<Enemy*> deleteEnemies;
	for (auto const& enemy : enemies) {				 
		bool eInList = std::find(deleteEnemies.begin(),
			deleteEnemies.end(),
			enemy) != deleteEnemies.end();
		if (player->isOverlap(enemy)) {
			if (player->lives ==0) {
				init();
				return;
			}
			else if(  !eInList){
				player->lives--;
				textLives->content = to_string(player->lives);
				deleteEnemies.push_back(enemy);
			} 
		}
	}
		// Colisiones -Ampliación: Bombas
	list<Bomb*> deleteBombs;
	for (auto const& bomb : bombs) {
		bool bInList = std::find(deleteBombs.begin(),
			deleteBombs.end(),
			bomb) != deleteBombs.end();
		if (player->isOverlap(bomb)) {
			if (!bInList) {
				deleteBombs.push_back(bomb);
				deleteEnemies = enemies;
				newEnemyTime = 20;// Evitar que la pantalla quede vacía demasiado tiempo
			}
		}
	}
	//Ampliación : Número finito de disparos.

	list<PowerUp*> deletePowerup;
	
	for (auto const& powerup : powerups) {
		bool puInList = std::find(deletePowerup.begin(),
			deletePowerup.end(), powerup) != deletePowerup.end();
		if (player->isOverlap(powerup)) {
			if (!puInList) {
				player->shotsLeft += 10;
				deletePowerup.push_back(powerup);
			}
		}
	
	}

	//Popular lista de proyectiles para poder borrarlos.
	list<Projectile*> deleteProjectiles;
	for (auto const& projectile : projectiles) {
		if (projectile->isInRender() == false) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}
	//Ampliación: Enemigos con vida ampliada.
	// Colisiones , Enemy - Projectile
	//La lista delete projectiles se ha iniciado anteriormente, para eliminar los proyectiles que no impactan 
	//En este caso, se eliminan los enemigos que impactan con los proyectiles y a su vez se elimina el proyectil en sí
	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				enemy->lives--;//Ampliación: Enemigos con vida ampliada.
				if (enemy->lives == 0) { 
						bool eInList = std::find(deleteEnemies.begin(),
							deleteEnemies.end(),
							enemy) != deleteEnemies.end();

						if (!eInList) {
							deleteEnemies.push_back(enemy);
							player->shotsLeft++;//Ampliación : Número finito de disparos.
						}

						points++;
						textPoints->content = to_string(points);

						

				}
				 
				
				}
			}
			}


	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();
	//Ampliación: Monedas
	for (auto const& delCoin :deleteCoins)
	{
		coins.remove(delCoin);
		delete delCoin;
	}
	deleteCoins.clear();
	//Ampliación: Bombas
	for (auto const& delBomb : deleteBombs)
	{
		bombs.remove(delBomb);
		delete delBomb;
	}
	deleteBombs.clear();
	//Ampliación: Número finito de disparos.
	for (auto const& delpowerup : deletePowerup)
	{
  		powerups.remove(delpowerup);
		delete delpowerup;
	}
	deletePowerup.clear();
	//cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	background->draw();
	player->draw();
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	for (auto const& coin: coins) {//Ampliación: Monedas
		coin->draw();
	}
	for (auto const& bomb: bombs) {//Ampliación: Bombas
		bomb->draw();
	}
 
	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	for (auto const& powerup :powerups) {
		powerup->draw();
	}
	backgroundPoints->draw();
	backgroundLives->draw();//Ampliación : El jugador tiene vidas.
	textLives->draw();
	textPoints->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}


	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}

	}

}

