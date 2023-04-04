#ifndef _STOPWATCH_INCLUDE
#define _STOPWATCH_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Stopwatch
{
public:
	~Stopwatch();

	void init(const glm::ivec2& tileMapPos, glm::ivec2 itemPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, char& timeState);
	void render();

	void setPosition(const glm::ivec2& tileMapPos, glm::ivec2 itemPos);
	glm::ivec2 getSize() const;
	glm::vec2 getPosition();
	bool isVisible(); // puede que haya que tener presente cuando aparece, para no recogerlo inmediatamente si está en su animación de inicio

	void setVisibility(bool visible);
	void activate(char& timeState); // para poner en marcha temporizador que congela el tiempo (le pasamos el timeState de Scene para que sincronize con ello)

private:
	glm::vec2 position;
	glm::ivec2 size;
	Texture spritesheet;
	Sprite* sprite;

	float timeLeft; // indica tiempo restante para recogerlo
	char clockState; // 0: invisible, 1: recolectable, 2: activo (invisible, pero congelando el tiempo)

	enum StopwatchAnimations
	{
		IDLE
	};

};


#endif 
