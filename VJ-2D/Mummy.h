#ifndef _MUMMY_INCLUDE
#define _MUMMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

// Class that represents the Mummy enemy

class Mummy
{
public:

	~Mummy();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::vec2 playerPos, glm::vec2 playerSize);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getSize() const;
	glm::vec2 getPosition();
	glm::ivec2 getPoisonSize() const;
	glm::vec2 getPoisonPosition();

	bool poisonExists();

private:
	glm::ivec2 tileMapDispl, posMummy, size, posPoison, sizePoison; // poison es el gas que echa la momia si se cruza con el personaje
	Texture spritesheet, poisonSpritesheet;
	Sprite* sprite;
	Sprite* poisonSprite;
	TileMap* map;

	bool poisonVisible;

	enum MummyAnimations
	{
		MOVE_LEFT, MOVE_RIGHT, BREATHE_LEFT, BREATHE_RIGHT
	};

	enum PoisonAnimations
	{
		POISON_LEFT, POISON_RIGHT
	};

};

#endif // _MUMMY_INCLUDE

