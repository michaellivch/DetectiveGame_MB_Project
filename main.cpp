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
  parser.printStates();


  return 0;
}
