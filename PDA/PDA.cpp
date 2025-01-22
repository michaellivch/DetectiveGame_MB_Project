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
        T.image = transitions["image"];
        for (const auto& replacementsymbol : transitions["replacement"]){
            T.replacement.push_back(replacementsymbol);
        }
        transitionTable.push_back(T);
    }
    initialState = j["StartState"];
    currentState = initialState;
    stackInitialState = j["StartStack"];

    stacks["MainStack"].push_back(stackInitialState);
    stacks["ImageStack"] = {"../Scenes/house.jpg"}; // Kan leeg beginnen
}

bool PDA::processInput(const string& input) {
  for (const auto& transition : transitionTable) {
    if (transition.currentState == currentState && transition.input == input &&
        !stacks[transition.stack].empty() &&
        stacks[transition.stack].back() == transition.stackTop) {
      // Debugging: print huidige status
      cout << "Processing transition: " << transition.currentState << " -> "
           << transition.nextState << " on input: " << input << endl;
      // Werk de stack bij
      stacks[transition.stack].pop_back();
      for (auto it = transition.replacement.rbegin(); it != transition.replacement.rend(); ++it) {
        stacks[transition.stack].push_back(*it);
      }
      stacks["ImageStack"] = {transition.image};
      // Verander de huidige status
      currentState = transition.nextState;
      // Debugging: toon nieuwe stack en status
      printStatus();
      return true; // Succesvolle transitie
    }
  }
  cerr << "No valid transition found for input: " << input << endl;
  return false; // Geen geldige transitie
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
void PDA::printStatus() const {
  cout << "Current State: " << currentState << endl;
  for (const auto& [stackName, stack] : stacks) {
    cout << stackName << ": ";
    for (const auto& symbol : stack) {
      cout << symbol << " ";
    }
    cout << endl;
  }
}

string PDA::getCurrentState() const {
  return currentState;
}
vector<string> PDA::getStack(const string &stackName) const {
  return stacks.at(stackName);
}
vector<PDA::Transition> PDA::getTransitions() { return transitionTable; }

bool PDA::isAccepted() const {
  return find(finalStates.begin(), finalStates.end(), currentState) != finalStates.end();
}
