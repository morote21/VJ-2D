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
#include "Mummy.h"
#include "Gem.h"
#include "ExtraLife.h"
#include "Stopwatch.h"

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

	static bool samePosition(glm::vec2 e1pos, glm::vec2 e1size, glm::vec2 e2pos, glm::vec2 e2size);
	bool getDoorEntered();
	bool getStageCompleted();

private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	TexturedQuad* background;
	Texture levelBackground, countdownTexture, stageCompletedTexture;
	Key key;
	Door door;
	Pause pauseMenu;
	vector<Skeleton*> testSkelArray;
	vector<Vampire*> testVampArray;

	Mummy testMummy;

	Sprite* countdown, *stageCompleteSprite;

	Gem testGem; // en principio bastará uno de cada de los coleccionables, porque no habrá 2 iguales a la vez (¿podemos borrarlos y cargarlos como con Player, tal vez?)
	ExtraLife testLife;
	Stopwatch testWatch;


	ShaderProgram texProgram;
	float currentTime; // level time
	glm::mat4 projection;
	bool keyCollected, playedMissionComplete, pause, doorEntered, stageCompleted, countdownDone, playedOne, playedTwo, playedThree, playedStart;
	int timer, countdownTimer, completeTimer;
	char timeState; // 0: objetos congelados (debería tambien timer), 1: enemigos congelados, 2: todo en marcha

	//Text HUDText; // Is this unnecessary? (probably, just one)
	enum CountdownAnimations
	{
		ONE, TWO, THREE
	};
};


#endif // _SCENE_INCLUDE