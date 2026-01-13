![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)


# Order Book Matching Engine
A highly optimized C++ order book matching engine. [TODO: more detailed/descriptive description]
Single threaded, deterministic

Concise performance metrics...

Image somehow? Latency histogram? Implementation visualization from blog post?

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

## Document
Want all information and details about the matching engine in a nicely structured document? View or download this pdf:

PDF about final product (implementation / metrics / benchmarks / final product) (View | Download)

_Found it interesting? Feel free to share it with friends or colleagues who might enjoy or benefit from it. Thank you ;)_

## Blog

Want more details about the build process and development? Read my blog posts with visualizations, explanations, and more:

- #1 : link
- #2 : link

_Enjoyed the blog? Feel free to share it with friends or colleagues who might enjoy or benefit from it. Thank you ;)_

## Dependencies & Tooling
**Language:** C++20

**Build System:** CMake (version 4.2)

**Testing:** CTest (version 4.2)

### Development

**Formatter:** Clang-Format (Google Style, see `/.clang-format`)

**Linter:** Clang-Tidy (see `.clang-tidy`) 

## Build and Run

### Prerequisites
- **C++20 compiler** (MSVC, GCC, Clang, etc)
- **CMake**

    Download, run installer, select `Add CMake to system PATH`

### Build & Run Benchmark
Use the following commands to build and run the benchmark (`/benchmark`). Uses `Release` build type for compiler optimizations.

_Linux_
```powershell
> cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release
> cmake --build build_release
> .\build_release\benchmark_engine.exe
```

### Build & Run Tests
Use the following commands to build and run the tests (`/tests`).

_Linux_
```powershell
> cmake -S . -B build_debug -DCMAKE_BUILD_TYPE=Debug
> cmake --build build_debug
> .\build_debug\test_engine.exe # run tests directly, or
> ctest --test-dir build_debug # run tests via ctest
```

### Build & Run During Development
Modify the `src/main.cpp` file to test individual features during development, which will be compiled into the `run_engine.exe` file.

_Linux_
```powershell
> cmake -S . -B build_debug -DCMAKE_BUILD_TYPE=Debug
> cmake --build build_debug
> .\build_debug\run_engine.exe
```

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
│   ├───common
│   │       Types.hpp
│   └───matching_engine
│           Order.hpp
│           OrderBook.hpp
├───scripts
│       dashboard.py
├───src
│   │   main.cpp
│   └───matching_engine
│           Order.cpp
│           OrderBook.cpp
└───tests
        test_order_book.cpp
```

Use the following command to update file structure (Windows), make sure that the /build folder is deleted:
```
tree /f
```

blah blah short explanation about file structure

## Contribute

### Extra / Optional Tools
- **LLVM** (for Clang-Format and Clang-Tidy)

    Download, run installer, select `Add LLVM to system PATH`
- **VS Code**

    Install VS Code and use extensions: `C/C++`, `CMake Tools` and `clangd` (make sure to disable IntelliSense for clangd to work)

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