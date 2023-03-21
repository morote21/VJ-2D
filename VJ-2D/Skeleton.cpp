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
	spritesheet.loadFromFile("images/rebel.png", TEXTURE_PIXEL_FORMAT_RGBA);
	//spritesheet.loadFromFile("images/skeleton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(30, 38);
	sprite = Sprite::createSprite(size, glm::vec2(1/12.f, 0.5), &spritesheet, &shaderProgram); // para el quad representado (+ tamaño frame)
	//sprite = Sprite::createSprite(size, glm::vec2(0.25f, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);	// hay que poner el numero de animaciones de Animations
	
	sprite->setAnimationSpeed(MOVE_LEFT, 20);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(1/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(2/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(3/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(4/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(5/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(6/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(7/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(8/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(9/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(10/12.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(11/12.f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 20);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(3 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(4 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(5 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(6 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(7 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(8 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(9 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(10 / 12.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(11 / 12.f, 0.5f));


	/*
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));
	*/
	sprite->changeAnimation(MOVE_RIGHT, true);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSkeleton.x), float(tileMapDispl.y + posSkeleton.y))); // default value?

}

void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);
	// ...
	int Yplatform = posSkeleton.y + size.y; // para los collisionMoveDown

	map->collisionMoveDown(posSkeleton, size, &posSkeleton.y);
	if (sprite->animation() == MOVE_RIGHT) {

		if (map->collisionMoveRight(glm::ivec2(posSkeleton.x + 2, posSkeleton.y), size) ||
			!map->collisionMoveDown(glm::ivec2(posSkeleton.x + 27, Yplatform), glm::ivec2(1, 1), &Yplatform)) // si hay muro enfrente o NO hay plataforma que pisar (pasarle Yplatform es sólo para relleno)
			sprite->changeAnimation(MOVE_LEFT, true);

		else posSkeleton.x += 2;

		
	}else {// caso MOVE_LEFT

		if (map->collisionMoveLeft(glm::ivec2(posSkeleton.x - 2, posSkeleton.y), size) ||
			!map->collisionMoveDown(glm::ivec2(posSkeleton.x - 3, Yplatform), glm::ivec2(1, 1), &Yplatform)) // (igual)
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