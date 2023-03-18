#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6	// ha de ser un numero de manera que si dividimos 180 entre este numero el resultado sea un numero entero
#define JUMP_HEIGHT 65
#define FALL_STEP 6
#define PLAYER_SPEED 3

Player::~Player()
{
	sprite->free();
	map->free();
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	lives = 3;
	score = 0;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(32, 32);
	sprite = Sprite::createSprite(size, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);	// hay que poner el numero de animaciones de Animations (incluyendo las que no son del player)

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

	sprite->changeAnimation(STAND_RIGHT, false);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT, true);
		posPlayer.x -= PLAYER_SPEED;
		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += PLAYER_SPEED;
			sprite->changeAnimation(STAND_LEFT, false);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT, true);
		posPlayer.x += PLAYER_SPEED;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= PLAYER_SPEED;
			sprite->changeAnimation(STAND_RIGHT, false);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT, false);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT, false);
	}

	if (bJumping)
	{
		// cuando esta en la zona alta del salto, se decrementa la velocidad de subida y bajada
		if (jumpAngle > 85 && jumpAngle < 95) jumpAngle += JUMP_ANGLE_STEP / 4;
		else
			jumpAngle += JUMP_ANGLE_STEP;

		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{	
			float angle = sin(3.14159f * jumpAngle / 180.f);
			posPlayer.y = int(startY - JUMP_HEIGHT * angle);
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
		
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	if (!bJumping) {
		// playerxleft
		if ((map->getTileInPos(posPlayer.x / 40, (posPlayer.y + int(sprite->getSpriteSize().y)) / 20)) == 2) {
			++score;
			map->tileStepped(posPlayer.x / 40, (posPlayer.y + int(sprite->getSpriteSize().y)) / 20);
		}
		// playerxright
		else if ((map->getTileInPos((posPlayer.x + int(sprite->getSpriteSize().x)) / 40, (posPlayer.y + int(sprite->getSpriteSize().y)) / 20)) == 2) {
			++score;
			map->tileStepped((posPlayer.x + int(sprite->getSpriteSize().x)) / 40, (posPlayer.y + int(sprite->getSpriteSize().y)) / 20);
		}
	}
	
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

glm::ivec2 Player::getSize() const
{
	return size;
}

int Player::getLives()
{
	return lives;
}

int Player::getScore()
{
	return score;
}