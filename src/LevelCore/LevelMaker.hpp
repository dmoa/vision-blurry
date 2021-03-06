#pragma once

#include <Collision/Collision.hpp>

#include "LevelRenderer.hpp"
#include "LevelManager.hpp"
#include "../GameCore/include/EnemyProperties.hpp"
#include "../GameCore/include/Enemies.hpp"
#include "../GameCore/include/Player.hpp"

#include "../Render.hpp"

enum CursorModes
{
	Drawing, Erase
};

enum DrawingModes
{
	D_Enemies, D_Walls, D_Player // ugly prefix so that it doesn't clash with classes
};

struct Tool
{
	std::vector<sf::ConvexShape> shapes;
	int width;
	int height;
	bool should_draw;
};

class LevelMaker
{
public:
	LevelMaker(sf::RenderWindow* renderWindow, LevelRenderer* levelRenderer, LevelManager* levelManager, Player* player, Enemies* enemies, sf::Font* font);
	void Draw();
	void Update(std::vector<sf::Event>* events);
private:
	void ToggleMode();
	void SetDrawMode();
	void SetEraseMode();
	void SelectEnemy(int index = -1);
	void SelectWall();
	void SelectPlayer();
	void UpdateText();
	void HandleTile(sf::Vector2f pos);
	void ChangeLevelSize(int x, int y);
	sf::RenderWindow* m_window;
	LevelRenderer* m_levelRenderer;
	LevelManager* m_levelManager;
	Player* m_player;
	Enemies* m_enemies;
	std::vector<EnemyProperties>* m_enemyTypes;
	CursorModes m_currentMode;
	DrawingModes m_currentDrawingMode;
	int m_selectedEnemyIndex;
	int m_commonBorder;
	int m_itemSelectorWidth;
	sf::Font* m_font;
	sf::Text m_details;
	sf::RectangleShape m_modeSelectorShape;
	sf::RectangleShape m_selectedShapeOutline;
	sf::RectangleShape m_wallSelectorShape;
	sf::RectangleShape m_playerSelectorShape;
	sf::RectangleShape m_saveLevelShape;
	Tool m_toolRenderer;
};