#include "State.h"
#include "StateManager.h"
#include <iostream>

// ---- State Base Class ----
// Already defined in header. No additional implementation needed here.

// ---- MainScreen Class ----
MainScreen::MainScreen(StateManager& manager) : GameState(manager) {
  if (!font.loadFromFile("../Assets/font.ttf")) {
    std::cerr << "Error: Could not load font for MainScreen.\n";
  }

  titleText.setFont(font);
  titleText.setCharacterSize(50);
  titleText.setString("Welcome to Detective Game\nPress Enter to Start");
  titleText.setPosition(50, 200);
}

void MainScreen::enter(sf::RenderWindow& window) {
  // Reset or initialize elements if necessary
}

void MainScreen::update(sf::RenderWindow& window, float deltaTime) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
    manager.set_active_state(manager.get_play());
  }
  window.draw(titleText);
}

void MainScreen::exit(sf::RenderWindow& window) {
  // Clean up or reset any MainScreen-specific data if necessary
}


// ---- PlayState Class ----

// Constructor
PlayState::PlayState(StateManager& manager, PDA& pda)
    : GameState(manager), pda(pda), userInput("") {
  if (!font.loadFromFile("../Assets/font.ttf")) {
    std::cerr << "Error: Could not load font.\n";
  }
  inputText.setFont(font);
  inputText.setCharacterSize(24);
  inputText.setFillColor(sf::Color::White);
  inputText.setPosition(10, 683); // Position for input text

  // Initialize Input Bar
  inputBar.setSize(sf::Vector2f(600, 50));
  inputBar.setFillColor(sf::Color(50, 50, 50));
  inputBar.setPosition(0, 675); // Bar at the bottom

  // Initialize State Text
  stateText.setFont(font);
  stateText.setCharacterSize(24);
  stateText.setFillColor(sf::Color::White);
  stateText.setPosition(10, 10); // Position for state text
}

// Enter state
void PlayState::enter(sf::RenderWindow& window) {
  // Update scene image to match the initial PDA state
  if (!texture.loadFromFile(pda.getStack("ImageStack")[0])) {
    std::cerr << "Error: Could not load initial image from PDA.\n";
  }
  sprite.setPosition(0,75);
  sprite.setScale(
window.getSize().x / static_cast<float>(texture.getSize().x),
(window.getSize().y - 125) / static_cast<float>(texture.getSize().y)
  );
  sprite.setTexture(texture);
}

// Update state
void PlayState::update(sf::RenderWindow& window, float deltaTime) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
    // Handle user typing
    if (event.type == sf::Event::TextEntered) {
      if (event.text.unicode == '\b') { // Backspace
        if (!userInput.empty()) {
          userInput.pop_back();
        }
      } else if (event.text.unicode == '\r' || event.text.unicode == '\n') { // Enter key
        if (userInput.empty() && pda.isFinalState()){
          manager.set_active_state(manager.get_end());
        }
        if (userInput.empty()){
          processInput("epsilon");
        }
        if (!userInput.empty()) {
          processInput(userInput);
          userInput.clear();
        }
      } else if (event.text.unicode < 128) { // Valid character
        userInput += static_cast<char>(event.text.unicode);
      }
    }
  }
  if (pda.hasEpsilonTransition() || pda.isFinalState()) {
    inputText.setString("Press Enter to Continue");
  } else {
    inputText.setString(userInput);
  }
  stateText.setString(pda.getStack("TextStack")[0]);
  // Draw everything
  window.clear();

  // Draw scene image
  window.draw(sprite);

  // Draw text and input bar
  window.draw(stateText);
  window.draw(stackText);
  window.draw(inputBar);
  window.draw(inputText);

  window.display();
}

// Exit state
void PlayState::exit(sf::RenderWindow& window) {
  std::cout << "Exiting PlayState.\n";
}

// Process user input
void PlayState::processInput(const std::string& input) {
  if (!pda.processInput(input)) {
    std::cerr << "Invalid transition.\n";
    return;
  }

  // Update scene based on PDA's new state
  const auto& imageStack = pda.getStack("ImageStack");
  if (!imageStack.empty() && !texture.loadFromFile(imageStack[0])) {
    std::cerr << "Error: Could not load image " << imageStack[0] << ".\n";
  }
  sprite.setTexture(texture);
}

// Convert stack to string for display
std::string PlayState::stackToString(const std::vector<std::string>& stack) const {
  std::string result;
  for (const auto& symbol : stack) {
    result += symbol + " ";
  }
  return result.empty() ? "Empty" : result;
}


// ---- EndScreen Class ----
EndScreen::EndScreen(StateManager& manager) : GameState(manager) {
  if (!font.loadFromFile("../Assets/font.ttf")) {
    std::cerr << "Error: Could not load font for EndScreen.\n";
  }
  endText.setFont(font);
  endText.setCharacterSize(50);
  endText.setString("Game Over\nPress Enter to Return to Menu");
  endText.setPosition(50, 200);
}

void EndScreen::enter(sf::RenderWindow& window) {
  std::cout << "EndScreen entered.\n";
}

void EndScreen::update(sf::RenderWindow& window, float deltaTime) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
      manager.set_active_state(manager.get_menu());
    }
  }
  window.draw(endText);
}


void EndScreen::exit(sf::RenderWindow& window) {
  // Clean up or reset EndScreen-specific data if necessary
}
