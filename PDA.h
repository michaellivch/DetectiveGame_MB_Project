//
// Created by jeremi on 8/10/24.
//

#ifndef CFG_PDA_H
#define CFG_PDA_H
#include "vector"
#include "string"
using namespace std;
class PDA {

private:
    struct Transition {
        string currentState;
        string input;
        string nextState;
        string stackTop; //first which stack of the MPDA, then the symbol
        vector<string> replacement = {};
    };
    string initialState;
    vector<Transition> transitionTable;
    vector<string> allStates;
    vector<string> Alphabet;
    vector<string> stackAlphabet;
    vector<string> finalStates;
    string stackInitialState;

public:
    PDA(const string& jsonfile);
    string getNextState(const string& currentState, const string& input, const string& stackTop);
    string getState(string input);
};


#endif //CFG_PDA_H
