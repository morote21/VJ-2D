#include <iostream>
#include <cmath>
#include <string>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
using namespace std;

#define SCREEN_X 0
#define SCREEN_Y 40

#define SIZEITEMS_X 16
#define SIZEITEMS_Y 16

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


void Scene::init(string mapPath) // We may want to modify this so that it sets up different levels...
{
	initShaders();
	keyCollected = false;
	doorEntered = false;
	timer = 60; // Debería ser diferente entre niveles...
	pause = false;
	timeState = 2;
	map = TileMap::createTileMap(mapPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram); // for specific level: maybe have object map?

	vector<glm::ivec2> itemsPositions = map->getItemsPositions();	// siempre tendra 4 posiciones, ya que habra 1 objeto de cada por mapa (si el objeto aparece o no al final sera opcional, a excepcion de la llave)
	
	glm::vec2 geom[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	levelBackground.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGB);
	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setStartingPosition(glm::vec2(map->getPlayerInitPos().x, map->getPlayerInitPos().y)); // ¿LIGADO AL NIVEL?
	player->setTileMap(map);
	

	random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, itemsPositions.size()-1);
	int indexItem1, indexItem2, indexItem3, indexItem4;
	// asignamos indices de posiciones random a los objetos y evitamos que se repitan entre objetos 

	indexItem1 = dist(mt);

	indexItem2 = dist(mt);
	while (indexItem2 == indexItem1)
		indexItem2 = dist(mt);

	indexItem3 = dist(mt);
	while (indexItem3 == indexItem1 || indexItem3 == indexItem2)
		indexItem3 = dist(mt);

	indexItem4 = dist(mt);
	while (indexItem4 == indexItem1 || indexItem4 == indexItem2 || indexItem4 == indexItem3)
		indexItem4 = dist(mt);


	key.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(int(itemsPositions[indexItem1].x) + int((40 - SIZEITEMS_X) / 2), int(itemsPositions[indexItem1].y) + int((40 - SIZEITEMS_Y) / 2)), texProgram); // this could be on the object map
	
	door.init(glm::ivec2(SCREEN_X, SCREEN_Y), map->getDoorPos(), texProgram); //

	testGem.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem2].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem2].y + int((40 - SIZEITEMS_Y) / 2)), texProgram);

	testLife.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem3].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem3].y + int((40 - SIZEITEMS_Y) / 2)), texProgram);

	testWatch.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem4].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem4].y + int((40 - SIZEITEMS_Y) / 2)), texProgram);

	for (int i = 0; i < map->getSoldiersPositions().size(); i++) {
		Skeleton *testSkel = new Skeleton();
		testSkel->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		testSkel->setPosition(glm::vec2(map->getSoldiersPositions()[i].x, map->getSoldiersPositions()[i].y + 2)); // 1a plataforma desde abajo, sin paredes
		testSkel->setTileMap(map);
		testSkelArray.push_back(testSkel);
	}
	//testSkel.setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()+5 )); // suelo inferior, el rodeado por paredes

	for (int i = 0; i < map->getAliensPositions().size(); i++) {
		Vampire *testVamp = new Vampire();
		testVamp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		testVamp->setPosition(glm::vec2(map->getAliensPositions()[i].x, map->getAliensPositions()[i].y + 5)); // 1a plataforma desde abajo, sin paredes
		testVamp->setTileMap(map);
		testVampArray.push_back(testVamp);
	}
	//testVamp.setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()+5 )); // suelo inferior, el rodeado por paredes

	pauseMenu.init();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::update(int deltaTime, int& lives, int& score)
{
	// Pause game 
	// Mas adelante cambiar p por la tecla escape, y entonces hacer que aparezca un menu de opciones
	// con resume, quit o algunas otras opciones como size de la ventana


	if (!pauseMenu.isPaused()) {

		if (Game::instance().getKey(27) && !Game::instance().getKeyAlreadyPressing(27)) {
			Game::instance().setKeyAlreadyPressing(27);
			pauseMenu.setPaused(true);
		}
		// Make key appear
		if (Game::instance().getKey('k') && !Game::instance().getKeyAlreadyPressing('k')) {
			Game::instance().setKeyAlreadyPressing('k');
			map->setAllSteppedTiles();
		}

		// Toggle invincibility
		if (Game::instance().getKey('g') && !Game::instance().getKeyAlreadyPressing('g')) {
			Game::instance().setKeyAlreadyPressing('g');
			player->setInvincibility(!player->isInvincible());
		}

		currentTime += deltaTime;
		if (currentTime >= 1000) {
			currentTime = 0;
			timer -= 1;
			if (timer == 0)
				cout << "Time Up!" << endl;
		}
		player->update(deltaTime, score, lives);
		key.update(deltaTime);
		door.update(deltaTime, keyCollected);

		testGem.update(deltaTime);
		testLife.update(deltaTime);
		testWatch.update(deltaTime, timeState);
		
		if (timeState == 2) { // para evitar que se muevan si no se mueve todo
			for (int i = 0; i < testSkelArray.size(); i++) {
				testSkelArray[i]->update(deltaTime);
			}
			for (int i = 0; i < testVampArray.size(); i++) {
				testVampArray[i]->update(deltaTime);
			}
		}
		
		// Colisión con Player de los enemigos (lo dejo aquí, porque si lo hacemos bien, podemos reducir el número de checkeos considerablemente)
		if (!player->isInvincible()) { // tal vez queramos algo más complejo, como canBeHit(), para considerar animaciones
			for (int i = 0; i < testSkelArray.size(); i++) {
				if (samePosition(testSkelArray[i]->getPosition(), testSkelArray[i]->getSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
					player->hit(lives);
			}
			for (int i = 0; i < testVampArray.size(); i++) {
				if (samePosition(testVampArray[i]->getPosition(), testVampArray[i]->getSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
					player->hit(lives);
			}
			//if (samePosition(testSkel.getPosition(), testSkel.getSize(), player->getHitBoxPosition(), player->getHitBoxSize())
			// || samePosition(testVamp.getPosition(), testVamp.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
			//	player->hit(lives);
		}


		// Colisión con objetos coleccionables (para recogerlos)
		if (testGem.isVisible() && samePosition(testGem.getPosition(), testGem.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()) ){
			testGem.setVisibility(false);
			score += 100;
		}

		if (testLife.isVisible() && samePosition(testLife.getPosition(), testLife.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()) ) {
			testLife.setVisibility(false);
			++lives;
		}

		if (testWatch.isVisible() && samePosition(testWatch.getPosition(), testWatch.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
			testWatch.activate(timeState);

		if (timer == 0) {
			player->hit(lives);
			timer = 60;
		}

		if (!keyCollected && samePosition(key.getPosition(), key.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()) && map->keyAppeared()) {
			cout << "Key collected" << endl;
			keyCollected = true;
		}
		if (keyCollected && samePosition(door.getPosition(), door.getSize(), player->getHitBoxPosition(), player->getHitBoxSize())) {
			cout << "Door entered" << endl;
			doorEntered = true;
		}
	}
	else 
		pauseMenu.update(deltaTime);
	
}

int Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(levelBackground);
	map->render(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	if (map->keyAppeared() && !keyCollected)
		key.render();

	testGem.render();
	testLife.render();
	testWatch.render();

	door.render();
	
	for (int i = 0; i < testSkelArray.size(); i++) {
		testSkelArray[i]->render();
		
	}
	
	for (int i = 0; i < testVampArray.size(); i++) {
		testVampArray[i]->render();
	}

	player->render();

	if (pauseMenu.isPaused()) {
		pauseMenu.render();
	}

	return timer;
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


bool Scene::samePosition(glm::vec2 objPos, glm::vec2 objSize, glm::vec2 playerPos, glm::vec2 playerSize)
{
	if (objPos.x + objSize.x < playerPos.x || objPos.x > playerPos.x + playerSize.x)
		return false;
	else if (objPos.y + objSize.y < playerPos.y || objPos.y > playerPos.y + playerSize.y)
		return false;
	return true;
}

bool Scene::getDoorEntered() {
	return doorEntered;
}