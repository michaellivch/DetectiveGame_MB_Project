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

  void add_state(const std::shared_ptr<GameState>& state) {
    states.push_back(state);
  }

  void set_active_state(const std::shared_ptr<GameState>& state) {
    if (active_state) {
      active_state->exit(window); // Exit the current state
    }
    active_state = state;
    if (active_state) {
      active_state->enter(window); // Enter the new state
    }
  }

  std::shared_ptr<GameState> get_menu() {
    return (states.size() > 0) ? states[0] : nullptr;
  }

  std::shared_ptr<GameState> get_play() {
    return (states.size() > 1) ? states[1] : nullptr;
  }

  std::shared_ptr<GameState> get_end() {
    return (states.size() > 2) ? states[2] : nullptr;
  }

  void run(); // Main game loop
};

#endif // DETECTIVEGAME_MB_PROJECT_STATEMANAGER_H

