#ifndef _SCENEMANAGER_INCLUDE
#define _SCENEMANAGER_INCLUDE

#include "Scene.h"
#include "Text.h"
#include <vector>

using namespace std;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void init();
	void update(int deltaTime);
	void render();

	void resetLevels();
	void resetLevel(int level);
	static int getCurrentScene();

private:
	vector<Scene*> sceneArray;
	static int currentScene;

	int lives, score; // para que sea persistente entre niveles
	Text HUDText; // para HUD
};


#endif