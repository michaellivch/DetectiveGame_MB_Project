#include "LALRParser/include/LALR_Parser.h"
#include "LALRParser/include/cfg.h"
#include <iostream>

int main() {
  Grammar grammar = Grammar(); // Assume this is initialized
  LALRParser parser(grammar);

  parser.createParser();
  parser.printStates();

  std::vector<Token> tokens = tokenize("interrogate suspect about cop.");

  return 0;
}

