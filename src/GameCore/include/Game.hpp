#pragma once

#include <StateMachine/State.hpp>

#include "LevelManager.hpp"
#include "Player.hpp"
// #include "Enemies.hpp"

class Game final: public sm::State
{
public:
	Game();
private:
	void Update(sf::Time deltaTime, const std::shared_ptr<sf::RenderWindow>& window);
	void Draw(const std::shared_ptr<sf::RenderWindow>& window);
	void PlayerMoveAttempt(bool playerDidMove);
	sf::RenderTexture m_renderTexture;
	// sf::RenderTexture m_renderTexture_noShader;
	sf::Color m_backgroundColor;
	sf::Shader m_shader;
	sf::Sprite m_sprite;
	sf::Sprite m_sprite_noShader;
	sf::Texture m_tileSetTexture;
	LevelManager m_levelManager;
	Player m_player;
	// Enemies m_enemies;
};
