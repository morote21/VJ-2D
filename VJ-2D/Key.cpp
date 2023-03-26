#include "Key.h"
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;


void Key::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/key-white.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(32, 32);
	sprite = Sprite::createSprite(size, glm::vec2(1.f / 12.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(FLOATING, 8);
	sprite->addKeyframe(FLOATING, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(1.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(2.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(3.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(4.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(5.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(6.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(7.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(8.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(9.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(10.f / 12.f, 0.f));
	sprite->addKeyframe(FLOATING, glm::vec2(11.f / 12.f, 0.f));

	sprite->changeAnimation(FLOATING, true);

	position = glm::vec2(float(40), float(18*20));
	sprite->setPosition(glm::vec2(position.x + tileMapPos.x, position.y + tileMapPos.y));
}

void Key::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Key::render()
{
	sprite->render();
}

Key::~Key()
{
	sprite->free();
}

glm::ivec2 Key::getSize() const
{
	return size;
}

glm::vec2 Key::getPosition()
{
	return position;
}