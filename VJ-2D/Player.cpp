#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6
#define JUMP_HEIGHT 62
#define FALL_STEP 6
#define PLAYER_SPEED 3

#define HITBOXOFFSETX 8

Player::~Player()
{
	sprite->free();
	map->free();
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	falling = false;
	//lives = 3;
	//score = 0;
	invincible = false;
	spritesheet.loadFromFile("images/marco.png", TEXTURE_PIXEL_FORMAT_RGBA);
	size = glm::ivec2(30, 38);
	sprite = Sprite::createSprite(size, glm::vec2(1/16.f, 1/8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);	// hay que poner el numero de animaciones de Animations (incluyendo las que no son del player)

	sprite->setAnimationSpeed(STAND_LEFT, 4);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0/16.f, 4/8.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(1/16.f, 4/8.f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(2/16.f, 4/8.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 4);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0/16.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(1/16.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(2/16.f, 0.f));


	sprite->setAnimationSpeed(MOVE_LEFT, 16);
	for (float i = 0; i < 9; i++) {
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(i / 16.f, 5 / 8.f));
	}

	sprite->setAnimationSpeed(MOVE_RIGHT, 16);
	for (float i = 0; i < 9; i++) {
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i / 16.f, 1 / 8.f));
	}

	// implementarlo bien mas adelante
	sprite->setAnimationSpeed(AIR_RIGHT, 0);
	for (float i = 0; i < 9; i++) {
		sprite->addKeyframe(AIR_RIGHT, glm::vec2(i / 16.f, 2 / 8.f));
	}

	sprite->setAnimationSpeed(AIR_LEFT, 0);
	for (float i = 0; i < 9; i++) {
		sprite->addKeyframe(AIR_LEFT, glm::vec2(i / 16.f, 6 / 8.f));
	}
	
	
	sprite->changeAnimation(STAND_RIGHT, true);
	prevDir = 1;
	tileMapDispl = tileMapPos;
	posPlayer = glm::ivec2(tileMapDispl.x, tileMapDispl.y);
	sprite->setPosition(posPlayer);

	hitboxSize = glm::ivec2(8, 38);
	hitboxPos = glm::ivec2(posPlayer.x + HITBOXOFFSETX, posPlayer.y);
}

