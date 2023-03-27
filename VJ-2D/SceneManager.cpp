#include "SceneManager.h"
#include "Game.h"
#include <string>
#include <iostream>
using namespace std;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() 
{
	for (Scene* scene : sceneArray)
		delete scene;
}

int SceneManager::currentScene;

void SceneManager::init()
{
	Scene *scene1 = new Scene(), *scene2 = new Scene();
	scene1->init("levels/level01.txt"); scene2->init("levels/level02.txt");
	sceneArray.push_back(scene1); sceneArray.push_back(scene2);
	currentScene = 0;

	lives = 3;
	score = 0;

	if (!HUDText.init("fonts/dungeon_font.ttf"))
		cout << "Could not load HUD font" << endl;
}

void SceneManager::update(int deltaTime) // ??? (keys)
{
	// En todos los casos, reseteamos el nivel actual, y nos movemos al correspondiente
	//(¿resetear vidas y score si hacemos lo de las teclas? porque estamos forzando cambio de nivel sin completarlo...)
	if (Game::instance().getKey('1') && !Game::instance().getKeyAlreadyPressing('1')) {
		Game::instance().setKeyAlreadyPressing('1');
		string levelString = "levels/level0";
		levelString += to_string(currentScene+1);
		levelString += ".txt";
		sceneArray[currentScene]->init(levelString);
		currentScene = 0;
	}

	if (Game::instance().getKey('2') && !Game::instance().getKeyAlreadyPressing('2')) {
		Game::instance().setKeyAlreadyPressing('2');
		string levelString = "levels/level0";
		levelString += to_string(currentScene+1);
		levelString += ".txt";
		sceneArray[currentScene]->init(levelString);
		currentScene = 1;
	}
	
	if (sceneArray[currentScene]->getDoorEntered()) {
		// Reseteamos el nivel actual y pasamos al siguiente
		string levelString = "levels/level0";
		levelString += to_string(currentScene + 1);
		levelString += ".txt";
		sceneArray[currentScene]->init(levelString);

		currentScene += 1;
		if (currentScene == sceneArray.size()) {
			Game::instance().setStart(false);
			currentScene = 0;
		}
	}

	sceneArray[currentScene]->update(deltaTime, lives, score);
}

void SceneManager::render()
{
	int timer = sceneArray[currentScene]->render();

	// render de HUD aquí
	HUDText.render(to_string(lives), glm::vec2(50, 33), 40, glm::vec4(1, 1, 1, 1));
	HUDText.render("Score: ", glm::vec2(200, 33), 40, glm::vec4(1, 1, 1, 1));
	HUDText.render(to_string(score), glm::vec2(330, 33), 40, glm::vec4(1, 1, 1, 1));
	HUDText.render("Time: ", glm::vec2(450, 33), 40, glm::vec4(1, 1, 1, 1));
	HUDText.render(to_string(timer), glm::vec2(570, 33), 40, glm::vec4(1, 1, 1, 1));
}

void SceneManager::resetLevels() 
{
	for (int i = 1; i <= sceneArray.size(); i++) {
		string levelString = "levels/level0";
		levelString += to_string(i);
		levelString += ".txt";
		sceneArray[i - 1]->init(levelString);
	}
	currentScene = 0;
}

void SceneManager::resetLevel(int level)
{
	string levelString = "levels/level0";
	levelString += to_string(level);
	levelString += ".txt";
	sceneArray[level - 1]->init(levelString);
	currentScene = level;
}

int SceneManager::getCurrentScene()
{
	return currentScene;
}