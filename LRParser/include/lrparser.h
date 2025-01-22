//
// Created by michael on 1/16/25.
//

#ifndef LRPARSER_H
#define LRPARSER_H



#include "cfg.h"
#include <vector>

struct Item {
    std::string lhs;                      // Left-hand side of the production
    std::vector<std::string> rhs;         // Right-hand side of the production
    size_t dotPosition;                   // Position of the dot in the RHS

    // Comparison operator for std::set and std::map
    bool operator<(const Item& other) const {
        // Compare lhs first
        if (lhs != other.lhs) {
            return lhs < other.lhs;
        }

        // Compare rhs lexicographically
        if (rhs != other.rhs) {
            return rhs < other.rhs; // std::vector provides < operator
        }

        // Compare dotPosition as a last resort
        return dotPosition < other.dotPosition;
    }
};

struct State {
    int id;                               // Unique identifier for the state
    std::set<Item> items;                 // Set of items in this state

};

// LRParser Class
class LRParser {
public:
    explicit LRParser(const Grammar& grammar);

    // Augmented Grammar Accessor
    [[nodiscard]] const std::vector<GrammarRule>& getAugmentedGrammar() const;


    void createInitialState();
    [[nodiscard]] std::set<Item> computeClosure(const std::set<Item>& items) const;

private:
    const Grammar& grammar;                      // Reference to the grammar
    std::vector<GrammarRule> augmentedGrammar;   // Augmented grammar rules
    std::vector<State> states;

    void constructAugmentedGrammar();            // Method to construct the augmented grammar
};



#endif //LRPARSER_H
