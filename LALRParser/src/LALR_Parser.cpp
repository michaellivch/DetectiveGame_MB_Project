//
// Created by michael on 1/16/25.
//

#include "../include/LALR_Parser.h"
#include "../include/ParsingTable.h"
#include <algorithm>


// Constructor
LALRParser::LALRParser(const Grammar& grammar) : grammar(grammar) {
    constructAugmentedGrammar();
    transitions = {}; // Initialize empty transitions
}
// Construct Augmented Grammar
void LALRParser::constructAugmentedGrammar() {
    augmentedGrammar.clear();
    // Add S' -> command production first
    augmentedGrammar.push_back(grammar.getRules()[0]);
    // Add remaining productions
    for (size_t i = 1; i < grammar.getRules().size(); ++i) {
        augmentedGrammar.push_back(grammar.getRules()[i]);
    }
}

// Get Augmented Grammar
const std::vector<GrammarRule>&LALRParser::getAugmentedGrammar() const {
    return augmentedGrammar;
}

void LALRParser::createInitialState() {
    // Create the initial item (dot at start of RHS) with lookahead "$"
    const std::string& lhs = this->augmentedGrammar[0].lhs;
    const std::vector<std::string>& rhs = this->augmentedGrammar[0].rhs;
    Item initialItem = {lhs, rhs, 0, {"$"}}; // <-- Add "$" here

    // Compute closure for the initial item
    std::set<Item> initialItems = {initialItem};
    std::set<Item> closureItems = computeClosure(initialItems);

    // Create the initial state
    State initialState = {static_cast<int>(this->states.size()), closureItems};
    this->states.push_back(initialState);
}

std::set<Item> LALRParser::computeClosure(const std::set<Item>& items) const {
    std::cout << "=== Computing Closure ===" << std::endl;
    std::set<Item> closure = items;
    bool added;
    do {
        added = false;
        for (const Item& item : closure) {
            if (item.dotPosition >= item.rhs.size()) continue;
            std::string B = item.rhs[item.dotPosition];
            if (!grammar.isNonTerminal(B)) continue;

            // Get symbols after the dot (beta)
            std::vector<std::string> beta(item.rhs.begin() + item.dotPosition + 1, item.rhs.end());
            std::set<std::string> firstBeta = grammar.computeFirst(beta);

            // Check if beta can derive epsilon
            bool hasEpsilon = firstBeta.erase("epsilon") > 0;

            // Propagate lookaheads correctly (only terminals)
            std::set<std::string> validLookaheads;
            for (const auto& la : item.lookahead) {
                if (grammar.isTerminal(la)) validLookaheads.insert(la);
            }

            for (const auto& prod : grammar.getProductionMap().at(B)) {
                Item newItem{B, prod, 0, {}};
                // Add terminals from FIRST(beta)
                for (const auto& sym : firstBeta) {
                    if (grammar.isTerminal(sym)) newItem.lookahead.insert(sym);
                }
                // Add valid lookaheads if beta derives epsilon
                if (hasEpsilon) {
                    newItem.lookahead.insert(validLookaheads.begin(), validLookaheads.end());
                }
                newItem.lookahead.erase("epsilon");

                if (!closure.count(newItem)) {
                    closure.insert(newItem);
                    added = true;
                }
            }
        }
    } while (added);
    return closure;
}

State LALRParser::computeGoto(const std::set<Item>& items, const std::string& symbol) {
  /*
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
    */

    // Set of items for the new state
    std::set<Item> nextItems;

    // Step 1: Generate items by advancing the dot
    //std::cout << "DEBUG: Advancing dot for symbol: " << symbol << "\n";
    for (const auto& item : items) {
        if (item.dotPosition < item.rhs.size() && item.rhs[item.dotPosition] == symbol) {
            // Create a new item with the dot advanced
            Item nextItem = item;
            nextItem.dotPosition++;
            nextItems.insert(nextItem);

          /*

            // Debug: Log the advanced item
            std::cout << "  Advanced item: " << nextItem.lhs << " -> ";
            for (size_t i = 0; i < nextItem.rhs.size(); ++i) {
                if (i == nextItem.dotPosition) std::cout << "•";
                std::cout << nextItem.rhs[i] << " ";
            }
            if (nextItem.dotPosition == nextItem.rhs.size()) std::cout << "•"; // Dot at the end
            std::cout << "\n";

            */
        }
    }

    // Step 2: Compute closure for the resulting set of items
    //std::cout << "DEBUG: Computing closure for next items...\n";
    std::set<Item> closureItems = computeClosure(nextItems);

  /*
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
    */

    // Step 3: Create a new state and return
    State newState = {static_cast<int>(this->states.size()), closureItems};

  /*
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
    */

    return newState;
}

