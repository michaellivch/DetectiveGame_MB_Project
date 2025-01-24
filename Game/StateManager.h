//
// Created by jeremi on 22/01/25.
//
#ifndef DETECTIVEGAME_MB_PROJECT_STATEMANAGER_H
#define DETECTIVEGAME_MB_PROJECT_STATEMANAGER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include "State.h"
#include "memory"
// Forward declaration of State

class StateManager {
private:
  std::vector<std::shared_ptr<GameState>> states; // Holds all the states
  std::shared_ptr<GameState> active_state;       // Currently active state
  sf::RenderWindow& window;                  // Reference to the main window

public:
  explicit StateManager(sf::RenderWindow& window) : window(window) {}
  void add_state(const std::shared_ptr<GameState>& state);
  void set_active_state(const std::shared_ptr<GameState>& state);
  std::shared_ptr<GameState> get_menu();
  std::shared_ptr<GameState> get_play();
  std::shared_ptr<GameState> get_end();
  void run(); // Main game loop
};

#endif // DETECTIVEGAME_MB_PROJECT_STATEMANAGER_H

