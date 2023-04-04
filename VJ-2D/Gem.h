#ifndef _GEM_INCLUDE
#define _GEM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Gem
{
public:
	~Gem();

	void init(const glm::ivec2& tileMapPos, glm::ivec2 itemPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::ivec2& tileMapPos, glm::ivec2 itemPos);
	glm::ivec2 getSize() const;
	glm::vec2 getPosition();
	bool isVisible(); // puede que haya que tener presente cuando aparece, para no recogerlo inmediatamente si está en su animación de inicio

	void setVisibility(bool visible);

private:
	glm::vec2 position;
	glm::ivec2 size;
	Texture spritesheet;
	Sprite* sprite;

	float timeLeft; // indica tiempo restante para recogerlo (y si -1, indica que ya no es visible; no sé si ésto último será necesario)

	enum GemAnimations
	{
		IDLE
	};

};


#endif 

