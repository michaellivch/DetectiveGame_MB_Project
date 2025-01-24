//
// Created by jeremi on 22/01/25.
//

#include "StateManager.h"
#include <iostream>

void StateManager::run() {
  sf::Clock clock;
  while (window.isOpen()) {
    float deltaTime = clock.restart().asSeconds();
    // Update and render the active state
    window.clear();
    if (active_state) {
      active_state->update(window, deltaTime); // Delegate update to the active state
    }
    window.display();
  }
}

void StateManager::add_state(const std::shared_ptr<GameState>& state) {
  states.push_back(state);
}
void StateManager::set_active_state(const std::shared_ptr<GameState>& state) {
  active_state = state;
  if (active_state) {
    active_state->enter(window); // Enter the new state
  }
}
std::shared_ptr<GameState> StateManager::get_menu() {
  return (states.size() > 0) ? states[0] : nullptr;
}
std::shared_ptr<GameState> StateManager::get_play() {
  return (states.size() > 1) ? states[1] : nullptr;
}
std::shared_ptr<GameState> StateManager::get_end() {
  return (states.size() > 2) ? states[2] : nullptr;
}
