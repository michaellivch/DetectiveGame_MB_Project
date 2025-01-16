//
// Created by jeremi on 8/10/24.
//

#include "PDA.h"
#include "json.hpp"
#include <fstream>
#include "iostream"
using json = nlohmann::json;

PDA::PDA(const string& jsonfile){
    std::ifstream input(jsonfile);
    json j;
    input >> j;
    for (const auto& symbol : j["Alphabet"]) {
        Alphabet.push_back(symbol);
    }
    for (const auto& symbol : j["StackAlphabet"]) {
        stackAlphabet.push_back(symbol);
    }
    for (auto states : j["States"]) {
        allStates.push_back(states);
    }
    for (auto states : j["FinalStates"]) {
            finalStates.push_back(states);
    }
    for (auto transitions : j["Transitions"]){
        Transition T;
        T.currentState = transitions["from"];
        T.nextState = transitions["to"];
        T.input = transitions["input"];
        T.stackTop = transitions["stacktop"];
        for (const auto& replacementsymbol : transitions["replacement"]){
            T.replacement.push_back(replacementsymbol);
        }
        transitionTable.push_back(T);
    }
    initialState = j["StartState"];
    stackInitialState = j["StartStack"];
}
string PDA::getNextState(const string &currentState, const string &input,
                         const string &stackTop) {
    for (const auto &transition : transitionTable) {
      if (transition.currentState == currentState && transition.input == input &&
          transition.stackTop == stackTop) {
        return transition.nextState;
      }
    }
    cerr << "No transition found for state " << currentState << ", input " << input
         << ", stack top " << stackTop << endl;
    return "";
}

string PDA::getState(string input) {
    string currentState = initialState;
    for (auto i : input){
      string character = to_string(i);
      for (auto transition : transitionTable){
        if (transition.input == character && transition.currentState == currentState ){
          currentState = transition.nextState;
        }
      }
    }
    return currentState;
}
