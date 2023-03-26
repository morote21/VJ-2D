#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include <vector>
#include "TexturedQuad.h"
#include "Sprite.h"
#include "Credits.h"
#include "Instructions.h"

class Menu 
{
public:
	Menu();
	~Menu();

	void init();	
	void initShaders();
	void update(int deltaTime);
	void render();

	void setSelectorIndex(int index);
	int getSelectorIndex();
	
private:
	int currentWindow, selectorIndex;

	float currentTime;
	vector<TexturedQuad*> backgroundArray;
	Texture menuWindow, creditsWindow, instructionsWindow;
	Texture spritesheet;

	ShaderProgram texProgram;
	glm::mat4 projection;

	Credits credits;
	Instructions instructions;

	enum BatAnimations
	{
		MENU_BAT
	};

};


#endif // _MENU_INCLUDE
