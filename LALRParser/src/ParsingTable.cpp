//
// Created by michael on 1/16/25.
//

#include "../include/ParsingTable.h"


// Get action for state and terminal symbol
std::string ParsingTable::getAction(int state, const std::string& symbol) const {
  // Check if state exists
  auto stateEntry = actionTable.find(state);
  if (stateEntry == actionTable.end()) return "";

  // Check if symbol exists for this state
  auto symbolEntry = stateEntry->second.find(symbol);
  if (symbolEntry == stateEntry->second.end()) return "";

  return symbolEntry->second;
}

// Get goto state for state and non-terminal symbol
int ParsingTable::getGoto(int state, const std::string& nonTerminal) const {
  // Check if state exists
  auto stateEntry = gotoTable.find(state);
  if (stateEntry == gotoTable.end()) return -1;

  // Check if non-terminal exists for this state
  auto nonTerminalEntry = stateEntry->second.find(nonTerminal);
  if (nonTerminalEntry == stateEntry->second.end()) return -1;

  return nonTerminalEntry->second;
}

// Add action entry to table
void ParsingTable::addAction(int state, const std::string& symbol, const std::string& action) {
  actionTable[state][symbol] = action;
}

// Add goto entry to table
void ParsingTable::addGoto(int state, const std::string& nonTerminal, int nextState) {
  gotoTable[state][nonTerminal] = nextState;
}

// Print all table entries
void ParsingTable::printTable() const {
  std::cout << "ACTIONS:\n";
  for (const auto& stateEntry : actionTable) {
    int state = stateEntry.first;
    for (const auto& symbolEntry : stateEntry.second) {
      std::cout << "State " << state << ", Symbol '"
                << symbolEntry.first << "': "
                << symbolEntry.second << "\n";
    }
  }

  std::cout << "\nGOTOS:\n";
  for (const auto& stateEntry : gotoTable) {
    int state = stateEntry.first;
    for (const auto& nonTerminalEntry : stateEntry.second) {
      std::cout << "State " << state << ", Non-Terminal '"
                << nonTerminalEntry.first << "': "
                << nonTerminalEntry.second << "\n";
    }
  }
}