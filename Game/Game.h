//
// Created by jeremi on 22/01/25.
//

#ifndef DETECTIVEGAME_MB_PROJECT_GAME_H
#define DETECTIVEGAME_MB_PROJECT_GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../PDA/PDA.h"

class GUIManager {
private:
  sf::RenderWindow window;
  sf::Font font;
  sf::Text stateText;
  sf::Text stackText;
  sf::Text inputOptions;
  sf::Texture texture;
  sf::Sprite sprite;

public:
  GUIManager() : window(sf::VideoMode(500, 500), "Detective Game") {
    if (!font.loadFromFile("../Assets/font.ttf")) {
      std::cerr << "Error: Could not load font.\n";
    }

    stateText.setFont(font);
    stateText.setCharacterSize(24);
    stateText.setPosition(10, 10);

    stackText.setFont(font);
    stackText.setCharacterSize(24);
    stackText.setPosition(10, 50);

    inputOptions.setFont(font);
    inputOptions.setCharacterSize(24);
    inputOptions.setPosition(10, 90);
  }

  void update(const PDA& pda, const std::string& currentImage) {
    // Update texture and sprite
    if (!texture.loadFromFile(currentImage)) {
      std::cerr << "Error: Could not load image " << currentImage << "\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(
        window.getSize().x / static_cast<float>(texture.getSize().x),
        window.getSize().y / static_cast<float>(texture.getSize().y)
    );
    sprite.setPosition(0,0);

    // Update text elements
    stateText.setString("Current State: " + pda.getCurrentState());
    stackText.setString("Stack: " + stackToString(pda.getStack("MainStack")));
    inputOptions.setString("Options: Press 0, 1, or 2 for transitions.");
  }

  void render() {
    window.clear();
    window.draw(sprite);
    window.draw(stateText);
    window.draw(stackText);
    window.draw(inputOptions);
    window.display();
  }

  bool isOpen() const {
    return window.isOpen();
  }

  void handleEvents(PDA& pda) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      // Handle input
      if (event.type == sf::Event::KeyPressed) {
        std::string input;
        if (event.key.code == sf::Keyboard::Num0) {
          input = "0";
        } else if (event.key.code == sf::Keyboard::Num1) {
          input = "1";
        } else if (event.key.code == sf::Keyboard::Num2) {
          input = "2";
        }
        else if (event.key.code == sf::Keyboard::Enter){
          input = "epsilon";
        }

        if (!input.empty()) {
          if (!pda.processInput(input)) {
            std::cerr << "Invalid transition.\n";
          }
        }
      }
    }
  }

  std::string stackToString(const std::vector<std::string>& stack) const {
    std::string result;
    for (const auto& symbol : stack) {
      result += symbol + " ";
    }
    return result.empty() ? "Empty" : result;
  }
};

#endif // DETECTIVEGAME_MB_PROJECT_GAME_H
