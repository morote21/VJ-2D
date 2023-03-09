#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Door
{
public:

	void init(const glm::ivec2& tileMapPos, glm::ivec2 doorMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, bool opened);
	void render();

	glm::ivec2 getSize() const;
	glm::vec2 getPos();


private:
	glm::vec2 position;
	glm::ivec2 size;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool alreadyOpened;

};


#endif 

