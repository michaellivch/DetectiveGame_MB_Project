//
// Created by michael on 1/16/25.
//

#ifndef LRPARSER_H
#define LRPARSER_H



#include "cfg.h"
#include <vector>
#include <iostream>
#include <queue>

struct Item {
    std::string lhs;                      // Left-hand side of the production
    std::vector<std::string> rhs;         // Right-hand side of the production
    size_t dotPosition;                   // Position of the dot in the RHS
    std::set<std::string> lookahead;      // Set of terminal symbols (lookahead)

    // Equality operator (used for comparisons)
    bool operator==(const Item& other) const {
        return lhs == other.lhs &&
               rhs == other.rhs &&
               dotPosition == other.dotPosition &&
               lookahead == other.lookahead; // Include lookahead in comparisons
    }

    // Less-than operator (used for ordering in std::set)
    bool operator<(const Item& other) const {
        if (lhs != other.lhs) return lhs < other.lhs;
        if (rhs != other.rhs) return rhs < other.rhs;
        if (dotPosition != other.dotPosition) return dotPosition < other.dotPosition;
        return lookahead < other.lookahead; // Include lookahead in ordering
    }
};

struct State {
    int id;                               // Unique identifier for the state
    std::set<Item> items;                 // Set of items in this state

    // Equality operator for State
    bool operator==(const State& other) const {
        return this->items == other.items;
    }
};

// LRParser Class
class LRParser {
public:
    explicit LRParser(const Grammar& grammar);

    // Augmented Grammar Accessor
    [[nodiscard]] const std::vector<GrammarRule>& getAugmentedGrammar() const;

    // Parser Creation
    void createInitialState();
    [[nodiscard]] std::set<Item> computeClosure(const std::set<Item>& items) const;
    State computeGoto(const std::set<Item>& items, const std::string& symbol);
    void createParser();

    // Setters And Getters
    State getInitialState() const;
    void addState(const State& state);

    // Debug
    void printStates() const;

    // Lalr specific functions
    void addLookahead();


private:
    const Grammar& grammar;                      // Reference to the grammar
    std::vector<GrammarRule> augmentedGrammar;   // Augmented grammar rules
    std::vector<State> states;

    void constructAugmentedGrammar();            // Method to construct the augmented grammar
};



#endif //LRPARSER_H
