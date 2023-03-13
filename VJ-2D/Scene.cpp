#include <iostream>
#include <cmath>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
using namespace std;

#define SCREEN_X 0
#define SCREEN_Y 40

#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 18


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init() // We may want to modify this so that it sets up different levels...
{
	initShaders();
	keyCollected = false;
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram); // for specific level: maybe have object map?
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()));
	player->setTileMap(map);
	key.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram); // this could be on the object map
	door.init(glm::ivec2(SCREEN_X, SCREEN_Y), map->getDoorPos(), texProgram); //

	testSkel.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	testSkel.setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), (INIT_PLAYER_Y_TILES-3) * map->getTileSizeY() + 8)); // 1a plataforma desde abajo, sin paredes
	//testSkel.setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()+8 )); // suelo inferior, el rodeado por paredes
	testSkel.setTileMap(map);


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (!HUDText.init("fonts/dungeon_font.ttf"))
		cout << "Could not load HUD font" << endl;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	key.update(deltaTime);
	door.update(deltaTime, keyCollected);

	testSkel.update(deltaTime);

	if (!keyCollected && samePosition(key.getPos(), key.getSize(), player->getPosition(), player->getSize()) && map->keyAppeared()) {
		keyCollected = true;
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	if (map->keyAppeared() && !keyCollected)
		key.render();
	door.render();
	testSkel.render();

	player->render();
	
	// render vidas del jugador
	HUDText.render(to_string(player->getLives()), glm::vec2(50, 33), 40, glm::vec4(0, 0, 0, 1));
	HUDText.render("Score: ", glm::vec2(200, 33), 40, glm::vec4(0, 0, 0, 1));
	HUDText.render(to_string(player->getScore()), glm::vec2(330, 33), 40, glm::vec4(0, 0, 0, 1));

}

void Scene::initShaders()
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


bool Scene::samePosition(glm::vec2 e1pos, glm::vec2 e1size, glm::vec2 e2pos, glm::vec2 e2size)
{
	if (e1pos.x + e1size.x < e2pos.x || e1pos.x > e2pos.x + e2size.x)
		return false;
	else if (e1pos.y + e1size.y < e2pos.y || e1pos.y > e2pos.y + e2size.y)
		return false;
	return true;
}

