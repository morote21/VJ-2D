#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Key.h"
#include "Door.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	bool samePosition(glm::vec2 e1pos, glm::vec2 e1size, glm::vec2 e2pos, glm::vec2 e2size);

private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	Key key;
	Door door;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool keyCollected;
};


#endif // _SCENE_INCLUDE