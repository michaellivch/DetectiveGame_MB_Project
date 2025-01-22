#include "./LRParser/include/cfg.h"
#include "PDA/PDA.h"
#include <iostream>
#include <vector>

//void runTests() {
//    // Initialize the Grammar
//    Grammar grammar;
//
//    // 1. Test Tokenizer
//    std::cout << "==== Tokenizer Test ====" << std::endl;
//    std::string input = "Interrogate the suspect about the bloody knife.";
//    auto tokens = tokenize(input);
//
//    std::cout << "Input: " << input << std::endl;
//    std::cout << "Tokens:" << std::endl;
//    for (const auto& token : tokens) {
//        std::cout << "  Type: " << token.type << ", Value: " << token.value << std::endl;
//    }
//
//    // 2. Test Grammar Rules
//    std::cout << "\n==== Grammar Rules Test ====" << std::endl;
//    const auto& rules = grammar.getRules();
//    for (const auto& rule : rules) {
//        std::cout << rule.lhs << " ::= ";
//        for (const auto& symbol : rule.rhs) {
//            std::cout << symbol << " ";
//        }
//        std::cout << std::endl;
//    }
//
//    // 3. Test Production Map
//    std::cout << "\n==== Production Map Test ====" << std::endl;
//    const auto& productionMap = grammar.getProductionMap();
//    for (const auto& [lhs, productions] : productionMap) {
//        std::cout << lhs << " ::= ";
//        for (const auto& rhs : productions) {
//            for (const auto& symbol : rhs) {
//                std::cout << symbol << " ";
//            }
//            std::cout << "| ";
//        }
//        std::cout << std::endl;
//    }
//
//
//    // 4. Test Follow Sets
//    std::cout << "\n==== Follow Sets Test ====" << std::endl;
//    for (const auto& nonTerminal : grammar.getNonTerminals()) {
//        const auto& followSet = grammar.getFollowSet(nonTerminal);
//        std::cout << "Follow(" << nonTerminal << ") = { ";
//        for (const auto& symbol : followSet) {
//            std::cout << symbol << " ";
//        }
//        std::cout << "}" << std::endl;
//    }
//
//    std::cout << "==== All Tests Completed ====" << std::endl;
//}
//
//int main() {
//    runTests();
//    return 0;
//}

#include "Game/Game.h"

int main() {
  // Load the PDA from a JSON file
  PDA pda("../Assets/game.json");

  // GUI Manager
  GUIManager gui;

  // Main loop
  while (gui.isOpen()) {
    gui.handleEvents(pda);

    // Determine current image based on the PDA's state
    std::string currentImage;
    for (const auto& transition : pda.getTransitions()) {
      if (transition.currentState == pda.getCurrentState()) {
        currentImage = transition.image;
        break;
      }
    }

    gui.update(pda, pda.getStack("ImageStack")[0]);
    gui.render();

    // Check if the PDA has reached an accepting state
    if (pda.isAccepted()) {
      std::cout << "PDA reached an accepting state: " << pda.getCurrentState() << std::endl;
      break;
    }
  }

  return 0;
}