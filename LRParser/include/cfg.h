#ifndef CFG_H
#define CFG_H

#include <string>
#include <vector>
#include <map>
#include <set>

// Token Types
enum TokenType {
    ACTION,
    TARGET,
    ARTICLE,
    PREPOSITION,
    TOPIC,
    TERMINAL_PUNCTUATION,
    INVALID,
    END
};

// Token Structure
struct Token {
    TokenType type;
    std::string value;
};

// Grammar Rule Structure
struct GrammarRule {
    std::string lhs;                  // Left-hand side of the rule
    std::vector<std::string> rhs;     // Right-hand side of the rule
};

// Grammar Class
class Grammar {
public:
    Grammar(); // Constructor to initialize grammar rules

    // Accessor Methods
    [[nodiscard]] const std::vector<GrammarRule>& getRules() const;                                     // Get all grammar rules
    [[nodiscard]] const std::map<std::string, std::vector<std::vector<std::string>>>& getProductionMap() const;
    [[nodiscard]] const std::set<std::string>& getTerminals() const;                                    // Get terminal symbols
    [[nodiscard]] const std::set<std::string>& getNonTerminals() const;                                 // Get non-terminal symbols
    [[nodiscard]] const std::set<std::string>& getSymbols() const;                                      // Get all grammar symbols
    [[nodiscard]] const std::set<std::string>& getFollowSet(const std::string& nonTerminal) const;      // Get Follow set for a non-terminal

    // Utility Methods
    [[nodiscard]] bool isTerminal(const std::string& symbol) const;     // Check if a symbol is a terminal
    [[nodiscard]] bool isNonTerminal(const std::string& symbol) const;  // Check if a symbol is a non-terminal

    std::set<std::string> computeFirst(const std::vector<std::string>& symbols) const {
      std::set<std::string> firstSet;

      for (const auto& symbol : symbols) {
        if (isTerminal(symbol)) {
          firstSet.insert(symbol); // Terminals go directly into FIRST set
          break;
        } else if (isNonTerminal(symbol)) {
          const auto& productions = productionMap.at(symbol);
          for (const auto& production : productions) {
            auto firstOfProduction = computeFirst(production);
            firstSet.insert(firstOfProduction.begin(), firstOfProduction.end());

            // If the production doesn't produce epsilon, stop
            if (!firstOfProduction.contains("epsilon")) {
              break;
            }
          }
        }

        // If the symbol produces epsilon, continue to the next symbol
        if (!firstSet.contains("epsilon")) {
          break;
        }
      }

      return firstSet;
    }


private:
    std::vector<GrammarRule> rules;                                 // List of grammar rules
    std::set<std::string> terminals;                                // Set of terminal symbols
    std::set<std::string> nonTerminals;                             // Set of non-terminal symbols
    std::map<std::string, std::vector<std::vector<std::string>>> productionMap;  // Non-terminal -> RHS productions
    std::map<std::string, std::set<std::string>> followSets;        // Follow sets for non-terminals

    void computeFollowSets(); // Internal method to calculate Follow sets
};

// Tokenizer Function
std::vector<Token> tokenize(const std::string& input);

// Utility Functions
std::string toLowerCase(const std::string& str);      // Convert string to lowercase
int levenshteinDistance(const std::string& s1, const std::string& s2); // Calculate Levenshtein distance
double similarityPercentage(const std::string& s1, const std::string& s2); // Get similarity percentage

#endif // CFG_H
