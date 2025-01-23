#include "LALRParser/include/LALR_Parser.h"
#include "LALRParser/include/cfg.h"
#include <iostream>

int main() {
  // Initialize grammar and parser
  Grammar grammar;
  LALRParser parser(grammar);

  // Build the parsing table
  parser.createParser();

  // For debugging: print states and parsing table
  parser.printStates();
  parser.getParsingTable().printTable();

  std::string input;
  while (true) {
    std::cout << "Enter command (or 'exit' to quit):\n> ";
    std::getline(std::cin, input);

    if (input == "exit") break;

    // Tokenize input
    std::vector<Token> tokens = tokenize(input);

    // Check for invalid tokens
    bool valid = true;
    for (const auto& token : tokens) {
      if (token.type == INVALID && token.value != "") {
        std::cerr << "Invalid token: " << token.value << "\n";
        valid = false;
        break;
      }
    }
    if (!valid) continue;


    // Parse the tokens
    parser.parse(tokens);
  }

  return 0;
}


//#include <SFML/Graphics.hpp>
//#include "Game/StateManager.h"
//#include "Game/State.h"
//#include "PDA/PDA.h"
//
//int main() {
//  sf::RenderWindow window(sf::VideoMode(600, 725), "Detective Game");
//  // Create PDA instance
//  PDA pda("../Assets/game.json");
//  // Create StateManager instance
//  StateManager manager(window);
//  // Create states
//  auto mainScreen = std::make_shared<MainScreen>(manager);
//  auto playState = std::make_shared<PlayState>(manager, pda);
//  auto endScreen = std::make_shared<EndScreen>(manager);
//  // Add states to manager
//  manager.add_state(mainScreen);
//  manager.add_state(playState);
//  manager.add_state(endScreen);
//  // Set initial state
//  manager.set_active_state(mainScreen);
//  // Run the game loop
//  manager.run();
//
//  return 0;
//}