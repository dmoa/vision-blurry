#include "../include/LevelManager.hpp"

LevelManager::LevelManager()
{
    m_lineThickness = 2;
    m_tileLength = 100 + m_lineThickness;

    m_gridLineColor = sf::Color(46, 52, 64);

    m_tileData[0] = Tile("ground", sf::Color(216, 222, 233), true);
    m_tileData[1] = Tile("wall", sf::Color(129,161,193), false);

    LoadLevel();
    ReloadRenderer();
}

// Draws everything except the player and enemies
void LevelManager::Draw(sf::RenderTexture* renderTexture)
{
	for (unsigned int i = 0; i < m_currentLevel.base_tiles.size(); i++)
    {
        renderTexture->draw(m_renderedTiles[i]);
    }
}

void LevelManager::LoadLevel()
{
    m_currentLevelIndex = 1;
    std::string path = "content/levels/" + std::to_string(m_currentLevelIndex);
    std::tuple<int, int, std::vector<int> > loadedData = LevelLoader::GetLevel(path);
    m_currentLevel.width = std::get<0>(loadedData);
    m_currentLevel.height = std::get<1>(loadedData);
    m_currentLevel.all_tiles = std::get<2>(loadedData);
}

void LevelManager::ReloadRenderer()
{
    printf("reload renderer\n");
    // setting render of level objects other than enemy and player
    m_renderedTiles.clear();
    m_currentLevel.base_tiles.clear();
    for (unsigned int i = 0; i < m_currentLevel.all_tiles.size(); i++)
    {
        int ID = m_currentLevel.all_tiles[i];
        // if the ID is a player or enemy, make the tile a background / empty tile
        if (ID > 1) {
            ID = 0;
        }
        m_currentLevel.base_tiles.push_back(ID);

        sf::RectangleShape shape(sf::Vector2f(m_tileLength, m_tileLength));
        sf::Vector2f pos = MapUtil::GetIntToVector(i, m_currentLevel.width);
        shape.setPosition(pos.x * m_tileLength + m_lineThickness / 2, pos.y * m_tileLength + m_lineThickness / 2);
        shape.setFillColor(m_tileData[ID].GetColor());
        // dividing by 2, since the border of two squares will double the line thickness
        shape.setOutlineThickness(- m_lineThickness / 2);
        shape.setOutlineColor(m_gridLineColor);
        m_renderedTiles.push_back(shape);
    }
}

std::vector<sf::Vector2f> LevelManager::GetTileLocations(int ID)
{
    std::vector<sf::Vector2f> results;
    printf("4-3\n");
    for (unsigned int i = 0; i < m_currentLevel.all_tiles.size(); i++)
    {
        if (m_currentLevel.all_tiles[i] == ID)
        {
            results.push_back(MapUtil::GetIntToVector(i, m_currentLevel.width));
        }
    }
    printf("4-4\n");
    return results;
}

Tile* LevelManager::GetBaseTile(sf::Vector2f pos)
{
    return &m_tileData[m_currentLevel.base_tiles[MapUtil::GetVectorToInt(pos, m_currentLevel.width)]];
}

bool LevelManager::IsBaseTile(int index)
{
    return index < 2;
}

int LevelManager::GetTileIndex(sf::Vector2f pos)
{
    return m_currentLevel.all_tiles[MapUtil::GetVectorToInt(pos, m_currentLevel.width)];
}

void LevelManager::SetTile(sf::Vector2f pos, int new_id)
{
    m_currentLevel.all_tiles[MapUtil::GetVectorToInt(pos, m_currentLevel.width)] = new_id;
    ReloadRenderer();
}

int* LevelManager::GetTileLength()
{
    return &m_tileLength;
}

int* LevelManager::GetLineThickness()
{
    return &m_lineThickness;
}

int LevelManager::GetLevelWidth()
{
    return m_currentLevel.width * m_tileLength + m_lineThickness;
}

int LevelManager::GetLevelHeight()
{
    return m_currentLevel.height * m_tileLength + m_lineThickness;
}

int LevelManager::GetLevelTileWidth()
{
    return m_currentLevel.width;
}

int LevelManager::GetLevelTileHeight()
{
    return m_currentLevel.height;
}