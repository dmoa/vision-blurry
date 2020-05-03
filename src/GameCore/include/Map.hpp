#pragma once

#include <TileMapRenderer/TileMapRenderer.hpp>
#include <MapLoader/MapLoader.hpp>
#include "Tile.hpp"

class Map {
public:
	Map();
	void Update();
	void Draw(sf::RenderTexture* renderTexture);
	Tile* GetTile(int tileX, int tileY);
	int GetTileID(int tileX, int tileY);
	sf::IntRect GetTileQuad(int tileX, int tileY);
	void RemoveTile(int tileX, int tileY);
	std::vector<int> GetPlayerStartingPos();
private:
	std::map<int, Tile> m_tileData;
	std::vector<int> m_gridData;
	TileMapRenderer m_mapRenderer;
	int m_mapTileLength;
	int m_tileLength;
	int m_tilesetWidth;
	int m_playerStartingPos;
};