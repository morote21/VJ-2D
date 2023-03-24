#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Vampire.h"

#define TIME_TO_TRANSFORM 7000.f

Vampire::~Vampire()
{
	sprite->free();
	map->free();
}

void Vampire::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/skeleton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(25, 35);
	sprite = Sprite::createSprite(size, glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram); // para el quad representado (+ tamaño frame)
	sprite->setNumberAnimations(3);	// hay que poner el numero de animaciones de Animations

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(FLY, 0);
	sprite->addKeyframe(FLY, glm::vec2(0.f, 0.f));

	//currentForm = 0; <- de momento no se usa
	//speed.x = 2; <- se inicializan en el update()
	//speed.y = -2;
	sprite->changeAnimation(MOVE_RIGHT, true);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVampire.x), float(tileMapDispl.y + posVampire.y))); // default value?

	currentTime = 0.0;

}

void Vampire::update(int deltaTime)
{
	sprite->update(deltaTime);
	// ...

	if (currentTime != -1) {
		currentTime += deltaTime;

		if (currentTime >= TIME_TO_TRANSFORM)
			currentTime = -1; // marca para cambio de forma (de momento sirve en lugar de estado)
	}

	//cout << "Time state: " << currentTime << endl;

	if (currentTime == -1) { // si después de eso estamos aquí

		if (sprite->animation() != FLY) { // Cambiar a murciélago
			if (sprite->animation() == MOVE_RIGHT)
				speed.x = 2;
			else
				speed.x = -2;

			speed.y = -2;
			currentTime = 0.0;
			sprite->changeAnimation(FLY, true);

		}else { // Cambiar a humano (si sobre un suelo)
			// Eje x //
			if (speed.x > 0) {
				if (map->collisionMoveRight(glm::ivec2(posVampire.x + speed.x, posVampire.y), size))
					speed.x = -2;
				else
					posVampire.x += speed.x;

			}else{
				if (map->collisionMoveLeft(glm::ivec2(posVampire.x + speed.x, posVampire.y), size))
					speed.x = 2;
				else
					posVampire.x += speed.x;
			}

			// Eje y //
			posVampire.y += speed.y;
			if (speed.y > 0) {
				if (map->collisionMoveDown(posVampire, size, &posVampire.y)) {
					currentTime = 0.0;
					if (speed.x > 0)
						sprite->changeAnimation(MOVE_RIGHT, true);
					else
						sprite->changeAnimation(MOVE_LEFT, true);
				}
			}else{
				if (map->collisionMoveUpAnything(posVampire, size, &posVampire.y))
					speed.y = 2;
			}
		}

	}else{ // Comportamiento cuando no se transforma

		if (sprite->animation() != FLY) { // FORMA "HUMANA"
			
			int Yplatform = posVampire.y + 35; // para los collisionMoveDown

			if (sprite->animation() == MOVE_RIGHT) {

				if (map->collisionMoveRight(glm::ivec2(posVampire.x + 2, posVampire.y), glm::ivec2(25, 35)) ||
					!map->collisionMoveDown(glm::ivec2(posVampire.x + 27, Yplatform), glm::ivec2(1, 1), &Yplatform)) // si hay muro enfrente o NO hay plataforma que pisar (pasarle Yplatform es sólo para relleno)
					sprite->changeAnimation(MOVE_LEFT, true);

				else posVampire.x += 2;
		
			}else {// caso MOVE_LEFT

				if (map->collisionMoveLeft(glm::ivec2(posVampire.x - 2, posVampire.y), glm::ivec2(25, 35)) ||
					!map->collisionMoveDown(glm::ivec2(posVampire.x - 3, Yplatform), glm::ivec2(1, 1), &Yplatform)) // (igual)
					sprite->changeAnimation(MOVE_RIGHT, true);

				else posVampire.x -= 2;
			}

		}
		else { // FORMA MURCIÉLAGO
			// Eje x //
			if (speed.x > 0) {
				if (map->collisionMoveRight(glm::ivec2(posVampire.x + speed.x, posVampire.y), size))
					speed.x = -2;
				else
					posVampire.x += speed.x;

			}
			else {
				if (map->collisionMoveLeft(glm::ivec2(posVampire.x + speed.x, posVampire.y), size))
					speed.x = 2;
				else
					posVampire.x += speed.x;
			}

			// Eje y //
			int upPlatform = posVampire.y - 4;
			if (!map->collisionMoveUpAnything(glm::ivec2(posVampire.x, upPlatform), size, &upPlatform) ||
				!map->collisionMoveDown(glm::ivec2(posVampire.x, posVampire.y + 4), size, &upPlatform) ){
				posVampire.y += speed.y;
				if (speed.y > 0) {
					if (map->collisionMoveDown(posVampire, size, &posVampire.y))
						speed.y = -2;

				}else{
					if (map->collisionMoveUpAnything(posVampire, size, &posVampire.y))
						speed.y = 2;
				}
			}

		}

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVampire.x), float(tileMapDispl.y + posVampire.y)));
}

void Vampire::render()
{
	sprite->render();
}

void Vampire::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Vampire::setPosition(const glm::vec2& pos)
{
	posVampire = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVampire.x), float(tileMapDispl.y + posVampire.y)));
}

glm::vec2 Vampire::getPosition()
{
	return posVampire;
}

glm::ivec2 Vampire::getSize() const
{
	return size;
}