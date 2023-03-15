#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
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
	// selector index:
	// play -> 0
	// instructions -> 1
	// credits -> 2
	
	// current window:
	// menu -> 0
	// instructions -> 1
	// credits -> 2

	float currentTime;
	TexturedQuad* background;
	Texture menuWindow, creditsWindow, instructionsWindow;
	// posiblemente las instrucciones sean un vector de texturas, y ir cambiando entre las posiciones
	// del vector con las flechas del teclado, donde cada posicion muestra una pagina diferente de las
	// instrucciones

	Texture spritesheet;
	Sprite* selector;

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
