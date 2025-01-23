#include "LALRParser/include/LALR_Parser.h"
#include "LALRParser/include/cfg.h"
#include <iostream>

int main() {
  Grammar grammar = Grammar(); // Assume this is initialized
  LALRParser parser(grammar);

  parser.createParser();
  parser.printStates();

//  parser.generateParsingTable();
//  parser.parsingTable.printTable(); //Print the parsing table for DEBUG

  std::vector<Token> tokens = tokenize("interrogate suspect about cop.");

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