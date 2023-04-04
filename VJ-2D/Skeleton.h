#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

// Class that represents the Skeleton enemy

class Skeleton
{
public:

	~Skeleton();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void resetAnimation();

	glm::ivec2 getSize() const;
	glm::vec2 getPosition();

private:
	glm::ivec2 tileMapDispl, posSkeleton, size;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	enum SkeletonAnimations
	{
		MOVE_LEFT, MOVE_RIGHT
	};

};

#endif // _SKELETON_INCLUDE

