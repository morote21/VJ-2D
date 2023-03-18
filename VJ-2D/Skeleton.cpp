#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Skeleton.h"


Skeleton::~Skeleton()
{
	sprite->free();
	map->free();
}

void Skeleton::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/skeleton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(25, 35);
	sprite = Sprite::createSprite(size, glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram); // para el quad representado (+ tamaño frame)
	sprite->setNumberAnimations(2);	// hay que poner el numero de animaciones de Animations
	
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(MOVE_RIGHT, true);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y))); // default value?

}

void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);
	// ...
	int Yplatform = posSkeleton.y + 1; // para los collisionMoveDown

	if (sprite->animation() == MOVE_RIGHT) {
		if (map->collisionMoveRight(glm::ivec2(posSkeleton.x + 2, posSkeleton.y), glm::ivec2(25, 35)) ||
			!map->collisionMoveDown(glm::ivec2(posSkeleton.x + 25, Yplatform), glm::ivec2(25, 35), &posSkeleton.y)) // si hay muro enfrente o NO hay plataforma que pisar (OJO, aquí comprobamos UN tile para delante en lo 2o, para que no apure la plataforma; en el vampiro ésto no valdrá)
			sprite->changeAnimation(MOVE_LEFT, true);

		else posSkeleton.x += 2;

		
	}else {// caso MOVE_LEFT
		if (map->collisionMoveLeft(glm::ivec2(posSkeleton.x - 2, posSkeleton.y), glm::ivec2(25, 35)) ||
			!map->collisionMoveDown(glm::ivec2(posSkeleton.x - 25, Yplatform), glm::ivec2(25, 35), &posSkeleton.y)) // (igual)
			sprite->changeAnimation(MOVE_RIGHT, true);
		
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