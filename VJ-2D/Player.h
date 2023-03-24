#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:

	~Player();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setStartingPosition (const glm::vec2& pos); // posici�n de inicio en el mapa actual (para respawn)
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getSize() const;
	glm::vec2 getPosition();
	int getLives();
	int getScore();

	void hit();
	bool isInvincible();

private:
	bool bJumping, alreadyInGround;
	bool invincible; // para trucos y potencialmente para justo despu�s de ser golpeado
	glm::ivec2 tileMapDispl, posPlayer, startingPosPlayer, size;
	int jumpAngle, startY, lives, score;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	enum PlayerAnimations
	{
		STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT, JUMP, FALL
	};

};


#endif // _PLAYER_INCLUDE
