#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class TileMap
{
private:
	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	~TileMap();

	void render() const;
	void free();

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	int getTileSizeX() const;
	int getTileSizeY() const;

private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileWidth, tileHeight;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;	// seguramente poner aqui una matriz con un vector de vectores en vez de simular una matriz 

};

#endif