#include <iostream>
#include "LRParser/include/lrparser.h"
#include "LRParser/include/cfg.h"

int main() {
  // Definieer een voorbeeldgrammatica
  Grammar grammar = Grammar();

  // Initialiseer de LR-parser met de grammatica
  LRParser parser(grammar);

  // Bouw de LR(0)-automaat
  parser.createParser();

  // Print de gegenereerde staten
  std::cout << "LR(0)-automaat:\n";
  parser.printStates();

  // Voeg lookaheads toe
  parser.addLookahead();

  // Print de staten met lookahead-symbolen
  std::cout << "\nLR(1)-automaat met lookahead-symbolen:\n";
  parser.printStates();

  return 0;
}
