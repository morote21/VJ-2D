#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
#include "Sprite.h"

class Menu 
{
public:
	Menu();
	~Menu();

	void init();	
	void initShaders();
	void update(int deltaTime);
	void render(int numScreen);

	void setSelectorIndex(int index);
	int getSelectorIndex();
	
private:
	int menuSelection, selectorIndex;
	// selector index:
	// play -> 0
	// instructions -> 1
	// credits -> 2
	float currentTime;
	TexturedQuad* background;
	Texture menuWindow, creditsWindow;
	// posiblemente las instrucciones sean un vector de texturas, y ir cambiando entre las posiciones
	// del vector con las flechas del teclado, donde cada posicion muestra una pagina diferente de las
	// instrucciones

	Texture spritesheet;
	Sprite* selector;

	ShaderProgram texProgram;
	glm::mat4 projection;

};


#endif // _MENU_INCLUDE
