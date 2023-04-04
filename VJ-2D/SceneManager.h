#ifndef _SCENEMANAGER_INCLUDE
#define _SCENEMANAGER_INCLUDE

#include "Scene.h"
#include "Text.h"
#include "TexturedQuad.h"
#include <vector>

using namespace std;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void init();
	void initShaders();
	void update(int deltaTime);
	void render();

	void resetLevels();
	void resetLevel(int level);
	static int getCurrentScene();

private:
	vector<Scene*> sceneArray;
	static int currentScene;
	TexturedQuad* hudBackground;
	vector<TexturedQuad*> numbers;
	Texture hudBackgroundTexture, numbersTexture, heartTexture, scoreTexture, missionTexture, gameoverTexture, alloverTexture;
	TexturedQuad* heart, *scoreHud, * missionHud, * gameoverQuad, * alloverQuad;
	int lives, score, finishTime; // para que sea persistente entre niveles
	Text HUDText; // para HUD
	glm::mat4 projection;
	ShaderProgram texProgram;	// pasarlo a la clase Game mas adelante
	bool gameoverPlayed, gameover, showScore, allover, alloverPlayed;

	enum VampireAnimations
	{
		HEART_ANIMATION
	};

};


#endif