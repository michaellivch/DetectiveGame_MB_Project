//
// Created by jeremi on 22/01/25.
//

#ifndef DETECTIVEGAME_MB_PROJECT_STATE_H
#define DETECTIVEGAME_MB_PROJECT_STATE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../PDA/PDA.h"
// Forward declaration of StateManager
class StateManager;

class GameState {
protected:
  StateManager& manager; // Reference to the StateManager
public:
  explicit GameState(StateManager& manager) : manager(manager) {}
  virtual ~GameState() = default;

  virtual void enter(sf::RenderWindow& window) = 0;  // Called when the state becomes active
  virtual void update(sf::RenderWindow& window, float deltaTime) = 0; // Logic updates
  virtual void exit(sf::RenderWindow& window) = 0;   // Called when the state is exited
};


class MainScreen : public GameState {
private:
  sf::Text titleText;
  sf::Font font;
public:
  explicit MainScreen(StateManager& manager);
  void enter(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window, float deltaTime) override;
  void exit(sf::RenderWindow& window) override;
};

class PlayState : public GameState {
private:
  PDA& pda;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Font font;
  sf::Text stateText;
  sf::Text stackText;
  sf::Text inputText;  // User input text
  sf::RectangleShape inputBar; // Bar for user input
  std::string userInput;  // To store the current input
  std::string stackToString(const std::vector<std::string>& stack) const;
public:
  PlayState(StateManager& manager, PDA& pda);
  void processInput(const std::string& input);
  void enter(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window, float deltaTime) override;
  void exit(sf::RenderWindow& window) override;
};

class EndScreen : public GameState {
private:
  sf::Text endText;
  sf::Font font;
public:
  explicit EndScreen(StateManager& manager);
  void enter(sf::RenderWindow& window) override;
  void update(sf::RenderWindow& window, float deltaTime) override;
  void exit(sf::RenderWindow& window) override;
};

#endif // DETECTIVEGAME_MB_PROJECT_STATE_H