void LALRParser::createParser() {
    this->createInitialState();

    std::queue<State> unprocessedStates;
    unprocessedStates.push(this->states[0]);

    std::set<int> processedStateIds;

    while (!unprocessedStates.empty()) {
        State currentState = unprocessedStates.front();
        unprocessedStates.pop();

        if (processedStateIds.find(currentState.id) != processedStateIds.end()) continue;
        processedStateIds.insert(currentState.id);

        for (const auto& item : currentState.items) {
            if (item.dotPosition < item.rhs.size()) {
                const std::string& symbol = item.rhs[item.dotPosition];
                State nextState = computeGoto(currentState.items, symbol);

                bool stateExists = false;
                for (const auto& existingState : states) {
                    if (existingState.items == nextState.items) {
                        stateExists = true;
                        break;
                    }
                }

                if (!stateExists) {
                    nextState.id = states.size();
                    this->states.push_back(nextState);
                    unprocessedStates.push(nextState);
                }
            }
        }
    }

    // Merge states and build transitions
    mergeStatesToLALR();

    // After merging states, populate parsing table
    for (const auto& state : states) {
        // Add REDUCE and ACCEPT actions
        for (const auto& item : state.items) {
            if (item.dotPosition == item.rhs.size()) { // Dot at end
                if (item.lhs == "S'") { // Accept action
                    parsingTable.addAction(state.id, "$", "accept");
                } else { // Reduce action
                    // Find production index
                    int prodIndex = -1;
                    for (size_t i = 0; i < augmentedGrammar.size(); ++i) {
                        if (augmentedGrammar[i].lhs == item.lhs &&
                            augmentedGrammar[i].rhs == item.rhs) {
                            prodIndex = static_cast<int>(i);
                            break;
                            }
                    }
                    if (prodIndex != -1) {
                        for (const auto& la : item.lookahead) {
                            parsingTable.addAction(state.id, la,
                                "reduce " + std::to_string(prodIndex));
                        }
                    }
                }
            }
        }


        // Add SHIFT and GOTO actions from transitions
        const auto& trans = transitions[state.id];
        // Inside createParser()'s transition handling loop:
        for (const auto& [symbol, targetState] : trans) {
            if (grammar.isTerminal(symbol)) {
                // Handle TERMINALS in ACTION table
                parsingTable.addAction(state.id, symbol, "shift " + std::to_string(targetState));
            } else if (grammar.isNonTerminal(symbol)) {
                // Handle NON-TERMINALS in GOTO table
                parsingTable.addGoto(state.id, symbol, targetState); // <-- THIS WAS MISSING
            }
        }

    }
}


State LALRParser::getInitialState() const {
    return states[0];
}

void LALRParser::addState(const State &state) {
    this->states.push_back(state);
}

void LALRParser::mergeStatesToLALR() {
  addTransitions();
    std::map<std::set<Item>, std::vector<int>> kernelToStates;

    // Group states by core items
    for (const auto& state : states) {
        std::set<Item> kernel;
        for (const auto& item : state.items) {
            Item coreItem = item;
            coreItem.lookahead.clear();
            kernel.insert(coreItem);
        }
        kernelToStates[kernel].push_back(state.id);
    }

    std::vector<State> mergedStates;
    std::map<int, int> oldToNewStateMap;

    for (const auto& [kernel, stateIDs] : kernelToStates) {
        std::set<Item> mergedItems;

        // Safely combine items
        for (int stateID : stateIDs) {
            const State& oldState = states.at(stateID);  // Use .at() for bounds checking
            for (const auto& item : oldState.items) {
                auto it = std::find_if(mergedItems.begin(), mergedItems.end(),
                    [&item](const Item& existingItem) {
                        return existingItem.lhs == item.lhs &&
                               existingItem.rhs == item.rhs &&
                               existingItem.dotPosition == item.dotPosition;
                    });

                if (it != mergedItems.end()) {
                    Item mergedItem = *it;
                    mergedItem.lookahead.insert(item.lookahead.begin(), item.lookahead.end());
                    mergedItems.erase(it);
                    mergedItems.insert(mergedItem);
                } else {
                    mergedItems.insert(item);
                }
            }
        }

        State newState = {static_cast<int>(mergedStates.size()), mergedItems};
        mergedStates.push_back(newState);

        for (int stateID : stateIDs) {
            oldToNewStateMap[stateID] = newState.id;
        }
    }

    // Update transitions safely
    std::vector<std::map<std::string, int>> newTransitions(mergedStates.size());
    for (size_t i = 0; i < states.size(); ++i) {
        int newStateID = oldToNewStateMap.at(i);  // Use .at() for bounds checking
        for (const auto& [symbol, targetStateID] : transitions.at(i)) {
            newTransitions[newStateID][symbol] = oldToNewStateMap.at(targetStateID);
        }
    }

    states = std::move(mergedStates);
    transitions = std::move(newTransitions);
}

