#ifndef CFG_H
#define CFG_H

#include <vector>
#include <string>
#include <map>

// Token types
enum TokenType { ACTION, TARGET, PREPOSITION, TOPIC, TERMINAL_PUNCTUATION, INVALID, END };

// Token structure
struct Token {
    TokenType type;
    std::string value;
};

// Grammar rule structure
struct GrammarRule {
    std::string lhs;                   // Left-hand side of the rule
    std::vector<std::string> rhs;      // Right-hand side of the rule (vector of symbols)
};

// Tokenizer function
std::vector<Token> tokenize(const std::string& input);

// CFG-based parser class
class CFGParser {
public:
    // Lower case for input handling
    std::string toLowerCase(const std::string& str);

    // Calculate Levenshtein distance
    int levenshteinDistance(const std::string& s1, const std::string& s2);

    // Get similarity percentage
    double similarityPercentage(const std::string& s1, const std::string& s2);

    std::vector<GrammarRule> grammarRules;  // Set of grammar rules
    std::map<std::string, std::vector<std::vector<std::string>>> grammarMap; // Efficient grammar lookup
};

#endif // CFG_H
