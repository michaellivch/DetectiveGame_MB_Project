#include <algorithm>
#include <iostream>

#include "./CFG/cfg.h"

int main() {
  // Define grammar rules
  std::vector<GrammarRule> grammar = {
    {"command", {"ACTION", "TARGET", "PREPOSITION", "TOPIC", "TERMINAL_PUNCTUATION"}},
    {"command", {"ACTION", "TARGET", "PREPOSITION", "TOPIC"}},
};


  // Input string
  std::string input;
  std::cout << "Enter a command: ";
  std::getline(std::cin, input);

  // Tokenize input
  auto tokens = tokenize(input);

  std::cout << "Tokens: ";
  for (const auto& token : tokens) {
    std::cout<<token.type<<": "<<token.value<<" ";
  }
  std::cout << std::endl;

  return 0;
}
