#include "Gem.h"
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

#define TIME_TO_COLLECT 7000.f

void Gem::init(const glm::ivec2& tileMapPos, glm::ivec2 itemPos, ShaderProgram& shaderProgram)
{
	// Tal y como está, deberías hacer el init SÓLO cuando aparezca

	spritesheet.loadFromFile("images/gem.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(16, 16);
	sprite = Sprite::createSprite(size, glm::vec2(1.f / 10.f, 0.5f), &spritesheet, &shaderProgram);
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
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(1.f / 10.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(2.f / 10.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(3.f / 10.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(4.f / 10.f, 0.5f));

	sprite->changeAnimation(IDLE, true);

	timeLeft = TIME_TO_COLLECT;
	position = itemPos;
	sprite->setPosition(glm::vec2(position.x + tileMapPos.x, position.y + tileMapPos.y));
}

void Gem::update(int deltaTime)
{
	if (timeLeft != -1) {
		timeLeft -= deltaTime;

		if (timeLeft <= 0.0)
			timeLeft = -1; // marca para hacer invisible (no coleccionable)
	}

	sprite->update(deltaTime);
}

void Gem::render()
{
	if (timeLeft != -1)
		sprite->render();
}

Gem::~Gem()
{
	sprite->free();
}

glm::ivec2 Gem::getSize() const
{
	return size;
}

glm::vec2 Gem::getPosition()
{
	return position;
}

bool Gem::isVisible() 
{
	return (timeLeft != -1);
}

void Gem::setVisibility(bool visible) 
{
	if (visible)
		timeLeft = TIME_TO_COLLECT;
	else
		timeLeft = -1;
}