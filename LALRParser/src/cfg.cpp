#include "../include/cfg.h"

#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>

// Convert string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string lower = str;
    std::ranges::transform(lower, lower.begin(), ::tolower);
    return lower;
}

// Calculate Levenshtein distance
int levenshteinDistance(const std::string& s1, const std::string& s2) {
    int m = static_cast<int>(s1.size()), n = static_cast<int>(s2.size());
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    }

    return dp[m][n];
}

// Get similarity percentage
double similarityPercentage(const std::string& s1, const std::string& s2) {
    int dist = levenshteinDistance(s1, s2);
    int maxLen = static_cast<int>(std::max(s1.size(), s2.size()));
    return 1.0 - static_cast<double>(dist) / maxLen;
}

// Tokenizer function
std::vector<Token> tokenize(const std::string& input) {
    const std::map<std::string, TokenType> keywords = {
        {"interrogate", ACTION}, {"examine", ACTION}, {"accuse", ACTION}, {"ask", ACTION}, {"investigate", ACTION}, {"blame", ACTION}, {"take", ACTION},
        {"suspect", TARGET}, {"evidence", TARGET}, {"information", TARGET}, {"window", TARGET}, {"desk", TARGET}, {"body", TARGET}, {"letter", TARGET}, {"wife", TARGET}, {"bat", TARGET},
        {"about", PREPOSITION}, {"regarding", PREPOSITION}, {"for", PREPOSITION},
        {".", TERMINAL_PUNCTUATION}, {"!", TERMINAL_PUNCTUATION}, {"?", TERMINAL_PUNCTUATION},
        {"the", ARTICLE}, {"a", ARTICLE}
    };

    constexpr double similarityThreshold = 0.75; // Minimum similarity to accept a typo

    std::vector<Token> tokens;
    size_t i = 0;

    std::string topicBuffer; // Accumulates words for a single TOPIC token

    while (i < input.size()) {
        // Skip whitespace
        if (std::isspace(input[i])) {
            ++i;
            continue;
        }

        // Extract a word
        std::string word;
        while (i < input.size() && (std::isalnum(input[i]) || input[i] == '-')) {
            word += input[i++];
        }

        // Convert word to lowercase
        word = toLowerCase(word);

        // Determine token type
        TokenType tokenType = INVALID;
        std::string bestMatch;
        double bestSimilarity = 0.0;

        for (const auto& [keyword, type] : keywords) {
            double similarity = similarityPercentage(word, keyword);
            if (similarity >= similarityThreshold && similarity > bestSimilarity) {
                bestSimilarity = similarity;
                bestMatch = keyword;
                tokenType = type;
            }
        }

        // Handle tokens based on their type
        if (tokenType == TOPIC || tokenType == INVALID) {
            if (!topicBuffer.empty()) topicBuffer += " ";
            topicBuffer += word;
        } else {
            if (!topicBuffer.empty()) {
                tokens.push_back({TOPIC, topicBuffer}); // Push the accumulated TOPIC
                topicBuffer.clear();
            }
            tokens.push_back({tokenType, bestMatch}); // Push the current token
        }

        // Handle punctuation
        if (i < input.size() && std::ispunct(input[i])) {
            std::string punctuation(1, input[i]);
            auto it = keywords.find(punctuation);
            if (it != keywords.end()) {
                if (!topicBuffer.empty()) {
                    tokens.push_back({TOPIC, topicBuffer}); // Push the accumulated TOPIC
                    topicBuffer.clear();
                }
                tokens.push_back({it->second, punctuation}); // Push punctuation token
            }
            ++i;
        }
    }

    // Push any remaining topicBuffer as the final TOPIC token
    if (!topicBuffer.empty()) {
        tokens.push_back({TOPIC, topicBuffer});
    }

    // Add END token
    tokens.push_back({END, ""});

    return tokens;
}


// Grammar initialization
Grammar::Grammar() {
    rules = {
        {"S'", {"command"}},
        {"command", {"action", "target", "preposition", "topic", "TERMINAL_PUNCTUATION"}},
        {"action", {"interrogate"}},
        {"action", {"examine"}},
        {"action", {"ask"}},
        {"action", {"accuse"}},
        {"action", {"investigate"}},
        {"action", {"blame"}},
        {"action", {"take"}},
        {"target", {"suspect"}},
        {"target", {"evidence"}},
        {"target", {"information"}},
        {"target", {"suspect"}},
        {"target", {"window"}},
        {"target", {"desk"}},
        {"target", {"body"}},
        {"target", {"letter"}},
        {"target", {"wife"}},
        {"target", {"bat"}},
        {"preposition", {"about"}},
        {"preposition", {"regarding"}},
        {"preposition", {"for"}},
        {"topic", {"cop"}},
        {"topic", {"house"}},
        {"topic", {"divorce"}},
        {"topic", {"husband"}},
        {"topic", {"murder"}},
        {"topic", {"clues"}},
        {"topic", {"safety"}},
        {"TERMINAL_PUNCTUATION", {"."}},
        {"TERMINAL_PUNCTUATION", {"!"}},
        {"TERMINAL_PUNCTUATION", {"?"}}
    };
    // First pass: Collect all non-terminals (LHS of rules)
    for (const auto& rule : rules) {
        nonTerminals.insert(rule.lhs);
    }

    // Second pass: Identify terminals (explicit list + punctuation)
    terminals = {
        // Terminals from productions
        "interrogate", "examine", "accuse", "ask", "investigate", "blame", "take",
        "evidence", "suspect", "information", "window", "desk", "body", "wife", "bat",
        "about", "regarding", "for",
        "cop", "letter", "house", "husband", "murder", "clues", "divorce", "safety",
        // Punctuation
        ".", "!", "?"
    };

    // Third pass: Build production map
    for (const auto& rule : rules) {
        productionMap[rule.lhs].push_back(rule.rhs);

        // Sanity check: Ensure RHS symbols are classified correctly
        for (const auto& symbol : rule.rhs) {
            if (!nonTerminals.contains(symbol) &&
                !terminals.contains(symbol) &&
                symbol != "TERMINAL_PUNCTUATION") {
                std::cerr << "Symbol '" << symbol << "' is neither terminal nor non-terminal!\n";
                }
        }
    }

    computeFollowSets();
}

