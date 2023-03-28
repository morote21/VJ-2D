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

Player::~Player()
{
	sprite->free();
	map->free();
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
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
	sprite->setAnimationSpeed(JUMP, 4);
	for (float i = 0; i < 5; i++) {
		sprite->addKeyframe(JUMP, glm::vec2(i / 16.f, 2 / 8.f));
	}
	
	sprite->changeAnimation(STAND_RIGHT, true);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime, int& score)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT, true);
		posPlayer.x -= PLAYER_SPEED;
		if (map->collisionMoveLeft(posPlayer, size))
		{
			posPlayer.x += PLAYER_SPEED;
			sprite->changeAnimation(STAND_LEFT, true);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT, true);
		posPlayer.x += PLAYER_SPEED;
		if (map->collisionMoveRight(posPlayer, size))
		{
			posPlayer.x -= PLAYER_SPEED;
			sprite->changeAnimation(STAND_RIGHT, true);
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
			posPlayer.y = startY;
		}
		else
		{	
			float angle = sin(3.14159f * jumpAngle / 180.f);
			posPlayer.y = int(startY - JUMP_HEIGHT * angle);
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, size, &posPlayer.y);
		}
		
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if (map->collisionMoveDown(posPlayer, size, &posPlayer.y))
		{
			// cuando este todo acabado intentar hacer animacion de salto 
			// teniendo en cuenta las direcciones desde donde salta

			//if (!alreadyInGround)
			//	sprite->changeAnimation(JUMP, false);
			//alreadyInGround = true;
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
				//sprite->changeAnimation(JUMP, false);
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
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


	posPlayer = startingPosPlayer;
	bJumping = false; // para resetear el estado
	sprite->changeAnimation(STAND_RIGHT, true);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::isInvincible()
{
	return invincible;
}