#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}

public:
	static Game& instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();
	void stopGame();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	void setStart(bool state);
	bool getStart();
	void resetMap();

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void setKeyAlreadyPressing(int key);
	void setSpecialKeyAlreadyPressing(int key);
	bool getKeyAlreadyPressing(int key) const;
	bool getSpecialKeyAlreadyPressing(int key) const;


private:
	bool bPlay, start;                       // Continue to play game?
	Scene scene;						  // Scene to render
	Menu menu;
	bool keys[256], specialKeys[256]; // Store key states so 
	// we can have access at any time
	bool keysAlreadyPressing[256], specialKeysAlreadyPressing[256];
	int level;
	

};


#endif // _GAME_INCLUDE


