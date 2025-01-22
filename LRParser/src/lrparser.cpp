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

State LRParser::computeGoto(const std::set<Item>& items, const std::string& symbol) {
    // Debug: Log input items and symbol
    std::cout << "DEBUG: computeGoto called with symbol: " << symbol << "\n";
    std::cout << "DEBUG: Current items:\n";
    for (const auto& item : items) {
        std::cout << "  " << item.lhs << " -> ";
        for (size_t i = 0; i < item.rhs.size(); ++i) {
            if (i == item.dotPosition) std::cout << "•";
            std::cout << item.rhs[i] << " ";
        }
        if (item.dotPosition == item.rhs.size()) std::cout << "•"; // Dot at the end
        std::cout << "\n";
    }

    // Set of items for the new state
    std::set<Item> nextItems;

    // Step 1: Generate items by advancing the dot
    std::cout << "DEBUG: Advancing dot for symbol: " << symbol << "\n";
    for (const auto& item : items) {
        if (item.dotPosition < item.rhs.size() && item.rhs[item.dotPosition] == symbol) {
            // Create a new item with the dot advanced
            Item nextItem = item;
            nextItem.dotPosition++;
            nextItems.insert(nextItem);

            // Debug: Log the advanced item
            std::cout << "  Advanced item: " << nextItem.lhs << " -> ";
            for (size_t i = 0; i < nextItem.rhs.size(); ++i) {
                if (i == nextItem.dotPosition) std::cout << "•";
                std::cout << nextItem.rhs[i] << " ";
            }
            if (nextItem.dotPosition == nextItem.rhs.size()) std::cout << "•"; // Dot at the end
            std::cout << "\n";
        }
    }

    // Step 2: Compute closure for the resulting set of items
    std::cout << "DEBUG: Computing closure for next items...\n";
    std::set<Item> closureItems = computeClosure(nextItems);

    // Debug: Log closure result
    std::cout << "DEBUG: Closure items:\n";
    for (const auto& item : closureItems) {
        std::cout << "  " << item.lhs << " -> ";
        for (size_t i = 0; i < item.rhs.size(); ++i) {
            if (i == item.dotPosition) std::cout << "•";
            std::cout << item.rhs[i] << " ";
        }
        if (item.dotPosition == item.rhs.size()) std::cout << "•"; // Dot at the end
        std::cout << "\n";
    }

    // Step 3: Create a new state and return
    State newState = {static_cast<int>(this->states.size()), closureItems};

    // Debug: Log the new state
    std::cout << "DEBUG: Created new state with ID: " << newState.id << "\n";
    std::cout << "DEBUG: State items:\n";
    for (const auto& item : newState.items) {
        std::cout << "  " << item.lhs << " -> ";
        for (size_t i = 0; i < item.rhs.size(); ++i) {
            if (i == item.dotPosition) std::cout << "•";
            std::cout << item.rhs[i] << " ";
        }
        if (item.dotPosition == item.rhs.size()) std::cout << "•"; // Dot at the end
        std::cout << "\n";
    }

    return newState;
}

void LRParser::createParser() {
    this->createInitialState(); // Generate the initial state

    // Use a queue to process states incrementally
    std::queue<State> unprocessedStates;
    unprocessedStates.push(this->states[0]); // Start with the initial state

    // Track already processed states
    std::set<int> processedStateIds;

    while (!unprocessedStates.empty()) {
        State currentState = unprocessedStates.front();
        unprocessedStates.pop();

        // Check if the current state is already processed
        if (processedStateIds.find(currentState.id) != processedStateIds.end()) {
            continue; // Skip already processed states
        }
        processedStateIds.insert(currentState.id);

        // Process items in the current state
        for (const auto& item : currentState.items) {
            // Check if dotPosition is valid
            if (item.dotPosition < item.rhs.size()) {
                // Get the symbol after the dot
                const std::string& symbol = item.rhs[item.dotPosition];

                // Compute GOTO for this symbol
                State nextState = computeGoto(currentState.items, symbol);

                // Add the resulting state if it's new
                // Check if the resulting state already exists
                bool stateExists = false;
                for (const auto& existingState : this->states) {
                    if (existingState.items == nextState.items) {
                        stateExists = true;
                        break;
                    }
                }

                // Add the new state if it doesn't exist
                if (!stateExists) {
                    nextState.id = static_cast<int>(this->states.size());
                    this->states.push_back(nextState);
                    unprocessedStates.push(nextState); // Add to unprocessed queue
                }
            }
        }
    }

}

State LRParser::getInitialState() const {
    return states[0];
}

void LRParser::addState(const State &state) {
    this->states.push_back(state);
}

void LRParser::printStates() const {
    std::cout << "LR(0) Automaton States:\n";
    for (const auto& state : this->states) {
        std::cout << "State ID: " << state.id << "\n";
        for (const auto& item : state.items) {
            std::cout << "  " << item.lhs << " -> ";
            for (size_t i = 0; i < item.rhs.size(); ++i) {
                if (i == item.dotPosition) std::cout << "•"; // Print dot at the correct position
                std::cout << item.rhs[i] << " ";
            }
            if (item.dotPosition == item.rhs.size()) std::cout << "•"; // Dot at the end
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void LRParser::addLookahead() {

}
