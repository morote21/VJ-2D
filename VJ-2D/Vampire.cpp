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
	spritesheet.loadFromFile("images/marspeople.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(35, 35);
	hitboxSize = glm::ivec2(35, 25);
	sprite = Sprite::createSprite(size, glm::vec2(1/10.f, 1/4.f), &spritesheet, &shaderProgram); // para el quad representado (+ tama�o frame)
	sprite->setNumberAnimations(5);	// hay que poner el numero de animaciones de Animations

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1/10.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2/10.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3/10.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4/10.f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5 / 10.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 / 10.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7 / 10.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 / 10.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 / 10.f, 0.f));

	sprite->setAnimationSpeed(FLY_LEFT, 8);
	sprite->addKeyframe(FLY_LEFT, glm::vec2(0.f, 1/4.f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(1 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(2 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(3 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_LEFT, glm::vec2(4 / 10.f, 1/4.f));

	sprite->setAnimationSpeed(FLY_RIGHT, 8);
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(5 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(6 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(7 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(8 / 10.f, 1/4.f));
	sprite->addKeyframe(FLY_RIGHT, glm::vec2(9 / 10.f, 1/4.f));

	sprite->setAnimationSpeed(TRANSFORM, 16);
	
	for (int j = 2; j <= 3; j++) {
		for (int i = 0; i < 7; i++) {
			sprite->addKeyframe(TRANSFORM, glm::vec2(i/10.f, j/4.f));
		}
	}

	//currentForm = 0; <- de momento no se usa
	//speed.x = 2; <- se inicializan en el update()
	//speed.y = -2;
	sprite->changeAnimation(MOVE_RIGHT, true);
	tileMapDispl = tileMapPos;
	//hitboxPos = glm::vec2(posVampire.x + 5, posVampire.y + 10);
	//hitboxSize = glm::vec2(25, 25);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posVampire.x), float(tileMapDispl.y + posVampire.y))); // default value?
	hitboxPos = glm::ivec2(posVampire.x, posVampire.y + 10);
	currentTime = 0.0;
	transformationTime = 0.f;
	flying = false;
}

void Vampire::update(int deltaTime)
{
	sprite->update(deltaTime);
	// ...

	if (transforming) {
		transformationTime += deltaTime;
		if (sprite->animation() != TRANSFORM) {
			sprite->changeAnimation(TRANSFORM, false);
		}
		if (transformationTime >= 1000.f) {
			transforming = false;
			transformationTime = 0;
			if (speed.x > 0)
				if (flying) {
					flying = false;
					sprite->changeAnimation(MOVE_RIGHT, true);
				}
				else {
					flying = true;
					sprite->changeAnimation(FLY_RIGHT, true);
				}
			else {
				if (flying) {
					flying = false;
					sprite->changeAnimation(MOVE_LEFT, true);
				}
				else {
					flying = true;
					sprite->changeAnimation(FLY_LEFT, true);
				}
			}
		}
	}
	else {

		if (currentTime != -1) {
			currentTime += deltaTime;

			if (currentTime >= TIME_TO_TRANSFORM)
				currentTime = -1; // marca para cambio de forma (de momento sirve en lugar de estado)
		}


		if (currentTime == -1) { // si despu�s de eso estamos aqu�

			if (sprite->animation() != FLY_LEFT && sprite->animation() != FLY_RIGHT) { // Cambiar a murci�lago
				transforming = true;
				
				if (sprite->animation() == MOVE_RIGHT) {
					speed.x = 2;
					//sprite->changeAnimation(FLY_RIGHT, true);
				}
				else {
					speed.x = -2;
					//sprite->changeAnimation(FLY_LEFT, true);
				}
				

				speed.y = -2;
				currentTime = 0.0;

			}else { // Cambiar a humano (si sobre un suelo)
				// Eje x //
				if (speed.x > 0) {
					if (map->collisionMoveRight(glm::ivec2(posVampire.x + speed.x, posVampire.y), size))
						speed.x = -2;
					else {
						posVampire.x += speed.x;
						hitboxPos.x = posVampire.x;
					}

				}else{
					if (map->collisionMoveLeft(glm::ivec2(posVampire.x + speed.x, posVampire.y), size))
						speed.x = 2;
					else {
						posVampire.x += speed.x;
						hitboxPos.x = posVampire.x;
					}
				}

				// Eje y //
				posVampire.y += speed.y;
				hitboxPos.y = posVampire.y + 10;
				if (speed.y > 0) {
					if (map->collisionMoveDown(posVampire, size, &posVampire.y)) {
						currentTime = 0.0;
						transforming = true;
						//if (speed.x > 0)
						//	sprite->changeAnimation(MOVE_RIGHT, true);
						//else
						//	sprite->changeAnimation(MOVE_LEFT, true);
					}
				}else{
					if (map->collisionMoveUpAnything(posVampire, size, &posVampire.y))
						speed.y = 2;
				}
			}

		}else{ // Comportamiento cuando no se transforma

			if (sprite->animation() != FLY_LEFT && sprite->animation() != FLY_RIGHT) { // FORMA "HUMANA"
			
				int Yplatform = posVampire.y + 35; // para los collisionMoveDown

				if (sprite->animation() == MOVE_RIGHT) {

					if (map->collisionMoveRight(glm::ivec2(posVampire.x + 2, posVampire.y), size) ||
						!map->collisionMoveDown(glm::ivec2(posVampire.x + size.y + 1, Yplatform), glm::ivec2(1, 1), &Yplatform)) {
						// si hay muro enfrente o NO hay plataforma que pisar (pasarle Yplatform es s�lo para relleno)
						sprite->changeAnimation(MOVE_LEFT, true);
						hitboxPos.y = posVampire.y + 10;
					}

					else {
						posVampire.x += 2;
						hitboxPos.x = posVampire.x;
					}
				}else {// caso MOVE_LEFT

					if (map->collisionMoveLeft(glm::ivec2(posVampire.x - 2, posVampire.y), size) ||
						!map->collisionMoveDown(glm::ivec2(posVampire.x - 3, Yplatform), glm::ivec2(1, 1), &Yplatform)) { // (igual)
						sprite->changeAnimation(MOVE_RIGHT, true);
						hitboxPos.y = posVampire.y + 10;
					}

					else {
						posVampire.x -= 2;
						hitboxPos.x = posVampire.x;
					}
				}

			}
			else { // FORMA MURCI�LAGO
				// Eje x //
				if (speed.x > 0) {
					if (map->collisionMoveRight(glm::ivec2(posVampire.x + speed.x, posVampire.y), size)) {
						speed.x = -2;
						sprite->changeAnimation(FLY_LEFT, true);
					}
					else {
						posVampire.x += speed.x;
						hitboxPos.x = posVampire.x;
					}

				}
				else {
					if (map->collisionMoveLeft(glm::ivec2(posVampire.x + speed.x, posVampire.y), size)) {
						speed.x = 2;
						sprite->changeAnimation(FLY_RIGHT, true);
					}
					else {
						posVampire.x += speed.x;
						hitboxPos.x = posVampire.x;
					}
				}

				// Eje y //
				int upPlatform = posVampire.y - 4;
				if (!map->collisionMoveUpAnything(glm::ivec2(posVampire.x, upPlatform), size, &upPlatform) ||
					!map->collisionMoveDown(glm::ivec2(posVampire.x, posVampire.y + 4), size, &upPlatform) ){
					posVampire.y += speed.y;
					hitboxPos.y = posVampire.y + 10;
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
	hitboxPos = glm::vec2(posVampire.x, posVampire.y + 10);
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

glm::vec2 Vampire::getHitboxPosition()
{
	return hitboxPos;
}

glm::ivec2 Vampire::getHitboxSize() const
{
	return hitboxSize;
}

void Vampire::resetAnimation() {
	currentTime = 0.0;
	transformationTime = 0.f;
	flying = false;
	sprite->changeAnimation(MOVE_RIGHT, true);
}