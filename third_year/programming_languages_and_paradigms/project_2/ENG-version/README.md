# OSkell - Language Declaration

## Description

OSkell is a simple functional language, heavily inspired by Haskell and OCaml. It features strong and static typing, incorporating many characteristics from both languages, such as similar syntax for lists, function notation, and `let` expressions. The exact features are outlined below:

### Requirements for 20 points:

| Requirement | Description                                | Status |
|-------------|--------------------------------------------|--------|
| 01          | Two types                                  | ✅     |
| 02          | Arithmetic, comparisons                    | ✅     |
| 03          | If statement                               | ✅     |
| 04          | Multi-argument functions, recursion        | ✅     |
| 05          | Anonymous functions, partial application   | ✅     |
| 06          | Runtime error handling                     | ✅     |
| 07          | Static identifier binding                  | ✅     |
| 09          | Lists with empty, head, tail               | ✅     |
| 10          | Lists with syntactic sugar                 | ✅     |

### Requirements for 25 points:

| Requirement | Description                                                  | Status |
|-------------|--------------------------------------------------------------|--------|
| 11          | Lists of any type, nested and function lists                 | ✅     |
| 13          | Static typing                                                | ✅     |

### Total: 25 points

## Grammar

The grammar of the language can be found in the `OSkell.cf` file, written in EBNF format.

## Examples

In the `examples` directory, there are examples demonstrating the features described above. This directory contains two subdirectories:

- **good**: Contains examples of correct usage, demonstrating how the interpreter handles various scenarios.
- **bad**: Contains examples of incorrect usage, showcasing cases where the interpreter detects errors.

To run the examples, execute the following command:

```bash
make
./run-examples.sh
```
