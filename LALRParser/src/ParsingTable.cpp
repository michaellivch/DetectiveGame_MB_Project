//
// Created by michael on 1/16/25.
//

#include "../include/ParsingTable.h"

std::string ParsingTable::getAction(int state,
                                    const std::string &symbol) const {
  return std::string();
}
int ParsingTable::getGoto(int state, const std::string &nonTerminal) const {
  return 0;
}
void ParsingTable::addAction(int state, const std::string &symbol,
                             const std::string &action) {}
void ParsingTable::addGoto(int state, const std::string &symbol,
                           int nextState) {}
void ParsingTable::printTable() const {}
