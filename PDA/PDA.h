//
// Created by jeremi on 8/10/24.
//

#ifndef CFG_PDA_H
#define CFG_PDA_H
#include "map"
#include "string"
#include "vector"
#include <SFML/Graphics.hpp>
using namespace std;
class PDA {

private:
private:
    map<string, vector<string>> stacks;
    struct Transition {
        string stack = "MainStack";
        string currentState;
        string input;
        string nextState;
        string stackTop; //first which stack of the MPDA, then the symbol
        string image;
        string message;
        vector<string> replacement = {};
        string topic;
        string target;
    };
    string initialState;
    vector<Transition> transitionTable;
    vector<string> allStates;
    vector<string> Alphabet;
    vector<string> stackAlphabet;
    vector<string> finalStates;
    string stackInitialState;
    string currentState;

    void printStatus() const;

public:
    PDA(const string& jsonfile);
    string getNextState(const string& currentState, const string& input, const string& stackTop);
    string getState(string input);
    bool processInput(const string& target, const string& topic);
    string getCurrentState() const;
    vector<string> getStack(const string& stackName) const;
    vector<Transition> getTransitions();
    bool isFinalState() const;
    bool hasEpsilonTransition() const;
    std::vector<std::pair<sf::FloatRect, std::string>> getHoverRegions();
    void reset();
};


#endif //CFG_PDA_H
