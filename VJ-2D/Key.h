#ifndef _KEY_INCLUDE
#define _KEY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Key
{
public:
	~Key();

	void init(const glm::ivec2& tileMapPos, glm::ivec2 itemPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	glm::ivec2 getSize() const;
	glm::vec2 getPosition();


private:
	glm::vec2 position;
	glm::ivec2 size;
	Texture spritesheet;
	Sprite* sprite;

	enum KeyAnimations
	{
		FLOATING
	};

};


#endif 

