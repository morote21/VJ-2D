#include "Stopwatch.h"
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

#define TIME_TO_COLLECT 7000.f

#define FREEZE_TIME 10000.f

void Stopwatch::init(const glm::ivec2& tileMapPos, glm::ivec2 itemPos, ShaderProgram& shaderProgram)
{
	// Tal y como está, deberías hacer el init SÓLO cuando aparezca

	spritesheet.loadFromFile("images/life.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(20, 20);
	sprite = Sprite::createSprite(size, glm::vec2(1.f / 10.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(1.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(2.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(3.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(4.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(5.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(6.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(7.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(8.f / 10.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(9.f / 10.f, 0.f));

	sprite->changeAnimation(IDLE, true);

	clockState = 1;
	timeLeft = TIME_TO_COLLECT;
	position = itemPos;
	sprite->setPosition(glm::vec2(position.x + tileMapPos.x, position.y + tileMapPos.y));
}

void Stopwatch::update(int deltaTime, char& timeState)
{
	if (clockState > 0) { // activo
		timeLeft -= deltaTime;

		if (timeLeft <= 0.0) {
			
			if (clockState == 2)
				timeState = 2; // poner en marcha enemigos
			
			clockState = 0; // se desactiva
		}

	}
	sprite->update(deltaTime);
}

void Stopwatch::render()
{
	if (clockState == 1)
		sprite->render();
}

Stopwatch::~Stopwatch()
{
	sprite->free();
}

glm::ivec2 Stopwatch::getSize() const
{
	return size;
}

glm::vec2 Stopwatch::getPosition()
{
	return position;
}

bool Stopwatch::isVisible()
{
	return (clockState == 1);
}

void Stopwatch::setVisibility(bool visible)
{
	if (visible) {
		timeLeft = TIME_TO_COLLECT;
		clockState = 1;
	}
	else
		clockState = 0; // a diferencia de otros objetos, ésto lo desactiva totalmente (ya lo usaremos)

}

void Stopwatch::activate(char& timeState) // se asume visibilidad (ya veremos)
{
	timeState = 1; // parar enemigos
	clockState = 2;
	timeLeft = FREEZE_TIME;
}

