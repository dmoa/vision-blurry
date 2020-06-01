#include "LevelMaker.hpp"

LevelMaker::LevelMaker(sf::RenderWindow* renderWindow, LevelManager* levelManager, Player* player, Enemies* enemies)
{
	m_window = renderWindow;
	m_levelManager = levelManager;
	m_player = player;
	m_enemies = enemies;
	m_enemyTypes = enemies->GetEnemyTypes();

	m_levelRender.texture.create(levelManager->GetLevelWidth(), levelManager->GetLevelHeight());
	m_levelRender.bg = sf::Color(34, 35, 35);
	m_levelRender.scale = 1;
	m_levelRender.sprite.setScale(m_levelRender.scale, m_levelRender.scale);

	m_cursorMode = Drawing;
	m_selectedItemIndex = 0;

	m_commonBorder = 30;
	m_enemySelectorWidth = (*m_enemyTypes)[0].width + m_commonBorder;

	m_modeSelectorShape.setSize(sf::Vector2f(100, 50));
	m_modeSelectorShape.setPosition(m_commonBorder, 180);
	m_modeSelectorShape.setFillColor(sf::Color::Yellow);

	m_selectedShapeOutline.setSize(sf::Vector2f(m_commonBorder / 2, m_commonBorder / 2));
	m_selectedShapeOutline.setFillColor(sf::Color::White);
	m_selectedShapeOutline.setPosition((m_selectedItemIndex + 0.5) * m_enemySelectorWidth + m_commonBorder - m_selectedShapeOutline.getSize().x / 2, m_commonBorder / 2 - m_selectedShapeOutline.getSize().y / 2);

	m_font.loadFromFile("content/fonts/stats.ttf");
	m_details.setFont(m_font);
	m_details.setCharacterSize(18);
	m_details.setPosition(m_commonBorder, 130);
	m_details.setString("mode: draw\nselected: " + std::to_string(m_selectedItemIndex));
	m_details.setFillColor(sf::Color::White);

	m_toolRenderer.shapes = (*m_enemyTypes)[0].shapes;
	m_toolRenderer.width = (*m_enemyTypes)[0].width;
	m_toolRenderer.height = (*m_enemyTypes)[0].height;
	m_toolRenderer.should_draw = false; // doesn't matter what we set it here because it calculates it every frame
}

void LevelMaker::Draw()
{
	m_levelRender.texture.clear(m_levelRender.bg);

	m_levelManager->Draw(&m_levelRender.texture);
	m_enemies->Draw(&m_levelRender.texture);
	m_player->Draw(&m_levelRender.texture);

	// drawing enemy options to choose from
	for (unsigned int i = 0; i < m_enemyTypes->size(); i++)
	{
		EnemyProperties tempEnemy = (*m_enemyTypes)[i];
		for (unsigned int j = 0; j < tempEnemy.shapes.size(); j ++)
		{
			tempEnemy.shapes[j].setPosition(i * m_enemySelectorWidth + m_commonBorder * 1.5, m_commonBorder);
			m_window->draw(tempEnemy.shapes[j]);
		}
	}

	m_levelRender.texture.display();

	m_levelRender.sprite.setTexture(m_levelRender.texture.getTexture());
	m_levelRender.sprite.setPosition((m_window->getSize().x - m_levelManager->GetLevelWidth() * m_levelRender.scale) / 2, (m_window->getSize().y - m_levelManager->GetLevelHeight() * m_levelRender.scale) / 2);

	m_window->draw(m_levelRender.sprite);
	m_window->draw(m_modeSelectorShape);
	m_window->draw(m_details);
	m_window->draw(m_selectedShapeOutline);

	if (m_toolRenderer.should_draw)
	{
		for (unsigned int i = 0; i < m_toolRenderer.shapes.size(); i++)
		{
			m_window->draw(m_toolRenderer.shapes[i]);
		}
	}
}

