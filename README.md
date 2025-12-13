![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)



# High Performance Market Engine
A distributed C++ market ecosystem featuring a low-latency order book matching engine, a custom FIX-style communication protocol, and an algorithmic trading client.

## Components / Architecture?

Component / Architecture Diagram?

1. **Matching Engine** (`/src/matching_engine`)
    
    blah blah
2. **Protocol Layer** (`/src/protocol_layer`)

    blah blah
3. **Trading Client** (`/src/trading_client`)

    blah blah

4. ( dashboard )

## Documents

- PDF about final product (implementation / metrics / benchmarks / final product)
- PDF about the build process (goals / teamwork / tools / frameworks / learning outcomes)
- Dashboard?
- ...

## Performance

Some performance stats to quickly showcase to recruiters

Read more in PDF about final product

## Dependencies & Tooling
**Language:** C++20

**Build System:** CMake (version 4.2)

**Formatter:** Clang-Format (Google Style, see `/.clang-format`)

**Linter:** Clang-Tidy (see `.clang-tidy`) 

**Testing:** ???

any other ???

## File Structure

```bash
│   .clang-format
│   .clang-tidy
│   .gitignore
│   CMakeLists.txt
│   README.md
├───benchmarks
│       bench_matching_engine.cpp
├───docs
│       diagrams.txt
├───include
│   ├───matching_engine
│   │       placeholder.hpp
│   ├───protocol_layer
│   │       placeholder.hpp
│   └───trading_client
│           placeholder.hpp
├───scripts
│       dashboard.py
├───src
│   ├───matching_engine
│   │       main.cpp
│   ├───protocol_layer
│   │       main.cpp
│   └───trading_client
│           main.cpp
└───tests
        test_order_book.cpp
        test_protocol.cpp
```

Use the following command to update file structure (Windows), make sure that the /build folder is deleted:
```
tree /f
```

blah blah short explanation about file structure

## Run & Contribute

### Prerequisites
- **C++20 compiler** (MSVC, GCC, Clang, etc)
- **CMake**

    Download, run installer, select `Add CMake to system PATH`
- **LLVM** (for Clang-Format and Clang-Tidy)

    Download, run installer, select `Add LLVM to system PATH`
- **VS Code** (optional)

    Install VS Code and use extension: `C/C++ (Microsoft)`

### Build using CMake
On build, Clang-Tidy is also run and outputs warnings in terminal.

**Windows**
```
> cmake --build build
> .\build\Debug\market_engine.exe
> .\build\Debug\trading_client.exe
```

**Linux**
```
> cmake --build build
> .\build\market_engine
> .\build\trading_client
```

### Test using CMake

???

### Set up Clang-Format in VS Code
Clang-Format is used for formatting the code. This guarantees that code is consistent across all files (spacing, tab size, etc)

1. Install the "C/C++" extension.
2. Go to settings, search for "Format on Save", and turn it on.
3. Open a .cpp file and hit `Ctrl+Shift+P`
4. Search for "Format Document With", click it and press `Configure Default Formatter...` and select `C/C++`.
3. Hit `Ctrl+S` to save and the code gets formatted.

### Naming Conventions
| Type | Convention | Example |
| :--- | :---: | ---: |
| Classes | PascalCase | `OrderBook`, `MatchingEngine` |
| Methods | PascalCase | `AddOrder()`, `CancelOrder()` |
| Variables | snake_case | `price_level`, `order_id` |
| Member Vars | snake_case + trailing _ | `best_bid_`, `orders_` |
| Files | snake_case | `order_book.cpp`, `tcp_server.hpp` |
| Namespaces | snake_case | `namespace exchange_core` |
| Constants | kPascalCase | `kMaxOrderSize`, `kDefaultPort` |

## License

???