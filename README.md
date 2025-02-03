# **Detective Game Parser**

This project implements a custom **LALR Parser** to validate and process inputs for a detective game. The parser uses a **context-free grammar (CFG)** to define the structure of commands and gameplay scripts, ensuring they adhere to the game's rules.

---

## **Table of Contents**

- [Introduction](#introduction)
- [Directory Structure](#directory-structure)
- [How It Works](#how-it-works)
- [Code Files](#code-files)
    - [CFG Module](#cfg-module)
    - [LALR Parser Module](#lalr-parser-module)
    - [Parsing Table Module](#parsing-table-module)
    - [Main Program](#main-program)
- [How to Run](#how-to-run)

---

## **Introduction**

This project provides a parsing system for a detective game where player inputs (or script commands) are validated and processed. The parser uses the **LR parsing technique** to ensure input conforms to the grammar and generates meaningful actions based on the game rules.

The core gameplay mechanic involves:
1. Exploring a scene and observing clues.
2. Interacting with these clues by typing a corresponding command.
3. The parser validates the command and allows similar input variations as long as they conform to the grammar.

---

## **Directory Structure**

```plaintext
DetectiveGameParser/
├── Assets/
│   ├── Scenes/
│   │   ├── font.ttf             # Font file for text rendering
│   │   ├── game.json            # Scene and game configuration
├── Game/
│   ├── State.cpp                # Handles game state transitions
│   ├── State.h
│   ├── StateManager.cpp         # Manages multiple game states
│   ├── StateManager.h
├── LALRParser/
│   ├── include/
│   │   ├── cfg.h                # Grammar definitions and tokenizer
│   │   ├── LALR_Parser.h        # LR parser logic
│   │   ├── ParsingTable.h       # Parsing table generator
│   ├── src/
│   │   ├── cfg.cpp              # Implementation of the CFG and tokenizer
│   │   ├── LALR_Parser.cpp      # Implementation of the LR parser
│   │   ├── ParsingTable.cpp     # Construction of Action and Goto tables
├── PDA/
│   ├── PDA.cpp                  # Pushdown automaton logic
│   ├── json.hpp                 # JSON handling library
├── main.cpp                     # Entry point to test the parser
├── CMakeLists.txt               # Build configuration
├── README.md                    # Project documentation
```

---

## **How It Works**

### **Game Mechanic**

The detective game immerses players in interactive crime scenes where they need to examine evidence and type commands to progress. The game provides hints for what actions are possible, and players must construct grammatically correct commands based on these hints.

For example:
- **Scene:** You are at a crime scene with a notebook clue.
- **Hint:** "Examine evidence about the notebook."
- **Input:** Players can type "Examine evidence about notebook" or similar variations, which the parser validates and processes.

### **Parser Role**
The parser ensures the following:
- **Input Validation:** Checks if commands adhere to the game's grammar.
- **Error Handling:** Allows small input variations while ensuring commands remain within the rules.
- **Gameplay Integration:** Translates valid inputs into game actions.

---

## **Code Files**

### **CFG Module**
- **Files:** `cfg.h`, `cfg.cpp`
- **Purpose:** Defines the context-free grammar and tokenizer. It breaks input strings into tokens and validates them against grammar rules.

### **LALR Parser Module**
- **Files:** `LALR_Parser.h`, `LALR_Parser.cpp`
- **Purpose:** Implements the LALR parsing logic to validate tokenized inputs and manage parsing states.

### **Parsing Table Module**
- **Files:** `ParsingTable.h`, `ParsingTable.cpp`
- **Purpose:** Generates action and goto tables for the parser, which guide the parsing process.

### **Main Program**
- **File:** `main.cpp`
- **Purpose:** Entry point for running and testing the parser. It demonstrates how commands are processed and validated in the game context.

---

## **How to Run**

### **Requirements**
- **C++ Compiler**: Compatible with modern C++ standards (e.g., GCC, Clang, or MSVC).
- **SFML Library**: Used for rendering the game's graphical interface and handling input.

### **Steps**
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd DetectiveGameParser
   ```

2. Install dependencies (e.g., SFML).
   ```bash
   sudo apt install libsfml-dev   # For Ubuntu-based systems
   ```

3. Build the project using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. Run the executable:
   ```bash
   ./DetectiveGameParser
   ```

### **Testing the Parser**
- Use `main.cpp` to test individual commands and validate their conformity to the grammar.
- Modify or extend `cfg.h` to customize the game's grammar as needed.

---

## **Screenshots and Examples**

### **Example Scene**
![Alt Text](Assets/house.jpg)
- **Player Task:** Examine clues to identify evidence.
- **Hint:** "Examine evidence about murder."
- **Player Input:** "Examine evidence about husband."

### **Correct Input Response**
The parser accepts grammatically correct inputs and triggers the corresponding game action. It also provides feedback for invalid commands.

---


## **Contributors**
- Michael Livchits
- Jeremi Lorek

For questions or feedback, please contact michael.livchits@hotmail.com.

