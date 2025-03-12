# Tensor-CPP

Tensor-CPP is a lightweight C++ library for tensor and matrix-vector operations. It provides efficient implementations for mathematical computations commonly used in scientific computing and machine learning.

## Features
- Basic tensor operations
- Matrix-vector computations
- C++ header-based library
- Includes test cases

## Installation

### Requirements
- C++17 or later
- CMake (>=3.10)

### Build Instructions
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/tensor-cpp.git
   cd tensor-cpp
   ```
2. Create a build directory and compile the project:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```

## Usage
Include the necessary headers in your C++ project:
```cpp
#include "matvec.hpp"
#include "tensor.hpp"
```
Run the test executables:
```sh
./test_matvec
./test_tensor
```

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-name`).
3. Commit your changes (`git commit -m 'Add feature'`).
4. Push to the branch (`git push origin feature-name`).
5. Open a Pull Request.

## License
This project is licensed under the MIT License. See `LICENSE` for details.

## Acknowledgements
Special thanks to all contributors and open-source projects that inspired this work.

