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
	void update(int deltaTime, int& score, int& lives);
	void render();

	void setTileMap(TileMap* tileMap);
	void setStartingPosition (const glm::vec2& pos); // posición de inicio en el mapa actual (para respawn)
	void setPosition(const glm::vec2& pos);
	void setInvincibility(bool inv);

	glm::ivec2 getSize() const;
	glm::vec2 getPosition();
	glm::ivec2 getHitBoxSize() const;
	glm::vec2 getHitBoxPosition();
	//int getLives();
	//int getScore();

	void hit(int& lives);
	bool isInvincible();

private:
	bool bJumping, falling, dead, reappeared;
	bool invincible; // para trucos y potencialmente para justo después de ser golpeado
	glm::ivec2 tileMapDispl, posPlayer, startingPosPlayer, size, hitboxPos, hitboxSize;
	int jumpAngle, startY, deathTime, currentTime, reappearedTime;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	int prevDir;
	// prevDir = 1 -> personaje estaba direccion hacia derecha
	// prevDir = 2 -> personaje estaba direccion hacia izquierda

	enum PlayerAnimations
	{
		STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DIE_LEFT, DIE_RIGHT, AIR_LEFT, AIR_RIGHT
	};

};


#endif // _PLAYER_INCLUDE
