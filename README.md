# BCSF24A024_DSA_A1
Repository for the Postfix Convertor and Calculator, Assignment 1 for the DSA Course for BCSF24A, FCIT, Spring Semester 2026


## Mathematical Expression Evaluator (Infix to Postfix)

A robust C++ CLI application that tokenizes, validates, converts, and evaluates infix mathematical expressions containing numbers, variables, operators (`+`, `-`, `*`, `/`), and mismatched bracket types (`()`, `[]`, `{}`).

---

## 🚀 Features

* **Comprehensive Tokenization:** Parses integers, multi-character variables (including underscores), operators, and brackets while ignoring whitespace.
* **Strict Syntax Validation:** * Detects out-of-order tokens (e.g., two consecutive operators, an operator starting an expression).
  * Validates bracket nesting and balancing across multiple types: `()`, `[]`, and `{}` (e.g., detects `( [ ) ]` as invalid).
* **Shunting-Yard Conversion:** Implements Dijkstra's Shunting-Yard algorithm to reliably convert standard infix expressions to Postfix (Reverse Polish Notation).
* **Dynamic Variable Resolution:** Automatically detects unique variables in the expression and prompts the user for their values at runtime.
* **Safe Runtime Evaluation:** Evaluates the generated postfix expression using a stack and catches runtime errors like division by zero.

---

## 🛠️ Architecture & Token Types

The program represents every component of the expression string as a `Token` structure defined by a numerical `type`:

| Type ID | Token Classification | Examples |
| :---: | --- | --- |
| **0** | Numbers (Integers) | `42`, `100`, `0` |
| **1** | Variables | `x`, `total_sum`, `_var1` |
| **2** | Binary Operators | `+`, `-`, `*`, `/` |
| **3** | Left Brackets | `(`, `[`, `{` |
| **4** | Right Brackets | `)`, `]`, `}` |

---

## 💻 How It Works (Pipeline)

1. **`tokenize()`**: Scans the input string character-by-character to group them into distinct `Token` objects.
2. **`validate()`**: Runs structural syntax checks and matches tracking brackets using a `std::stack`.
3. **`convert()`**: Uses the **Shunting-Yard Algorithm** to translate the linear expression into a postfix vector based on operator precedence:
   * High Precedence (Priority 2): `*`, `/`
   * Low Precedence (Priority 1): `+`, `-`
4. **`extractVars()`**: Scans the postfix tokens for variables and lists the unique ones required for calculation.
5. **`eval()`**: Dynamically prompts for variable entries and evaluates the final postfix expression using an operand stack.

---

## 🔧 Installation & Compilation

Ensure you have a modern C++ compiler supporting at least **C++11** installed (GCC, Clang, or MSVC).

### Using GCC/G++ via Terminal:
```bash
# Compile the program
g++ main.cpp -o expression_evaluator

# Run the executable
./expression_evaluator

