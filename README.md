# Teach Yourself C

<p align="center">
  <b>Learning record repository for "Teach Yourself C"</b>
</p>

<p align="center">
  <img alt="language" src="https://img.shields.io/badge/language-C-00599C.svg">
  <img alt="build" src="https://img.shields.io/badge/build-CMake-064F8C.svg">
  <img alt="formatter" src="https://img.shields.io/badge/formatter-clang--format-262D3A.svg">
  <img alt="lsp" src="https://img.shields.io/badge/LSP-clangd-1F6FEB.svg">
</p>

## Overview

This repository stores:
- chapter-based exercise code
- buildable project configuration

## Repository Map

```text
.
├── Chap02/ .. Chap06/   # Chapter code
├── CMakeLists.txt       # Project build configuration
├── .clang-format        # Formatting rules
└── .clangd              # clangd settings (uses build DB)
```

## Quick Start

### 1. Configure

```sh
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
ln -sf build/compile_commands.json compile_commands.json
```

### 2. Build

```sh
cmake --build build -j
```

### 3. Run (example)

```sh
./build/Chap06_ch06-00
```

## Progress

`Completion: 5 / 14 chapters`

| Chapter | Status |
| --- | --- |
| 1. Setup for Learning | `PENDING` |
| 2. Basics of C | `DONE` |
| 3. Numbers and Variables | `DONE` |
| 4. Control Flow: Booleans and Branching | `DONE` |
| 5. Control Flow: Arrays and Loops | `DONE` |
| 6. Control Flow: Conditional Loops | `DONE` |
| 7. Characters and Strings | `PENDING` |
| 8. Addresses and Pointers | `PENDING` |
| 9. Writing Functions | `PENDING` |
| 10. Input and Output | `PENDING` |
| 11. Structures | `PENDING` |
| 12. Unions and Bit Fields | `PENDING` |
| 13. Advanced Types and Operators | `PENDING` |
| 14. Preprocessor | `PENDING` |

## License

Personal study project.