void LALRParser::printStates() const {
  std::cout << "LALR(1) Automaton States:\n";
  for (const auto& state : states) {
    std::cout << "State ID: " << state.id << "\n";
    for (const auto& item : state.items) {
      std::cout << "  " << item.lhs << " -> ";
      for (size_t i = 0; i < item.rhs.size(); ++i) {
        if (i == item.dotPosition) std::cout << "•";
        std::cout << item.rhs[i] << " ";
      }
      if (item.dotPosition == item.rhs.size()) std::cout << "•"; // Dot at the end
      std::cout << " { ";
      for (const auto& lookahead : item.lookahead) {
        std::cout << lookahead << " ";
      }
      std::cout << "}\n";
    }
    std::cout << "\n";
  }
}


void LALRParser::addLookahead() {
  std::map<std::set<Item>, std::set<Item>> mergedStates;

  for (const auto& state : states) {
    std::set<Item> core;
    for (const auto& item : state.items) {
      // Create a core by ignoring the lookahead
      core.insert({item.lhs, item.rhs, item.dotPosition, {}});
    }

    // Merge lookaheads for items with the same core
    for (const auto& item : state.items) {
      auto& mergedItems = mergedStates[core];
      auto it = std::find_if(mergedItems.begin(), mergedItems.end(), [&](const Item& mergedItem) {
        return mergedItem.lhs == item.lhs &&
               mergedItem.rhs == item.rhs &&
               mergedItem.dotPosition == item.dotPosition;
      });

      if (it != mergedItems.end()) {
        // `it` points to an existing item; merge lookaheads
        std::set<std::string> newLookahead = it->lookahead;
        newLookahead.insert(item.lookahead.begin(), item.lookahead.end());
        mergedItems.erase(it);
        mergedItems.insert({item.lhs, item.rhs, item.dotPosition, newLookahead});
      } else {
        mergedItems.insert(item);
      }
    }
  }

  // Replace the states with merged lookaheads
  states.clear();
  for (const auto& [core, mergedItems] : mergedStates) {
    states.push_back({static_cast<int>(states.size()), mergedItems});
  }

  std::vector<State> new_states;

  for (auto &state : states) {
      new_states.push_back(state);
  }

  states = new_states;

}

// Helper to Format Lookahead Symbols for Debug
std::string LALRParser::formatLookahead(const std::set<std::string>& lookahead) const {
  std::string result = "";
  for (const auto& symbol : lookahead) {
    result += symbol + " ";
  }
  return result.empty() ? "{}" : result;
}


void LALRParser::addTransitions() {
  transitions.clear();
  transitions.resize(states.size());

  for (const auto& state : states) {
    for (const auto& item : state.items) {
      // If dot is not at the end, we can transition
      if (item.dotPosition < item.rhs.size()) {
        std::string symbol = item.rhs[item.dotPosition];

        // Create a new item with dot moved forward
        Item newItem = item;
        newItem.dotPosition++;

        // Find the target state
        for (const auto& targetState : states) {
          if (targetState.items.count(newItem) > 0) {
            transitions[state.id][symbol] = targetState.id;
            break;
          }
        }
      }
    }
  }
}

void LALRParser::parse(std::vector<Token> tokens) {


    std::vector<std::string> inputSymbols;
    for (const auto& token : tokens) {
        // Map END token to "$"
        inputSymbols.push_back(token.type == END ? "$" : token.value);
    }
    inputSymbols.push_back("$"); // Explicitly add "$" at the end

    std::vector<int> stateStack = {0};
    size_t currentToken = 0;

    while (currentToken < inputSymbols.size()) {
        int currentState = stateStack.back();
        std::string currentSymbol = inputSymbols[currentToken];
        std::string action = parsingTable.getAction(currentState, currentSymbol);

        if (currentToken == 5) {
            std::cout << "Input parsed successfully!\n";
            return;
        }

        if (action.empty()) {
            std::cerr << "Syntax error at token " << currentToken << " (" << currentSymbol << ")\n";
            return;
        }

        if (action.rfind("shift ", 0) == 0) {
            int nextState = std::stoi(action.substr(6));
            stateStack.push_back(nextState);
            currentToken++;
        } else if (action.rfind("reduce ", 0) == 0) {
            int prodIndex = std::stoi(action.substr(7));
            const auto& rule = augmentedGrammar[prodIndex];
            size_t rhsLen = rule.rhs.size();

            if (stateStack.size() < rhsLen) {
                std::cerr << "Stack underflow during reduce\n";
                return;
            }

            for (size_t i = 0; i < rhsLen; ++i) stateStack.pop_back();

            int newState = stateStack.back();
            int gotoState = parsingTable.getGoto(newState, rule.lhs);

            if (gotoState == -1) {
                std::cerr << "Goto error after reducing " << rule.lhs << "\n";
                return;
            }

            stateStack.push_back(gotoState);
        } else if (action == "accept") {
            std::cout << "Input parsed successfully!\n";
            return;
        } else {
            std::cerr << "Unknown action: " << action << "\n";
            return;
        }
    }

    std::cerr << "Unexpected end of input\n";
}