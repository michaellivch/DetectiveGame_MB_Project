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