const std::vector<GrammarRule>& Grammar::getRules() const {
    return rules;
}

const std::map<std::string, std::vector<std::vector<std::string>>> & Grammar::getProductionMap() const {
    return productionMap;
}

const std::set<std::string>& Grammar::getTerminals() const {
    return terminals;
}

const std::set<std::string>& Grammar::getNonTerminals() const {
    return nonTerminals;
}

const std::set<std::string>& Grammar::getSymbols() const {
    static std::set<std::string> symbols;
    if (symbols.empty()) {
        symbols.insert(terminals.begin(), terminals.end());
        symbols.insert(nonTerminals.begin(), nonTerminals.end());
    }
    return symbols;
}

bool Grammar::isTerminal(const std::string& symbol) const {
    return terminals.contains(symbol);
}

bool Grammar::isNonTerminal(const std::string& symbol) const {
    return nonTerminals.contains(symbol);
}

std::set<std::string> Grammar::computeFirst(const std::vector<std::string>& symbols) const {
    std::set<std::string> firstSet;

    for (const auto& symbol : symbols) {
        if (isTerminal(symbol)) {
            firstSet.insert(symbol);
            break;
        } else if (isNonTerminal(symbol)) {
            const auto& productions = productionMap.at(symbol);
            bool hasEpsilon = false;

            // Process all productions to collect FIRST terminals
            for (const auto& production : productions) {
                auto firstOfProduction = computeFirst(production);
                bool productionHasEpsilon = firstOfProduction.count("epsilon") > 0;

                // Add terminals from this production's FIRST
                for (const auto& s : firstOfProduction) {
                    if (s != "epsilon") firstSet.insert(s);
                }

                // Track if any production can derive epsilon
                if (productionHasEpsilon) hasEpsilon = true;
            }

            // If no production derives epsilon, stop further symbol processing
            if (!hasEpsilon) break;

            // Add epsilon if all symbols so far can derive epsilon
            firstSet.insert("epsilon");
        }

        // Stop if current symbol doesn't contribute epsilon
        if (!firstSet.count("epsilon")) break;
        firstSet.erase("epsilon"); // Remove epsilon to check next symbol
    }

    return firstSet;
}

void Grammar::computeFollowSets() {
    for (const auto& nonTerminal : nonTerminals) {
        followSets[nonTerminal] = {};
    }
    followSets["S'"].insert("$");

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& rule : rules) {
            const auto& lhs = rule.lhs;
            const auto& rhs = rule.rhs;
            for (size_t i = 0; i < rhs.size(); ++i) {
                const std::string& symbol = rhs[i];
                if (isNonTerminal(symbol)) {
                    std::set<std::string> followSet;

                    // Case 1: Symbol is followed by something
                    if (i + 1 < rhs.size()) {
                        std::string nextSymbol = rhs[i + 1];
                        if (isTerminal(nextSymbol)) {
                            followSet.insert(nextSymbol);
                        } else {
                            // Add FIRST of nextSymbol (non-terminal), excluding epsilon
                            auto& productions = productionMap.at(nextSymbol);
                            for (const auto& prod : productions) {
                                auto first = computeFirst(prod);
                                followSet.insert(first.begin(), first.end());
                            }
                            followSet.erase("epsilon");

                            // If FIRST contains epsilon, add FOLLOW of lhs
                            bool hasEpsilon = false;
                            for (const auto& prod : productions) {
                                if (prod.empty() || (prod.size() == 1 && prod[0] == "epsilon")) {
                                    hasEpsilon = true;
                                    break;
                                }
                            }
                            if (hasEpsilon) {
                                followSet.insert(followSets[lhs].begin(), followSets[lhs].end());
                            }
                        }
                    }

                    // Case 2: Symbol is at the end of production
                    if (i + 1 == rhs.size()) {
                        followSet.insert(followSets[lhs].begin(), followSets[lhs].end());
                    }

                    // Update the follow set for the symbol
                    size_t oldSize = followSets[symbol].size();
                    followSets[symbol].insert(followSet.begin(), followSet.end());
                    if (followSets[symbol].size() > oldSize) {
                        changed = true;
                    }
                }
            }
        }
    }
}

const std::set<std::string>& Grammar::getFollowSet(const std::string& nonTerminal) const {
    return followSets.at(nonTerminal);
}

