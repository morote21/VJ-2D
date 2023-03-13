#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skeleton.h"

#define MOVE_LEFT 0
#define MOVE_RIGHT 1

Skeleton::~Skeleton()
{
	sprite->free();
	map->free();
}

void Skeleton::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(32, 32);
	sprite = Sprite::createSprite(size, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram); // para el quad representado (+ tamaño frame)
	sprite->setNumberAnimations(2);	// hay que poner el numero de animaciones de Animations

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y))); // default value?

}

void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);
	// ...
	int Yplatform = posSkeleton.y + 1; // para los collisionMoveDown
	//ESTA VERSIÓN DEL CHECKEO LLEGA AL LÍMITE DE LAS PLATAFORMAS; QUIZÁS QUERAMOS QUE CAMBIE DE DIRECCIÓN ANTES, CUANDO UN SOLO PÍXEL ESTÉ
	// YA FUERA EN EL PASO SIGUIENTE (ÉSTO VALDRÍA PARA EL ESQUELETO)

	if (sprite->animation() == MOVE_RIGHT) {
		if (map->collisionMoveRight(glm::ivec2(posSkeleton.x + 2, posSkeleton.y), glm::ivec2(32, 32)) ||
			!map->collisionMoveDown(glm::ivec2(posSkeleton.x + 2, Yplatform), glm::ivec2(32, 32), &Yplatform)) // si hay muro enfrente o NO hay plataforma que pisar
			sprite->changeAnimation(MOVE_LEFT);

		else posSkeleton.x += 2;

		
	}else {// caso MOVE_LEFT
		if (map->collisionMoveLeft(glm::ivec2(posSkeleton.x - 2, posSkeleton.y), glm::ivec2(32, 32)) ||
			!map->collisionMoveDown(glm::ivec2(posSkeleton.x - 2, Yplatform), glm::ivec2(32, 32), &Yplatform)) // (igual)
			sprite->changeAnimation(MOVE_RIGHT);
		
		else posSkeleton.x -= 2;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y)));
}

void Skeleton::render()
{
	sprite->render();
}

void Skeleton::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Skeleton::setPosition(const glm::vec2& pos)
{
	posSkeleton = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y)));
}

glm::vec2 Skeleton::getPosition()
{
	return posSkeleton;
}

glm::ivec2 Skeleton::getSize() const
{
	return size;
}