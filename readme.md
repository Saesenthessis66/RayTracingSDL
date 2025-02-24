# RayTracingSDL

RayTracingSDL is a simple ray tracing project written in C, using the **SDL3** library for rendering.  
It includes **unit tests** with the **Unity** test framework.

## 🛠 SDL3 Installation (MSYS2 UCRT)

1. **Open the MSYS2 UCRT64 terminal** (⚠️ Not MSYS or MINGW!).

2. **Install SDL3**:
   ```sh
   pacman -S mingw-w64-ucrt-x86_64-SDL3 
   ```

## 🚀 Build the Project

1. **Clone the repository**:
   ```sh
   git clone https://github.com/your-username/RayTracingSDL.git
   cd RayTracingSDL
   ```

2. **Create a build directory**:
   ```sh
   mkdir build
   cd build
   ```

3. **Run CMake to configure the project**:
   ```sh
   cmake -G "MinGW Makefiles" ..
   ```

4. **Compile the project**:
   ```sh
   cmake --build .
   ```

5. **Run the executable**:
   ```sh
   ./RayTracingSDL
   ```

## ✅ Running Unit Tests

The project includes unit tests using the [Unity](https://github.com/ThrowTheSwitch/Unity) framework.  
Tests are automatically downloaded via **CMake FetchContent**.

To run the tests:

```sh
./build/run_tests
```

## 🎨 Features
- Ray tracing engine written in **C**
- Uses **SDL3** for rendering
- Lightweight **unit testing** with **Unity**