void Player::update(int deltaTime, int& score, int& lives)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		prevDir = 2;
		posPlayer.x -= PLAYER_SPEED;
		hitboxPos.x = posPlayer.x + HITBOXOFFSETX;
		if (map->collisionMoveLeft(hitboxPos, hitboxSize))
		{
			posPlayer.x += PLAYER_SPEED;
			hitboxPos.x = posPlayer.x + HITBOXOFFSETX;
			if (sprite->animation() != STAND_LEFT)
				sprite->changeAnimation(STAND_LEFT, true);
		}
		else {	
			if (!bJumping) {
				if (falling) {
					cout << "entra aqui 2" << endl;
					if (sprite->animation() != AIR_LEFT)
						sprite->changeAnimation(AIR_LEFT, false);
				}
				else {
					if (sprite->animation() != MOVE_LEFT)
						sprite->changeAnimation(MOVE_LEFT, true);
				}
			}
			else {
				if (sprite->animation() != AIR_LEFT)
					sprite->changeAnimation(AIR_LEFT, false);
			}
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		prevDir = 1;
		posPlayer.x += PLAYER_SPEED;
		hitboxPos.x = posPlayer.x + HITBOXOFFSETX;
		if (map->collisionMoveRight(hitboxPos, hitboxSize))
		{
			posPlayer.x -= PLAYER_SPEED;
			hitboxPos.x = posPlayer.x + HITBOXOFFSETX;
			if (sprite->animation() != STAND_RIGHT)
				sprite->changeAnimation(STAND_RIGHT, true);
		}
		else {
			if (!bJumping) {
				if (falling) {
					cout << "entra aqui 2" << endl;
					if (sprite->animation() != AIR_RIGHT)
						sprite->changeAnimation(AIR_RIGHT, false);
				}
				else {
					if (sprite->animation() != MOVE_RIGHT)
						sprite->changeAnimation(MOVE_RIGHT, true);
				}
			}
			else {
				if (sprite->animation() != AIR_RIGHT)
					sprite->changeAnimation(AIR_RIGHT, false);
			}
		}
	}

	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT, true);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT, true);
	}

	if (bJumping)
	{
		// cuando esta en la zona alta del salto, se decrementa la velocidad de subida y bajada
		if (jumpAngle > 85 && jumpAngle < 95) jumpAngle += JUMP_ANGLE_STEP / 4;
		else
			jumpAngle += JUMP_ANGLE_STEP;

		if (jumpAngle >= 180)	// >= de 180 asi no hace falta que el resultado de ir sumando sea exactamente 180
		{
			bJumping = false;
			falling = true;
			posPlayer.y = startY;
			hitboxPos.y = posPlayer.y;
		}
		else
		{	
			float angle = sin(3.14159f * jumpAngle / 180.f);
			posPlayer.y = int(startY - JUMP_HEIGHT * angle);
			hitboxPos.y = posPlayer.y;
			if (jumpAngle > 90) {
				bJumping = !map->collisionMoveDown(hitboxPos, hitboxSize, &posPlayer.y);
				hitboxPos.y = posPlayer.y;
			}
		}
		
	}
	else
	{
		posPlayer.y += FALL_STEP;
		hitboxPos.y = posPlayer.y;
		if (map->collisionMoveDown(hitboxPos, hitboxSize, &posPlayer.y))
		{
			
			falling = false;
			bJumping = false;
			hitboxPos.y = posPlayer.y;
			
			if (prevDir == 1) {
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT, true);
			}
			else if (prevDir == 2) {
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT, true);
			}

			
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
				if (prevDir == 1)
					sprite->changeAnimation(AIR_RIGHT, false);
				else if (prevDir == 2)
					sprite->changeAnimation(AIR_LEFT, false);
			}
		}
		else {
			falling = true;
			if (prevDir == 1) {
				if (sprite->animation() != AIR_RIGHT) {
					sprite->changeAnimation(AIR_RIGHT, false);
					cout << "entra aqui " << endl;
				}
			}
			else if (prevDir == 2) {
				if (sprite->animation() != AIR_LEFT) {
					sprite->changeAnimation(AIR_LEFT, false);
					cout << "entra aqui" << endl;
				}
			}
			
		}
	}


	if (!bJumping) {
		// playerxleft
		if ((map->getTileInPos(hitboxPos.x / 40, (hitboxPos.y + int(sprite->getSpriteSize().y)) / 20)) == 2) {
			++score;
			map->tileStepped(hitboxPos.x / 40, (hitboxPos.y + int(sprite->getSpriteSize().y)) / 20);
		}
		// playerxright
		else if ((map->getTileInPos((hitboxPos.x + HITBOXOFFSETX) / 40, (hitboxPos.y + int(sprite->getSpriteSize().y)) / 20)) == 2) {
			++score;
			map->tileStepped((hitboxPos.x + HITBOXOFFSETX) / 40, (hitboxPos.y + int(sprite->getSpriteSize().y)) / 20);
		}
	}
	
	if ((map->getTileInPos(hitboxPos.x / 40, (hitboxPos.y + int(sprite->getSpriteSize().y)) / 20)) == 5) {
		this->hit(lives);
	}
	// playerxright
	else if ((map->getTileInPos((hitboxPos.x + HITBOXOFFSETX) / 40, (hitboxPos.y + int(sprite->getSpriteSize().y)) / 20)) == 5) {
		this->hit(lives);
	}
}

void Player::setInvincibility(bool inv) 
{
	invincible = inv;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setStartingPosition(const glm::vec2& pos)
{
	startingPosPlayer = pos;
	posPlayer = pos;
	hitboxPos = glm::vec2(posPlayer.x + HITBOXOFFSETX, posPlayer.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	hitboxPos = glm::vec2(posPlayer.x + HITBOXOFFSETX, posPlayer.y);
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

glm::vec2 Player::getHitBoxPosition()
{
	return hitboxPos;
}

glm::ivec2 Player::getHitBoxSize() const
{
	return hitboxSize;
}

/*int Player::getLives()
{
	return lives;
}

int Player::getScore()
{
	return score;
}
*/

void Player::hit(int& lives) 
{
	if (lives == 0)
		cout << "GAME OVER" << endl;
	else
		--lives;

	falling = false;
	posPlayer = glm::vec2(startingPosPlayer.x, startingPosPlayer.y);
	hitboxPos = glm::vec2(posPlayer.x + HITBOXOFFSETX, posPlayer.y);
	bJumping = false; // para resetear el estado
	sprite->changeAnimation(STAND_RIGHT, true);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::isInvincible()
{
	return invincible;
}