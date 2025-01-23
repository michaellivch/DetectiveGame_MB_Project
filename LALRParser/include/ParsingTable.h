//
// Created by michael on 1/16/25.
//

#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

class ParsingTable {
public:
  // Get the action for a given state and symbol
  std::string getAction(int state, const std::string &symbol) const;

  // Get the goto state for a given state and non-terminal
  int getGoto(int state, const std::string &nonTerminal) const;

  // Add an action entry to the parsing table
  void addAction(int state, const std::string &symbol, const std::string &action);

  // Add a goto entry to the parsing table
  void addGoto(int state, const std::string &symbol, int nextState);

  // Print the parsing table for debugging purposes
  void printTable() const;

private:
  // Parsing table for actions: state -> (symbol -> action)
  std::map<int, std::map<std::string, std::string>> actionTable;

  // Parsing table for goto states: state -> (non-terminal -> next state)
  std::map<int, std::map<std::string, int>> gotoTable;
};


#endif // PARSING_TABLE_H

