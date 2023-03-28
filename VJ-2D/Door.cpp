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
	sprite = Sprite::createSprite(size, glm::vec2(1/6.f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(CLOSED, 1);
	sprite->addKeyframe(CLOSED, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(OPENING, 14);
	sprite->addKeyframe(OPENING, glm::vec2(1/6.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(2/6.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(3/6.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(4/6.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(5/6.f, 0.f));
	sprite->addKeyframe(OPENING, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(1/6.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(2/6.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(3/6.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(4/6.f, 0.5f));
	sprite->addKeyframe(OPENING, glm::vec2(5/6.f, 0.5f));

	sprite->changeAnimation(CLOSED, false);

	// Las posiciones para el renderizado se les suma el offset del hud de las vidas y el score,
	// pero las posiciones que se tienen en cuenta para ver si dos objetos estan en la misma posicion
	// por ejemplo son las posiciones dentro de lo que es el mapa en si, sin tener en cuenta el offset
	// (es decir, las x son lo mismo pero las y - 40, ya que no se tiene en cuenta el hud 

	position = glm::vec2(float(doorMapPos.x * 40), float((doorMapPos.y - 1) * 20));
	sprite->setPosition(glm::vec2(position.x + tileMapPos.x, position.y + tileMapPos.y));
	//position = glm::vec2(float(tileMapPos.x + doorMapPos.x * 40), float(tileMapPos.y + (doorMapPos.y - 3) * 20));
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

glm::vec2 Door::getPosition()
{
	return position;
}
