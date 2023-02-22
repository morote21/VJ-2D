#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "TexturedQuad.h"
//#include "Sprite.h"

class Menu 
{
public:
	Menu();
	~Menu();

	void init();	
	void initShaders();
	void update(int deltaTime);
	void render(int numScreen);
	
private:
	int menuSelection;
	float currentTime;
	TexturedQuad* background;
	Texture menuWindow, creditsWindow;
	// posiblemente las instrucciones sean un vector de texturas, y ir cambiando entre las posiciones
	// del vector con las flechas del teclado, donde cada posicion muestra una pagina diferente de las
	// instrucciones
	ShaderProgram texProgram;
	glm::mat4 projection;

};


#endif // _MENU_INCLUDE
