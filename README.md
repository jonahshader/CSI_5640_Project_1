# CSI 5640 Project 1

## Team Members

- **Jonah Shader**
- **Lucas Mitrak**
- **Jihene Chouiref**

## Project 1 Description

1. Select a computation problem.
2. Write a C/C++ program to solve the problem.
3. Modify the C/C++ program to use OpenACC.
4. Gather experimental data for both programs and compare.

## Cellular Automata

TODO: briefly discuss cellular automata

## Dependencies

- **CMake**: Make sure you have CMake installed on your system. You can download it from [here](https://cmake.org/download/).

## Setup

1. Clone the repository:

   ```bash
   git clone https://github.com/jonahshader/CSI_5640_Project_1.git
   cd CSI_5640_Project_1
   ```

2. Ensure CMake is installed and properly set up. TODO: determine g++ version needed

## Building the Project

### Linux/Windows

Note: For Windows users, it's recommended to use the **Visual Studio Developer Command Prompt** to ensure the MSVC compiler is used.

Note: If you are a developer, make sure to add a .gitignore file to the build directories with "*" to avoid adding build files to the repository.

#### Debug Build

1. Create a build directory for the debug configuration:

   ```bash
   mkdir build-debug
   cd build-debug
   ```

2. Configure the project with `CMAKE_BUILD_TYPE=Debug`:

   ```bash
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. Build the project:

   ```bash
   cmake --build .
   ```

#### Release Build

1. Create a build directory for the release configuration:

   ```bash
   mkdir build-release
   cd build-release
   ```

2. Configure the project with `CMAKE_BUILD_TYPE=Release`:

   ```bash
   cmake -DCMAKE_BUILD_TYPE=Release ..
   ```

3. Build the project:

   ```bash
    cmake --build .
   ```

- **CMake Options**:
  - `CMAKE_BUILD_TYPE=Debug`: Enables debugging information and disables optimizations.
  - `CMAKE_BUILD_TYPE=Release`: Optimizes the code and strips debug information.
