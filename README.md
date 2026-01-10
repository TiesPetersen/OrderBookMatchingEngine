![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)


# High Performance Matching Engine
A highly optimized C++ order book matching engine. [TODO: more detailed/descriptive description]

Concise performance metrics...

## Features

### Place order
Places and executes a new order using price-time priority. 

**Order types**
- Limit order
- Market order

### Cancel order
Removes order from limit order book.

_Note: the modify-order operation has been left out for simplicity. Modifying an order can simply be treated as a cancel + place order. You would lose your place in the time-priority queue this way, but that is what happens in real exchanges most of time anyway._

## Performance

Some performance stats to quickly showcase to recruiters

Read more in PDF about final product

## Documents

- PDF about final product (implementation / metrics / benchmarks / final product)
- PDF about the build process (goals / teamwork / tools / frameworks / learning outcomes)
- Dashboard?
- ...

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
├───benchmarks
│       bench.cpp
├───docs
│       diagrams.txt
├───include
│   ├───common
│   │       Types.hpp
│   └───matching_engine
│           Order.hpp
│           OrderBook.hpp
├───scripts
│       dashboard.py
├───src
│       main.cpp
│       Order.cpp
│       OrderBook.cpp
└───tests
        test.cpp
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

    Install VS Code and use extensions: `C/C++`, `CMake Tools` and `clangd` (make sure to disable IntelliSense for clangd to work)

### Build using CMake
Use the following commands to build the benchmark. On build, Clang-Tidy is also run and outputs warnings in terminal.

```
> cmake --build build
> .\build\bench_engine.exe
```

### Test using CMake

???

### Run during development
Use the following commands to compile the matching engine in order to test specific features by changing `src/main.cpp`.

```
> clang++ -std=c++20 -O3 -I include src/*.cpp -o test.exe
> .\test.exe
```

### Set up Clang-Format in VS Code
Clang-Format is used for formatting the code. This guarantees that code is consistent across all files (spacing, tab size, etc)

1. Go to settings, search for "Format on Save", and turn it on.
3. Open a .cpp file and hit `Ctrl+Shift+P`
4. Search for "Format Document With", click it and press `Configure Default Formatter...` and select `clangd`.
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