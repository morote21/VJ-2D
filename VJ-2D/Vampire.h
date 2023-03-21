#ifndef _VAMPIRE_INCLUDE
#define _VAMPIRE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

// Class that represents the Vampire enemy


class Vampire
{
public:

	~Vampire();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getSize() const;
	glm::vec2 getPosition();

private:
	glm::ivec2 tileMapDispl, posVampire, size; // size2 para forma murciélago?

	glm::ivec2 speed; // en principio sólo para cuando forma de murciélago
	//char currentForm; // 0: humano, 1: murciélago <- de momento no se usa

	float currentTime; // si llegamos a tiempo mínimo, deberá iniciar cambio de forma
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	enum SkeletonAnimations
	{
		MOVE_LEFT, MOVE_RIGHT, FLY
	};




};

#endif // _VAMPIRE_INCLUDE