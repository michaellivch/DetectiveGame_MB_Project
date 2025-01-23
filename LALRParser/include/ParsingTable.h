//
// Created by michael on 1/16/25.
//

#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include <map>
#include <string>
#include <vector>
#include <iostream>


class ParsingTable {
public:
  ParsingTable() = default;

  // Setters
  void addAction(int state, const std::string& symbol, const std::string& action);
  void addGoto(int state, const std::string& symbol, int nextState);

  // Getters
  [[nodiscard]] std::string getAction(int state, const std::string& symbol) const;
  [[nodiscard]] int getGoto(int state, const std::string& symbol) const;

  // Debugging
  void printTable() const;

private:
  // Action table: state -> (symbol -> action)
  std::map<int, std::map<std::string, std::string>> actionTable;

  // Goto table: state -> (non-terminal -> next state)
  std::map<int, std::map<std::string, int>> gotoTable;
};

#endif // PARSING_TABLE_H

