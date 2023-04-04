#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Mummy.h"
#include "Scene.h"

Mummy::~Mummy()
{
	sprite->free();
	map->free();
}

void Mummy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	// SETUP momia //

	spritesheet.loadFromFile("images/mummy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(30, 38);
	sprite = Sprite::createSprite(size, glm::vec2(1 / 17.f, 0.25), &spritesheet, &shaderProgram); // para el quad representado (+ tamaño frame)
	sprite->setNumberAnimations(4);	// hay que poner el numero de animaciones de Animations

	sprite->setAnimationSpeed(MOVE_LEFT, 20);
	for (int i = 0; i < 17; i++) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i/17.f, 0.f));
	}


	sprite->setAnimationSpeed(MOVE_RIGHT, 20);
	for (int i = 16; i >= 0; i--) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i / 17.f, 0.25f));
	}

	sprite->setAnimationSpeed(BREATHE_LEFT, 20);
	for (int i = 0; i < 17; i++) {
		sprite->addKeyframe(BREATHE_LEFT, glm::vec2(i / 17.f, 0.5f));
	}

	sprite->setAnimationSpeed(BREATHE_RIGHT, 20);
	for (int i = 16; i >= 0; i--) {
		sprite->addKeyframe(BREATHE_RIGHT, glm::vec2(i / 17.f, 0.75f));
	}


	sprite->changeAnimation(MOVE_RIGHT, true);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMummy.x), float(tileMapDispl.y + posMummy.y))); // default value?
	
	// SETUP veneno //

	poisonSpritesheet.loadFromFile("images/poison.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sizePoison = glm::ivec2(60, 20);
	poisonSprite = Sprite::createSprite(sizePoison, glm::vec2(1 / 7.f, 1 / 4.f), &poisonSpritesheet, &shaderProgram); // para el quad representado (+ tamaño frame)
	poisonSprite->setNumberAnimations(2);

	/*
	poisonSprite->setAnimationSpeed(POISON_LEFT, 10);
	poisonSprite->addKeyframe(POISON_LEFT, glm::vec2(0.f, 0.5f));
	poisonSprite->addKeyframe(POISON_LEFT, glm::vec2(1 / 4.f, 0.5f));
	poisonSprite->addKeyframe(POISON_LEFT, glm::vec2(1 / 2.f, 0.5f));
	*/

	poisonSprite->setAnimationSpeed(POISON_LEFT, 10);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 7; j++) {
			poisonSprite->addKeyframe(POISON_LEFT, glm::vec2(j/7.f, i/4.f));
		}
	}

	/*
	poisonSprite->setAnimationSpeed(POISON_RIGHT, 10);
	poisonSprite->addKeyframe(POISON_RIGHT, glm::vec2(0.f, 0.f));
	poisonSprite->addKeyframe(POISON_RIGHT, glm::vec2(1 / 4.f, 0.f));
	poisonSprite->addKeyframe(POISON_RIGHT, glm::vec2(1 / 2.f, 0.f));
	*/

	poisonSprite->setAnimationSpeed(POISON_RIGHT, 10);
	for (int i = 2; i <= 3; i++) {
		for (int j = 6; j >= 0; j--) {
			poisonSprite->addKeyframe(POISON_RIGHT, glm::vec2(j / 7.f, i / 4.f));
		}
	}

	// (NO hace falta el changeAnimation, setPosition para este caso)

	tileMapDispl = tileMapPos;
	poisonVisible = false;

}

void Mummy::update(int deltaTime, glm::vec2 playerPos, glm::vec2 playerSize)
{
	sprite->update(deltaTime);

	int Yplatform = posMummy.y + size.y; // para los collisionMoveDown

	map->collisionMoveDown(posMummy, size, &posMummy.y);
	if (sprite->animation() == MOVE_RIGHT) {

		bool found = false;

		for (int current_x = posMummy.x + 2; !found && !map->collisionMoveRight(glm::ivec2(current_x, posMummy.y), size)  
													&& current_x <= posMummy.x + 2; ++current_x)
			found = Scene::samePosition(playerPos, playerSize, glm::ivec2(current_x, posMummy.y), sizePoison);

		if (found) { // si player enfrente nuestro (dentro del rango de búsqueda)

			sprite->changeAnimation(BREATHE_RIGHT, false);
			poisonSprite->changeAnimation(POISON_RIGHT, false);
			posPoison.x = posMummy.x + 25;
			posPoison.y = posMummy.y - 5;
			poisonSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPoison.x), float(tileMapDispl.y + posPoison.y))); // colocar veneno enfrente
			poisonVisible = true;

		
		}else if (map->collisionMoveRight(glm::ivec2(posMummy.x + 2, posMummy.y), size) ||
				 !map->collisionMoveDown(glm::ivec2(posMummy.x + 27, Yplatform), glm::ivec2(1, 1), &Yplatform)) // si hay muro enfrente o NO hay plataforma que pisar (pasarle Yplatform es sólo para relleno)
			sprite->changeAnimation(MOVE_LEFT, true);

		else posMummy.x += 1;


	}
	else if (sprite->animation() == MOVE_LEFT){

		bool found = false;

		for (int current_x = posMummy.x - 2; !found && !map->collisionMoveLeft(glm::ivec2(current_x, posMummy.y), size) 
													&& current_x >= posMummy.x - 2; --current_x)
			found = Scene::samePosition(playerPos, playerSize, glm::ivec2(current_x - sizePoison.x + size.x, posMummy.y), sizePoison);


		if (found) { // si player enfrente nuestro (dentro del rango de búsqueda)

			sprite->changeAnimation(BREATHE_LEFT, false);
			poisonSprite->changeAnimation(POISON_LEFT, false);
			posPoison.x = posMummy.x - sizePoison.y;
			posPoison.y = posMummy.y - 5;
			poisonSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPoison.x), float(tileMapDispl.y + posPoison.y))); // colocar veneno enfrente
			poisonVisible = true;



		}else if (map->collisionMoveLeft(glm::ivec2(posMummy.x - 2, posMummy.y), size) ||
				 !map->collisionMoveDown(glm::ivec2(posMummy.x - 3, Yplatform), glm::ivec2(1, 1), &Yplatform)) // (igual)
			sprite->changeAnimation(MOVE_RIGHT, true);

		else posMummy.x -= 1;
	
	}else if (sprite->animationFinished() ){ // para caso momia echando veneno

		if (sprite->animation() == BREATHE_LEFT)
			sprite->changeAnimation(MOVE_LEFT, true);
		else
			sprite->changeAnimation(MOVE_RIGHT, true);
	}

	
	if (poisonVisible){

		if (poisonSprite->animationFinished())
			poisonVisible = false;
		else
			poisonSprite->update(deltaTime);
	}
		

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMummy.x), float(tileMapDispl.y + posMummy.y)));
}

void Mummy::render()
{
	sprite->render();

	if (poisonVisible)
		poisonSprite->render();
}

void Mummy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Mummy::setPosition(const glm::vec2& pos)
{
	posMummy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMummy.x), float(tileMapDispl.y + posMummy.y)));
}

glm::vec2 Mummy::getPosition()
{
	return posMummy;
}

glm::ivec2 Mummy::getSize() const
{
	return size;
}

glm::vec2 Mummy::getPoisonPosition()
{
	return posPoison; 
}

glm::ivec2 Mummy::getPoisonSize() const
{
	return sizePoison;
}

bool Mummy::poisonExists() 
{
	return poisonVisible;
}