void LevelMaker::Update(std::vector<sf::Event>* events)
{
	sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(*m_window));

	// handling events
	for (unsigned int i = 0; i < events->size(); i++)
	{
		sf::Event event = (*events)[i];
		if (event.type == sf::Event::MouseButtonPressed)
		{
			// checking if clicking toggling mode box
			if (Collision::PointInRect(mouse_pos, m_modeSelectorShape.getGlobalBounds()))
			{
				ToggleMode();
			}
			// all enemies have a height of 80, this is temp fix, because enemy heights could change.
			// ideally, we would figure out the max enemy height earlier and use that.
			if (mouse_pos.y > m_commonBorder && mouse_pos.y < m_commonBorder + 80)
			{
				for (unsigned int i = 0; i < m_enemyTypes->size(); i++)
				{
					EnemyProperties ep = (*m_enemyTypes)[i];
					if (Collision::PointInRect(mouse_pos, sf::FloatRect(m_commonBorder + m_enemySelectorWidth * i, m_commonBorder, m_enemySelectorWidth, (*m_enemyTypes)[i].height)))
					{
						SelectEnemy(i);
					}
				}
			}
			// calculating where the cursor is going if "inside the level"
			if (Collision::PointInRect(mouse_pos, m_levelRender.sprite.getGlobalBounds()))
			{
				int x = (mouse_pos.x - m_levelRender.sprite.getPosition().x) / *(m_levelManager->GetTileLength());
				int y = (mouse_pos.y - m_levelRender.sprite.getPosition().y) / *(m_levelManager->GetTileLength());
				HandleTile(sf::Vector2f(x, y));
			}
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case sf::Keyboard::D:
					SelectEnemy((m_selectedItemIndex + 1) % m_enemyTypes->size());
					break;
				case sf::Keyboard::A:
					SelectEnemy((m_selectedItemIndex + m_enemyTypes->size() - 1) % m_enemyTypes->size());
					break;
				default: break;
			}
		}
	}

	m_toolRenderer.should_draw = Collision::PointInRect(mouse_pos, m_levelRender.sprite.getGlobalBounds());
	// updating the tool renderer to the cursor's position
	for (unsigned int i = 0; i < m_toolRenderer.shapes.size(); i++)
	{
		int x = mouse_pos.x - m_toolRenderer.width / 2;
		int y = mouse_pos.y - m_toolRenderer.height / 2;
		m_toolRenderer.shapes[i].setPosition(x, y);
	}
}

void LevelMaker::ToggleMode()
{
	if (m_cursorMode == Erase)
	{
		SetDrawMode();
	}
	else
	{
		SetEraseMode();
	}
}

void LevelMaker::SetDrawMode()
{
	m_cursorMode = Drawing;

	m_modeSelectorShape.setFillColor(sf::Color::Yellow);
	SelectEnemy();

	UpdateText();
}

void LevelMaker::SetEraseMode()
{
	m_cursorMode = Erase;

	m_modeSelectorShape.setFillColor(sf::Color::White);

	m_toolRenderer.shapes.clear();
	m_toolRenderer.shapes.push_back(sf::ConvexShape(4));
	m_toolRenderer.shapes[0].setPoint(0, sf::Vector2f(0, 0));
	m_toolRenderer.shapes[0].setPoint(1, sf::Vector2f(40, 0));
	m_toolRenderer.shapes[0].setPoint(2, sf::Vector2f(40, 40));
	m_toolRenderer.shapes[0].setPoint(3, sf::Vector2f(0, 40));
	m_toolRenderer.shapes[0].setFillColor(sf::Color::Black);
	m_toolRenderer.width = 40;
	m_toolRenderer.height = 40;

	UpdateText();
}

void LevelMaker::SelectEnemy(int index)
{
	if (index != -1) { m_selectedItemIndex = index; } // useful for returning back to draw state without changing the selected enemy

	m_selectedShapeOutline.setPosition((m_selectedItemIndex + 0.5) * m_enemySelectorWidth + m_commonBorder - m_selectedShapeOutline.getSize().x / 2, m_commonBorder / 2 - m_selectedShapeOutline.getSize().y / 2);

	m_toolRenderer.shapes = (*m_enemyTypes)[m_selectedItemIndex].shapes;
	m_toolRenderer.width = (*m_enemyTypes)[m_selectedItemIndex].width;
	m_toolRenderer.height = (*m_enemyTypes)[m_selectedItemIndex].height;
}

void LevelMaker::UpdateText()
{
	std::string new_string = "mode: ";
	new_string += (m_cursorMode == Drawing) ? "draw" : "erase";
	new_string += "\n";
	new_string += "selected: " + std::to_string(m_selectedItemIndex);
	m_details.setString(new_string);
}

void LevelMaker::HandleTile(sf::Vector2f pos)
{
	// if that tile is not the player (player is special, you cannot delet it or add duplicates)
	if (m_cursorMode == Drawing)
	{
		// add if not in enemy pos

		// if choosing enemy
			//m_enemies->AddEnemy(m_selectedItemIndex, pos, m_player->GetPos());
			// remove a wall if it's there, i.e. set base_tile to empty
			// change enemy type in that position, maybe add methodd there?

		m_levelManager->SetTile(pos, m_enemies->GetID(m_selectedItemIndex));
		if (m_enemies->IsEnemyThere(pos))
		{
			m_enemies->ChangeEnemy(pos, m_selectedItemIndex);
		}
		else
		{
			m_enemies->AddEnemy(m_selectedItemIndex, pos, m_player->GetPos());
		}
	}
	else
	{
		if (m_enemies->IsEnemyThere(pos))
		{
			m_enemies->RemoveEnemy(pos);
		}
	}
}

// @DONE
// changing enemies

// @TODO
// deleting and creating enemies
// replacing if there's wall
// eraser and shit