#include "../include/cfg.h"

#include <algorithm>
#include <sstream>
#include <cctype>

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
        {"interrogate", ACTION}, {"examine", ACTION}, {"accuse", ACTION},
        {"suspect", TARGET}, {"evidence", TARGET},
        {"about", PREPOSITION}, {"regarding", PREPOSITION},
        {".", TERMINAL_PUNCTUATION}, {"!", TERMINAL_PUNCTUATION}, {"?", TERMINAL_PUNCTUATION}
    };

    constexpr double similarityThreshold = 0.75; // Minimum similarity to accept a typo

    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        // Skip whitespace
        if (std::isspace(input[i])) {
            ++i;
            continue;
        }

        // Extract word
        std::string word;
        while (i < input.size() && (std::isalnum(input[i]) || input[i] == '-')) {
            word += input[i++];
        }

        // Convert word to lowercase
        word = toLowerCase(word);

        // Find exact match or closest match
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

        if (tokenType != INVALID) {
            tokens.push_back({tokenType, bestMatch});
        } else if (!word.empty()) {
            tokens.push_back({TOPIC, word}); // Assume unrecognized words are part of the topic
        }

        // Handle punctuation
        if (i < input.size() && std::ispunct(input[i])) {
            std::string punctuation(1, input[i]);
            auto it = keywords.find(punctuation);
            if (it != keywords.end()) {
                tokens.push_back({it->second, punctuation});
            } else {
                tokens.push_back({INVALID, punctuation});
            }
            ++i;
        }
    }

    tokens.push_back({END, ""}); // End marker
    return tokens;
}