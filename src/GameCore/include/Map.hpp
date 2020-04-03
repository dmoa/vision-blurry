#pragma once

#include <TileMapRenderer/TileMapRenderer.hpp>

class Map {
public:
	Map();
	void Update();
	void Draw(sf::RenderTexture* renderTexture);
private:
	int tileLength;
	std::vector<int> gridData;
	TileMapRenderer mapRenderer;
};