//
// Created by michael on 1/16/25.
//

#include "../include/lrparser.h"


// Constructor
LRParser::LRParser(const Grammar& grammar) : grammar(grammar) {
    constructAugmentedGrammar();
}
// Construct Augmented Grammar
void LRParser::constructAugmentedGrammar() {

    // Copy all original grammar rules
    for (const auto& rule : grammar.getRules()) {
        augmentedGrammar.push_back(rule);
    }
}

// Get Augmented Grammar
const std::vector<GrammarRule>& LRParser::getAugmentedGrammar() const {
    return augmentedGrammar;
}

void LRParser::createInitialState() {
    // Create the initial item (dot at the start of RHS)
    const std::string& lhs = this->augmentedGrammar[0].lhs;
    const std::vector<std::string>& rhs = this->augmentedGrammar[0].rhs;
    Item initialItem = {lhs, rhs, 0};

    // Compute the closure for the initial item
    std::set<Item> initialItems = {initialItem};
    std::set<Item> closureItems = computeClosure(initialItems);

    // Create the initial state
    State initialState = {static_cast<int>(this->states.size()), closureItems};

    // Add the state to the list of states
    this->states.push_back(initialState);
}

std::set<Item> LRParser::computeClosure(const std::set<Item> &items) const {
    // Initialize the closure with the given items
    std::set<Item> closure = items;

    // Keep track of whether we are adding new items
    bool addedNewItem = true;

    while (addedNewItem) {
        addedNewItem = false;

        // Iterate through the current closure
        for (const auto& item : closure) {
            // Check if the dot is not at the end of the RHS
            if (item.dotPosition < item.rhs.size()) {
                const std::string& symbolAfterDot = item.rhs[item.dotPosition];

                // If the symbol after the dot is a non-terminal
                if (grammar.isNonTerminal(symbolAfterDot)) {
                    // Add all productions of this non-terminal
                    for (const auto& rule : grammar.getProductionMap().at(symbolAfterDot)) {
                        Item newItem = {symbolAfterDot, rule, 0}; // Dot at the beginning

                        // If the new item is not already in the closure
                        if (closure.find(newItem) == closure.end()) {
                            closure.insert(newItem);
                            addedNewItem = true;
                        }
                    }
                }
            }
        }
    }

    return closure;
}


