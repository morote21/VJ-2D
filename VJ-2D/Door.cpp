#include "Door.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

Door::~Door()
{
	sprite->free();
}

void Door::init(const glm::ivec2& tileMapPos, glm::ivec2 doorMapPos,  ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/doors.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(40, 40);
	sprite = Sprite::createSprite(size, glm::vec2(1.f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPENING, 1);
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.75f));

	sprite->changeAnimation(CLOSED, false);
	sprite->setPosition(glm::vec2(float(tileMapPos.x + doorMapPos.x*40), float(tileMapPos.y + (doorMapPos.y - 1)*20)));
}


void Door::update(int deltaTime, bool opened)
{
	sprite->update(deltaTime);
	if (opened && sprite->animation() == CLOSED) {
		sprite->changeAnimation(OPENING, false);
	}
}

void Door::render()
{
	sprite->render();
}

glm::ivec2 Door::getSize() const
{
	return size;
}

glm::vec2 Door::getPos()
{
	return position;
}
