#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Scene.h"

using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}

TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	texProgram = program;
	padding = minCoords;
	loadLevel(levelFile);
	prepareArrays(minCoords, texProgram);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render(const glm::vec2& minCoords, ShaderProgram& program) 
{
	//prepareArrays(minCoords, program);
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}


bool TileMap::loadLevel(const string& levelFile)
{
	steppedTiles = 0;

	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileWidth >> tileHeight;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	validTiles = 0;
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - int('0');
			if ((tile - int('0')) == 2)
				validTiles += 1;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	return true;
}

glm::ivec2 TileMap::getMapSize() const
{
	return mapSize;
}


void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;
	nTiles = 0;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0 && tile != 8 && tile != 6 && tile != 3 && tile != 9 && tile != 7)
			{
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileWidth, minCoords.y + j * tileHeight);
				if (tile == 1)
					texCoordTile[0] = glm::vec2(0.f, 0.f);
				else if (tile == 2)
					texCoordTile[0] = glm::vec2(0.f, 0.5f);
				else if (tile == -1)
					texCoordTile[0] = glm::vec2(0.5f, 0.f);
				else if (tile == 5)
					texCoordTile[0] = glm::vec2(0.5f, 0.5f);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + tileWidth); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + tileWidth); vertices.push_back(posTile.y + tileHeight);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);

				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + tileWidth); vertices.push_back(posTile.y + tileHeight);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + tileHeight);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
			if (tile == 4) {
				doorPos = glm::ivec2(i, j);
			}
			if (tile == 8) {
				itemsPositions.push_back(glm::ivec2( i * tileWidth, j * tileHeight));
			}
			if (tile == 3) {
				soldiersPositions.push_back(glm::ivec2(i * tileWidth, j * tileHeight));
			}
			if (tile == 6) {
				aliensPositions.push_back(glm::ivec2(i * tileWidth, j * tileHeight));
			}
			if (tile == 9) {
				playerInitPos = glm::ivec2(i * tileWidth, j * tileHeight);
			}
			if (tile == 7) {
				mummiesPositions.push_back(glm::ivec2(i * tileWidth, j * tileHeight));
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

int TileMap::getTileSizeX() const
{
	return tileWidth;
}

int TileMap::getTileSizeY() const
{
	return tileHeight;
}


bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = pos.x / tileWidth; // x izquierda (para ref.)
	y0 = pos.y / tileHeight;
	y1 = (pos.y + size.y - 1) / tileHeight; // anchura en y objeto

	for (int y = y0; y <= y1; y++) // chequeo sobre las y
	{
		// true en caso de que la tile sea un muro y no sea el techo
		if (map[y * mapSize.x + x] == 1 && y != 0) // si posiciones no son de estas tiles
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileWidth; // x derecha (sumado size a pos., para ref.)
	y0 = pos.y / tileHeight;
	y1 = (pos.y + size.y - 1) / tileHeight;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 1 && y != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const // �Podr�amos hacer que devolviera el tipo de tile, para los pinchos?
{
	int x0, x1, y;

	x0 = pos.x / tileWidth;
	x1 = (pos.x + size.x - 1) / tileWidth;
	y = (pos.y + size.y - 1) / tileHeight; // y ref. es la de abajo
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 1 && map[y * mapSize.x + x] != 4 && map[y * mapSize.x + x] != 8  && map[y * mapSize.x + x] != 6 && map[y * mapSize.x + x] != 9 && map[y * mapSize.x + x] != 3 && map[y * mapSize.x + x] != 7)
		{
			if (*posY - tileHeight * y + size.y <= 6)
			{
				*posY = tileHeight * y - size.y; // posy es para reajustar posici�n vertical objeto (por si te has "metido" en el objeto; en colisiones horizontales no hace falta, pues movimiento p�xel siempre es par (si respetamos velocidad de Bob), con lo que acabar�s siempre justo al lado de la tile)
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileWidth;
	x1 = (pos.x + size.x - 1) / tileWidth;
	y = (pos.y - 1) / tileHeight;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 1)
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUpAnything(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileWidth;
	x1 = (pos.x + size.x - 1) / tileWidth;
	y = (pos.y - 1) / tileHeight;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 4 && map[y * mapSize.x + x] != 8 && map[y * mapSize.x + x] != 3 && map[y * mapSize.x + x] != 6 && map[y * mapSize.x + x] != 9 && map[y * mapSize.x + x] != 7)
		{
			return true;
		}
	}

	return false;
}

int TileMap::getTileInPos(int x, int y) const
{
	return map[y * mapSize.x + x];
}

void TileMap::tileStepped(int x, int y)
{
	map[y * mapSize.x + x] = -1;
	prepareArrays(padding, texProgram);	// padding es minCoords (0, 40)
	steppedTiles += 1;
}

bool TileMap::keyAppeared() const
{
	return (steppedTiles >= validTiles);
}

glm::ivec2 TileMap::getDoorPos() 
{
	return doorPos;
}

void TileMap::setAllSteppedTiles()
{
	steppedTiles = validTiles;
}

vector<glm::ivec2> TileMap::getItemsPositions()
{
	return itemsPositions;
}

vector<glm::ivec2> TileMap::getSoldiersPositions()
{
	return soldiersPositions;
}


vector<glm::ivec2> TileMap::getAliensPositions()
{
	return aliensPositions;
}

vector<glm::ivec2> TileMap::getMummiesPositions()
{
	return mummiesPositions;
}

glm::ivec2 TileMap::getPlayerInitPos()
{
	return playerInitPos;
}