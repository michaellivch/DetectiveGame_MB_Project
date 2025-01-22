# **Detective Game Parser**

This project implements a custom **LR Parser** to validate and process inputs for a detective game. The parser uses a **context-free grammar (CFG)** to define the structure of commands and gameplay scripts, ensuring they adhere to the game's rules.

---

## **Table of Contents**

- [Introduction](#introduction)
- [Directory Structure](#directory-structure)
- [How It Works](#how-it-works)
- [Code Files](#code-files)
    - [CFG Module](#cfg-module)
    - [LALR Parser Module](#lr-parser-module)
    - [Parsing Table Module](#parsing-table-module)
    - [Main Program](#main-program)
- [How to Run](#how-to-run)

---

## **Introduction**

This project provides a parsing system for a detective game where player inputs (or script commands) are validated and processed. The parser uses the **LR parsing technique** to ensure input conforms to the grammar and generates meaningful actions based on the game rules.

---

## **Directory Structure**

```plaintext
LALRParser/
├── include/
│   ├── cfg.h               # Grammar definitions and tokenizer
│   ├── LALR_Parser.h       # LR parser logic
│   ├── ParsingTable.h      # Parsing table generator
├── src/
│   ├── cfg.cpp             # Implementation of the CFG and tokenizer
│   ├── LALR_Parser.cpp     # Implementation of the LR parser
│   ├── ParsingTable.cpp    # Construction of Action and Goto tables
├── main.cpp                # Entry point to test the parser
├── README.md               # Project documentation
├── CMakeLists.txt          # Build configuration

```
---

## **How It Works**