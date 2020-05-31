#pragma once

#include <MapUtil/MapUtil.hpp>

#include "LevelManager.hpp"
#include "EnemyProperties.hpp"
#include "ViewType.hpp"
#include "Enemy.hpp"

class Enemies
{
public:
	Enemies();
	void Setup(LevelManager* levelManager, sf::Vector2f playerPos);
	void Draw(sf::RenderTexture* renderTexture);
	void Update(sf::Vector2f playerPos);
	std::vector<EnemyProperties>* GetEnemyTypes();
	void Reset();
private:
	void AddEnemy(int enemyType, sf::Vector2f pos, sf::Vector2f playerPos);
	std::vector<Enemy> m_enemies;
	std::vector<EnemyProperties> m_enemyTypes;
	LevelManager* m_levelManager;
};