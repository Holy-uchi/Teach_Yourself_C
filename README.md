# Teach Yourself C

<p align="center">
  <b>Learning record repository for "Teach Yourself C"</b>
</p>

<p align="center">
  <img alt="language" src="https://img.shields.io/badge/language-C-00599C.svg">
  <img alt="build" src="https://img.shields.io/badge/build-CMake-064F8C.svg">
  <img alt="formatter" src="https://img.shields.io/badge/formatter-clang--format-262D3A.svg">
  <img alt="lsp" src="https://img.shields.io/badge/LSP-clangd-1F6FEB.svg">
  <img alt="progress" src="https://img.shields.io/badge/progress-6%2F14-2ea043.svg">
  <img alt="completion" src="https://img.shields.io/badge/completion-42.9%25-2ea043.svg">
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

`Completion: 6 / 14 chapters`

```text
[████████░░░░░░░░░░] 42.9%
```

```mermaid
pie showData
  title Chapter Completion
  "Done (6)" : 6
  "Pending (8)" : 8
```

| Chapter | Status |
| --- | --- |
| 1. Setup for Learning | ![done](https://img.shields.io/badge/DONE-2ea043) |
| 2. Basics of C | ![done](https://img.shields.io/badge/DONE-2ea043) |
| 3. Numbers and Variables | ![done](https://img.shields.io/badge/DONE-2ea043) |
| 4. Control Flow: Booleans and Branching | ![done](https://img.shields.io/badge/DONE-2ea043) |
| 5. Control Flow: Arrays and Loops | ![done](https://img.shields.io/badge/DONE-2ea043) |
| 6. Control Flow: Conditional Loops | ![done](https://img.shields.io/badge/DONE-2ea043) |
| 7. Characters and Strings | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 8. Addresses and Pointers | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 9. Writing Functions | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 10. Input and Output | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 11. Structures | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 12. Unions and Bit Fields | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 13. Advanced Types and Operators | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |
| 14. Preprocessor | ![pending](https://img.shields.io/badge/PENDING-9e9e9e) |

## License

Personal study project.
