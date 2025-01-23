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
        T.image = transitions["imageReplacement"];
        T.message = transitions["textReplacement"];
        T.target = transitions["target"];
        T.topic = transitions["topic"];
        for (const auto& replacementsymbol : transitions["replacement"]){
            T.replacement.push_back(replacementsymbol);
        }
        transitionTable.push_back(T);
    }
    initialState = j["StartState"];
    currentState = initialState;
    stackInitialState = j["StartStack"];
    for (auto state : j["AcceptStates"]) {
        finalStates.push_back(state);
    }

    stacks["MainStack"].push_back(stackInitialState);
    stacks["ImageStack"] = {"../Assets/Scenes/house.jpg"};
    stacks["TextStack"] = {"You have been called to go investigate a murder"};
}


bool PDA::processInput(const string& target, const string& topic) {
  // First check epsilon transitions
  if (target == "epsilon" && topic == "epsilon") {
    for (const auto& transition : transitionTable) {
      if (transition.target == "epsilon" &&
          transition.topic == "epsilon" &&
          transition.currentState == currentState &&
          !stacks[transition.stack].empty() &&
          stacks[transition.stack].back() == transition.stackTop) {

        // Apply transition
        stacks[transition.stack].pop_back();
        for (auto it = transition.replacement.rbegin(); it != transition.replacement.rend(); ++it) {
          stacks[transition.stack].push_back(*it);
        }
        stacks["ImageStack"] = {transition.image};
        stacks["TextStack"] = {transition.message};
        currentState = transition.nextState;
        printStatus();
        return true;
      }
    }
  }

  // Check tokenized transitions
  for (const auto& transition : transitionTable) {
    if (transition.currentState == currentState &&
        transition.target == target &&
        transition.topic == topic &&
        !stacks[transition.stack].empty() &&
        stacks[transition.stack].back() == transition.stackTop) {

      // Apply transition
      stacks[transition.stack].pop_back();
      for (auto it = transition.replacement.rbegin(); it != transition.replacement.rend(); ++it) {
        stacks[transition.stack].push_back(*it);
      }
      stacks["ImageStack"] = {transition.image};
      stacks["TextStack"] = {transition.message};
      currentState = transition.nextState;
      printStatus();
      return true;
    }
  }

  cerr << "No valid transition found for target: " << target << " and topic: " << topic << endl;
  return false;
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

bool PDA::isFinalState() const {
  for (auto state : finalStates) {
    if (currentState == state) {
      return true;
    }
  }
  return false;
}

bool PDA::hasEpsilonTransition() const {
  for (const auto& transition : transitionTable) {
    if (transition.currentState == currentState && transition.input == "epsilon") {
      return true;
    }
  }
  return false;
}


std::vector<std::pair<sf::FloatRect, std::string>> PDA::getHoverRegions() {
  std::vector<std::pair<sf::FloatRect, std::string>> regions;
  std::string currentImage = stacks.at("ImageStack")[0];

  float offset = 50.f;

  if (currentImage.find("../Assets/Scenes/house.jpg") != std::string::npos) {
    regions = {
        {sf::FloatRect(170, 300, 250-170, 370-300), "Ask suspect about husband."},
        {sf::FloatRect(260, 320, 310-260, 400-320), "Examine evidence about murder."},
        {sf::FloatRect(345, 350, 420-345, 470-350), "Ask information about body."},
    };
  }
  else if (currentImage.find("../Assets/Scenes/inside.png") != std::string::npos) {
    regions = {
        {sf::FloatRect(0, 377, 183, 401-377), "Investigate desk for clues."},
        {sf::FloatRect(265, 139, 500-265, 380-139), "Investigate window for clues."},
        {sf::FloatRect(30, 512, 352-30, 677-512), "Investigate body for clues."}
    };
  }
  else if (currentImage.find("../Assets/Scenes/paper_on_desk.png") != std::string::npos) {
    regions = {
        {sf::FloatRect(101, 208, 531-101, 666-208), "Investigate letter about divorce."},
    };
  }
  else if (currentImage.find("../Assets/Scenes/room_where_wife.png") != std::string::npos) {
    regions = {
        {sf::FloatRect(256, 633, 465-256, 656-633), "Take bat for safety"},
        {sf::FloatRect(274, 258, 353-274, 515-258), "Blame wife for murder"}
    };
  }
  return regions;
}

void PDA::reset() {
  currentState = initialState;
  stacks["MainStack"] = {stackInitialState};
  stacks["ImageStack"] = {"../Assets/Scenes/house.jpg"};
  stacks["TextStack"] = {"You have been called to go investigate a"};
}
