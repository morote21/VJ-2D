#include "SceneManager.h"
#include "Game.h"
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
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
	initShaders();
	Scene *scene1 = new Scene(), *scene2 = new Scene();
	scene1->init("levels/level01.txt"); scene2->init("levels/level02.txt");
	sceneArray.push_back(scene1); sceneArray.push_back(scene2);
	currentScene = 0;

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, 40) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.f, 1.f) };
	hudBackgroundTexture.loadFromFile("images/hudbackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	hudBackground = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	
	/*
	for (int i = 0; i < 10; i++) {
		texCoords[0] = glm::vec2(i/10, 0.0f); texCoords[1] = glm::vec2((i+1)/10, 1.f);
		numbers.push_back(TexturedQuad::createTexturedQuad(geom, texCoords, texProgram));
	}
	*/
	
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

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

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	hudBackground->render(hudBackgroundTexture);

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

void SceneManager::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}