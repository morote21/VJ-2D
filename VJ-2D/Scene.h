#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Key.h"
#include "Door.h"
#include "Skeleton.h"
#include "Vampire.h"
#include "Gem.h"

#include "Text.h"
#include "Pause.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


class Scene
{

public:
	Scene();
	~Scene();

	void init(string mapPath);
	void update(int deltaTime, int& lives, int& score);
	int render(); // devuelve el timer para mostrarlo en SceneManager

	bool samePosition(glm::vec2 e1pos, glm::vec2 e1size, glm::vec2 e2pos, glm::vec2 e2size);
	bool getDoorEntered();

private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	TexturedQuad* background;
	Texture levelBackground;
	Key key;
	Door door;
	Pause pauseMenu;
	Skeleton testSkel;
	Vampire testVamp;

	Gem testGem; // en principio bastará uno de cada de los coleccionables, porque no habrá 2 iguales a la vez (¿podemos borrarlos y cargarlos como con Player, tal vez?)

	ShaderProgram texProgram;
	float currentTime; // level time
	glm::mat4 projection;
	bool keyCollected, pause, doorEntered;
	int timer;
	//Text HUDText; // Is this unnecessary? (probably, just one)
};


#endif // _SCENE_INCLUDE