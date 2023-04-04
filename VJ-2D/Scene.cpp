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
	this->mapPath = mapPath;
	initShaders();
	keyCollected = false;
	doorEntered = false;
	timer = 60; // Debería ser diferente entre niveles...
	pause = false;
	stageCompleted = false;
	playedOne = playedTwo = playedThree = playedStart = false;
	timeState = 2;
	map = TileMap::createTileMap(mapPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram); // for specific level: maybe have object map?

	vector<glm::ivec2> itemsPositions = map->getItemsPositions();	// siempre tendra 4 posiciones, ya que habra 1 objeto de cada por mapa (si el objeto aparece o no al final sera opcional, a excepcion de la llave)
	
	glm::vec2 geom[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	levelBackground.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGB);
	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setStartingPosition(glm::vec2(map->getPlayerInitPos().x, map->getPlayerInitPos().y + 2)); // ¿LIGADO AL NIVEL?
	player->setTileMap(map);
	
	
	countdownTexture.loadFromFile("images/countdown.png", TEXTURE_PIXEL_FORMAT_RGBA);
	countdown = Sprite::createSprite(glm::vec2(120, 150), glm::vec2(1 / 3.f, 1.f), &countdownTexture, &texProgram); // para el quad representado (+ tamaño frame)
	countdown->setPosition(glm::vec2(260, 180));
	countdown->setNumberAnimations(3);	// hay que poner el numero de animaciones de Animations

	countdown->setAnimationSpeed(THREE, 0);
	countdown->addKeyframe(THREE, glm::vec2(0.f, 0.f));

	countdown->setAnimationSpeed(TWO, 0);
	countdown->addKeyframe(TWO, glm::vec2(1 / 3.f, 0.f));

	countdown->setAnimationSpeed(ONE, 0);
	countdown->addKeyframe(ONE, glm::vec2(2 / 3.f, 0.f));

	countdown->changeAnimation(THREE, false);


	stageCompletedTexture.loadFromFile("images/missioncomplete.png", TEXTURE_PIXEL_FORMAT_RGBA);
	stageCompleteSprite = Sprite::createSprite(glm::vec2(550, 100), glm::vec2(1.f, 1/2.f), &stageCompletedTexture, &texProgram);
	stageCompleteSprite->setPosition(glm::vec2(50, 200));

	stageCompleteSprite->setNumberAnimations(2);

	stageCompleteSprite->setAnimationSpeed(ONE, 0);
	stageCompleteSprite->addKeyframe(ONE, glm::vec2(0.f, 0.f));

	stageCompleteSprite->setAnimationSpeed(TWO, 0);
	stageCompleteSprite->addKeyframe(TWO, glm::vec2(0.f, 1/2.f));

	stageCompleteSprite->changeAnimation(ONE, true);

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
	

	dist = std::uniform_int_distribution<int>(2, 58);
	
	cout << mapPath << endl;
	gemSec = dist(mt);
	cout << "segundo aparicion gema: " << gemSec << endl;
	testGem.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem2].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem2].y + int((40 - SIZEITEMS_Y) / 2)), texProgram);

	lifeSec = dist(mt);
	cout << "segundo aparicion life: " << lifeSec << endl;
	testLife.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem3].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem3].y + int((40 - SIZEITEMS_Y) / 2)), texProgram);

	watchSec = dist(mt);
	cout << "segundo aparicion watch: " << watchSec << endl;
	testWatch.init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem4].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem4].y + int((40 - SIZEITEMS_Y) / 2)), texProgram);

	testSkelArray = vector<Skeleton*>();
	for (int i = 0; i < map->getSoldiersPositions().size(); i++) {
		Skeleton *testSkel = new Skeleton();
		testSkel->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		testSkel->setPosition(glm::vec2(map->getSoldiersPositions()[i].x, map->getSoldiersPositions()[i].y + 2)); // 1a plataforma desde abajo, sin paredes
		testSkel->setTileMap(map);
		testSkelArray.push_back(testSkel);
	}
	//testSkel.setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()+5 )); // suelo inferior, el rodeado por paredes

	testVampArray = vector<Vampire*>();
	for (int i = 0; i < map->getAliensPositions().size(); i++) {
		Vampire *testVamp = new Vampire();
		testVamp->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		testVamp->setPosition(glm::vec2(map->getAliensPositions()[i].x, map->getAliensPositions()[i].y + 5)); // 1a plataforma desde abajo, sin paredes
		testVamp->setTileMap(map);
		testVampArray.push_back(testVamp);
	}
	//testVamp.setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizeX(), INIT_PLAYER_Y_TILES * map->getTileSizeY()+5 )); // suelo inferior, el rodeado por paredes
	
	testMummyArray = vector<Mummy*>();
	for (int i = 0; i < map->getMummiesPositions().size(); i++) {
		Mummy* testMummy = new Mummy();
		testMummy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		testMummy->setPosition(glm::vec2(map->getMummiesPositions()[i].x, map->getMummiesPositions()[i].y + 2)); // 1a plataforma desde abajo, sin paredes
		testMummy->setTileMap(map);
		testMummyArray.push_back(testMummy);
	}
	

	pauseMenu.init();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	countdownTimer = 0.f;
	completeTimer = 0.f;
	countdownDone = false;
	playedMissionComplete = false;
}

