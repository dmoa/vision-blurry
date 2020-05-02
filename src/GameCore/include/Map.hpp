#pragma once

#include <TileMapRenderer/TileMapRenderer.hpp>
#include <MapLoader/MapLoader.hpp>
#include "Tile.hpp"

class Map {
public:
	Map();
	void Update();
	void Draw(sf::RenderTexture* renderTexture);
	Tile* getTile(int tileX, int tileY);
	int getTileID(int tileX, int tileY);
	sf::IntRect getTileQuad(int tileX, int tileY);
	void removeTile(int tileX, int tileY);
	std::vector<int> getPlayerStartingPos();
private:
	std::map<int, Tile> m_tileData;
	std::vector<int> m_gridData;
	TileMapRenderer m_mapRenderer;
	int m_mapTileLength;
	int m_tileLength;
	int m_tilesetWidth;
	int m_playerStartingPos;
};