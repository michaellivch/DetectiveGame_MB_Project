//
// Created by michael on 1/16/25.
//

#include "../include/ParsingTable.h"

// Get the action for a given state and symbol
std::string ParsingTable::getAction(int state, const std::string &symbol) const {
  auto stateIt = actionTable.find(state);
  if (stateIt != actionTable.end()) {
    auto symbolIt = stateIt->second.find(symbol);
    if (symbolIt != stateIt->second.end()) {
      return symbolIt->second;
    }
  }
  return ""; // Return an empty string if no action is found
}

// Get the goto state for a given state and non-terminal
int ParsingTable::getGoto(int state, const std::string &nonTerminal) const {
  auto stateIt = gotoTable.find(state);
  if (stateIt != gotoTable.end()) {
    auto nonTerminalIt = stateIt->second.find(nonTerminal);
    if (nonTerminalIt != stateIt->second.end()) {
      return nonTerminalIt->second;
    }
  }
  return -1; // Return -1 if no goto state is found
}

// Add an action entry to the parsing table
void ParsingTable::addAction(int state, const std::string &symbol, const std::string &action) {
  actionTable[state][symbol] = action;
}

// Add a goto entry to the parsing table
void ParsingTable::addGoto(int state, const std::string &symbol, int nextState) {
  gotoTable[state][symbol] = nextState;
}

// Print the parsing table for debugging purposes
void ParsingTable::printTable() const {
  std::cout << "Action Table:" << std::endl;
  for (const auto &stateEntry : actionTable) {
    int state = stateEntry.first;
    for (const auto &symbolEntry : stateEntry.second) {
      std::cout << "State " << state << ", Symbol '" << symbolEntry.first << "': " << symbolEntry.second << std::endl;
    }
  }

  std::cout << "\nGoto Table:" << std::endl;
  for (const auto &stateEntry : gotoTable) {
    int state = stateEntry.first;
    for (const auto &nonTerminalEntry : stateEntry.second) {
      std::cout << "State " << state << ", Non-Terminal '" << nonTerminalEntry.first << "': " << nonTerminalEntry.second << std::endl;
    }
  }
}