void Scene::update(int deltaTime, int& lives, int& score)
{
	// Pause game 
	// Mas adelante cambiar p por la tecla escape, y entonces hacer que aparezca un menu de opciones
	// con resume, quit o algunas otras opciones como size de la ventana
	if (!countdownDone) {
		countdownTimer += deltaTime;
		if (countdownTimer <= 1000.f) {
			countdown->changeAnimation(THREE, false);
			if (!playedOne) {
				SoundManager::instance().playThree();
				playedOne = true;
			}
		}
		else if (countdownTimer <= 2000.f) {
			countdown->changeAnimation(TWO, false);
			if (!playedTwo) {
				SoundManager::instance().playTwo();
				playedTwo = true;
			}
		}
		else if (countdownTimer <= 3000.f) {
			countdown->changeAnimation(ONE, false);
			if (!playedThree) {
				SoundManager::instance().playOne();
				playedThree = true;
			}
		}
		else {
			countdownDone = true;
			if (!playedStart) {
				SoundManager::instance().playStart();
				playedStart = true;
			}
		}
	}
	else if (doorEntered) {
		completeTimer += deltaTime;
		if (!playedMissionComplete) {
			SoundManager::instance().playMissionComplete();
			playedMissionComplete = true;
		}
		if (timer > 0) {	//se hace asi para el efecto de sumar time al score 
			for (int j = 0; j < 10000; j++);
			timer -= 1;
			score += 1;
		}

		if (completeTimer >= 3000.f)
			stageCompleted = true;
	}
	else {
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

				for (int i = 0; i < testMummyArray.size(); i++) {
					testMummyArray[i]->update(deltaTime, player);
				}
			}
		
			// Colisión con Player de los enemigos (lo dejo aquí, porque si lo hacemos bien, podemos reducir el número de checkeos considerablemente) (saliendo del bucle después de un hit())
			if (!player->isInvincible()) { // tal vez queramos algo más complejo, como canBeHit(), para considerar animaciones
				for (int i = 0; i < testSkelArray.size(); i++) {
					if (samePosition(testSkelArray[i]->getPosition(), testSkelArray[i]->getSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
						player->hit(lives);
				}
				for (int i = 0; i < testVampArray.size(); i++) {
					if (samePosition(testVampArray[i]->getHitboxPosition(), testVampArray[i]->getHitboxSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
						player->hit(lives);
				}
				//if (samePosition(testSkel.getPosition(), testSkel.getSize(), player->getHitBoxPosition(), player->getHitBoxSize())
				// || samePosition(testVamp.getPosition(), testVamp.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()))
				//	player->hit(lives);
				for (int i = 0; i < testMummyArray.size(); i++) {
					if (testMummyArray[i]->poisonExists() && samePosition(testMummyArray[i]->getPoisonPosition(), testMummyArray[i]->getPoisonSize(), player->getHitBoxPosition(), player->getHitBoxSize())
						|| samePosition(testMummyArray[i]->getPosition(), testMummyArray[i]->getSize(), player->getHitBoxPosition(), player->getHitBoxSize())) {
						SoundManager::instance().playPoison();
						player->hit(lives);
					}
				}
			}

			
			if (lifeSec != -1) {
				if (timer == lifeSec) {
					testLife.setVisibility(true);
					lifeSec = -1;
				}
			}

			if (gemSec != -1) {
				if (timer == gemSec) {
					testGem.setVisibility(true);
					gemSec = -1;
				}
			}

			if (watchSec != -1) {
				if (timer == watchSec) {
					testWatch.setVisibility(true);
					watchSec = -1;
				}
			}

			// Colisión con objetos coleccionables (para recogerlos)
			if (testGem.isVisible() && samePosition(testGem.getPosition(), testGem.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()) ){
				testGem.setVisibility(false);
				score += 100;
				SoundManager::instance().playItem();
			}

			if (testLife.isVisible() && samePosition(testLife.getPosition(), testLife.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()) ) {
				testLife.setVisibility(false);
				++lives;
				SoundManager::instance().playItem();
			}

			if (testWatch.isVisible() && samePosition(testWatch.getPosition(), testWatch.getSize(), player->getHitBoxPosition(), player->getHitBoxSize())) {
				testWatch.activate(timeState);
				SoundManager::instance().playItem();
			}

			if (timer == 0) {
				player->hit(lives);
				timer = 60;
			}

			if (!keyCollected && samePosition(key.getPosition(), key.getSize(), player->getHitBoxPosition(), player->getHitBoxSize()) && map->keyAppeared()) {
				keyCollected = true;
				SoundManager::instance().playDoor();
			}
			if (keyCollected && samePosition(door.getPosition(), door.getSize(), player->getHitBoxPosition(), player->getHitBoxSize())) {
				doorEntered = true;
			}
		}
		else 
			pauseMenu.update(deltaTime);
	}

	
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
	
	for (int i = 0; i < testMummyArray.size(); i++) {
		testMummyArray[i]->render();

	}

	for (int i = 0; i < testVampArray.size(); i++) {
		testVampArray[i]->render();
	}

	if (!doorEntered)
		player->render();

	if (!countdownDone) {
		countdown->render();
	}

	if (doorEntered) {
		if (!stageCompleted) {
			stageCompleteSprite->render();
			if (completeTimer >= 1000) {
				if (stageCompleteSprite->animation() == ONE) {
					stageCompleteSprite->changeAnimation(TWO, true);
				}
			}
		}
	}

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

bool Scene::getStageCompleted() {
	return stageCompleted;
}

void Scene::resetLevel() {
	keyCollected = false;
	doorEntered = false;
	timer = 60; // Debería ser diferente entre niveles...
	pause = false;
	pauseMenu.setPaused(false);
	stageCompleted = false;
	playedOne = playedTwo = playedThree = playedStart = false;
	timeState = 2;

	map = TileMap::createTileMap(mapPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram); // for specific level: maybe have object map?

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setStartingPosition(glm::vec2(map->getPlayerInitPos().x, map->getPlayerInitPos().y + 2)); // ¿LIGADO AL NIVEL?
	player->setTileMap(map);

	vector<glm::ivec2> itemsPositions = map->getItemsPositions();	// siempre tendra 4 posiciones, ya que habra 1 objeto de cada por mapa (si el objeto aparece o no al final sera opcional, a excepcion de la llave)

	random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, itemsPositions.size() - 1);
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

	key.setPosition(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(int(itemsPositions[indexItem1].x) + int((40 - SIZEITEMS_X) / 2), int(itemsPositions[indexItem1].y) + int((40 - SIZEITEMS_Y) / 2)));
	door.resetAnimation(glm::ivec2(SCREEN_X, SCREEN_Y), map->getDoorPos());

	dist = std::uniform_int_distribution<int>(2, 58);

	gemSec = dist(mt);
	cout << "segundo aparicion gema: " << gemSec << endl;
	testGem.setPosition(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem2].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem2].y + int((40 - SIZEITEMS_Y) / 2)));

	lifeSec = dist(mt);
	cout << "segundo aparicion life: " << lifeSec << endl;
	testLife.setPosition(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem3].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem3].y + int((40 - SIZEITEMS_Y) / 2)));

	watchSec = dist(mt);
	cout << "segundo aparicion watch: " << watchSec << endl;
	testWatch.setPosition(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(itemsPositions[indexItem4].x + int((40 - SIZEITEMS_X) / 2), itemsPositions[indexItem4].y + int((40 - SIZEITEMS_Y) / 2)));


	for (int i = 0; i < map->getSoldiersPositions().size(); i++) {
		testSkelArray[i]->setPosition(glm::vec2(map->getSoldiersPositions()[i].x, map->getSoldiersPositions()[i].y + 2)); // 1a plataforma desde abajo, sin paredes
		testSkelArray[i]->setTileMap(map);
		testSkelArray[i]->resetAnimation();
	}

	for (int i = 0; i < map->getAliensPositions().size(); i++) {
		testVampArray[i]->setPosition(glm::vec2(map->getAliensPositions()[i].x, map->getAliensPositions()[i].y + 5)); // 1a plataforma desde abajo, sin paredes
		testVampArray[i]->setTileMap(map);
		testVampArray[i]->resetAnimation();
	}

	for (int i = 0; i < map->getMummiesPositions().size(); i++) {
		testMummyArray[i]->setPosition(glm::vec2(map->getMummiesPositions()[i].x, map->getMummiesPositions()[i].y + 2)); // 1a plataforma desde abajo, sin paredes
		testMummyArray[i]->setTileMap(map);
		testMummyArray[i]->resetAnimation();
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	countdownTimer = 0.f;
	completeTimer = 0.f;
	countdownDone = false;
	playedMissionComplete = false;
}