//
// Created by jeremi on 22/01/25.
//

#ifndef DETECTIVEGAME_MB_PROJECT_STATE_H
#define DETECTIVEGAME_MB_PROJECT_STATE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../PDA/PDA.h"
#include "../LALRParser/include/cfg.h"
#include "../LALRParser/include/LALR_Parser.h"
// Forward declaration of StateManager
class StateManager;

class GameState {
protected:
  StateManager& manager;
public:
  explicit GameState(StateManager& manager) : manager(manager) {}
  virtual ~GameState() = default;
  virtual void enter(sf::RenderWindow& window) = 0;
  virtual void update(sf::RenderWindow& window, float deltaTime) = 0;
  virtual void exit(sf::RenderWindow& window) = 0;
};

class MainScreen : public GameState {
private:
  sf::Text titleText;
  sf::Font font;

  sf::Texture bg_texture_;
  sf::Sprite bg;
public:
  explicit MainScreen(StateManager& manager);
  void enter(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window, float deltaTime) override;
  void exit(sf::RenderWindow& window) override;
};

class PlayState : public GameState {
private:
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Font font;
  sf::Text stateText;
  sf::Text stackText;
  sf::Text inputText;
  sf::Text hoverText;
  sf::RectangleShape inputBar;
  std::vector<std::pair<sf::FloatRect, std::string>> hoverRegions;
  sf::RectangleShape rectangle;

  PDA& pda;

  std::string userInput;
  Grammar grammar;
  LALRParser parser;

  std::string stackToString(const std::vector<std::string>& stack) const;

public:
  PlayState(StateManager& manager, PDA& pda);
  void processInput(const std::string& target, const std::string& topic);
  void enter(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window, float deltaTime) override;
  void exit(sf::RenderWindow& window) override;
};

class EndScreen : public GameState {
private:
  sf::Text endText;
  sf::Font font;

  sf::Texture bg_texture_;
  sf::Sprite bg;
public:
  explicit EndScreen(StateManager& manager);
  void enter(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window, float deltaTime) override;
  void exit(sf::RenderWindow& window) override;
};

#endif // DETECTIVEGAME_MB_PROJECT_STATE_H
