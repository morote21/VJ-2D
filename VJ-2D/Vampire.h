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
	glm::ivec2 tileMapDispl, posVampire, size;
	glm::ivec2 speed; // en principio s�lo para cuando forma de murci�lago
	char currentForm; // 0: humano, 1: murci�lago

	float currentTime; // si llegamos a tiempo m�nimo, deber� iniciar cambio de forma
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	enum SkeletonAnimations
	{
		MOVE_LEFT, MOVE_RIGHT, FLY
	};




};

#endif // _VAMPIRE_INCLUDE