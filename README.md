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

For this project, we implemented Conway's Game of Life, a classic cellular automaton (CA) , using OpenACC to compare CPU and GPU performance. Our main goal was to analyze the simulation's behavior in terms of execution speed, efficiency, and frames per second when run on different hardware architectures.

## Replication Instructions (From our paper):

REPLICATION INSTRUCTIONS

This section presents information about the hardware (processor, memory, etc.) and software (operating system, programming language, compiler, and dependencies) used. We detail the software installation, configuration, and sources, allowing others to replicate the results using the submitted source code.

In terms of hardware, an Intel Core i7 or equivalent processor would be ideal but an NVIDIA GPU is a must. We think that it should be combined with CUDA Compute Capability 8.0 or higher and we recommend 16 GB RAM. However, in terms of software, a Linux Operating System distribution like Ubuntu or such is necessary. You will also need a compiler like g++ that supports OpenACC, with compatibility tested for GCC version 10 and above. The Make utility (make) for Makefile-based builds is required. The program relies on the standard C++ library and requires OpenACC support with the NVPTX backend for offloading. The code is written in C++20, so your environment should support this standard. Ensure these dependencies are properly installed and configured to successfully compile and run the software.

We initially used CMake for managing C++ libraries and dependencies. This was immensely useful during the earlier phase of development because it allowed us to write one script, CMakeLists.txt, which CMake would then use to generate platform specific build scripts for Windows and Linux. It also automatically handled the installation & linking of SDL2, allowing for a clean separation between user code and library code. However, as we completed the CPU implementation and moved on to the OpenACC implementation, we could drop the Windows and SDL2 support as they were not crucial for our program’s benchmarking functionality. Because of this, we decided to switch to a simple Makefile as an alternative, to eliminate the need to install CMake.
The computer we used has these exact specifications:

GPU: RTX 3060 12GB

CPU: Ryzen 5 7600

System Memory: 16GB

Motherboard: PRO A620M-E (MS-7E28)

To simplify the setup process as much as possible, we wrote a script, setup.sh, which installs all necessary dependencies for this project.
Project setup, build, and execution instructions:

Clone the repository using these commands:

git clone https://github.com/jonahshader/CSI_5640_Project_1.git && cd CSI_5640_Project_1

Run setup.sh:

Ensure setup.sh is executable by running:

sudo chmod +x setup.sh

Execute the script:

./setup.sh

The script will notify you if there are any issues.

Reboot (for NVIDIA drivers to activate):

sudo reboot

Your system should now be able to build & run the project.

Build the project:

make

Run the project:

From the project root:

./build/bin/cellular_automata

The program will go through the benchmarking process, which takes a significant amount of time to run on the specified hardware (10+ minutes). After it is finished, it will write two JSON files in the project root directory containing the timing information of the two parameter sweeps performed.

If one wants to generate plots from these JSONs, the python script can be used like so:
Activate python virtual environment:

From project root:

source visualization/venv/bin/activate

Run visualization script:

python visualization/main.py --headless --full-numbers --output-dir ./plots change_width_height.json change_iters.json

This will populate the plots folder with the eight plot variations shown in this paper. Running the script with just --help will give more information on the usage and options available.
For a consistent and less error-prone setup process, our setup.sh script was designed to automate the setup of the development environment for this project. It ensures that all necessary dependencies and configurations are in place. Specifically, it updates the system's package list and installs essential build tools, installs the required GCC/G++ compiler with OpenACC support and NVIDIA CUDA Toolkit, checks for NVIDIA drivers, installing them if necessary (and notifies the user to reboot if drivers were newly installed), sets up a Python virtual environment for the visualization component, installing dependencies from the  requirements.txt file, verifies installations by printing versions of gcc, g++, nvcc, nvidia-smi, and Python, as well as confirming OpenACC support, and provides helpful instructions for building the project using the Makefile and managing the Python visualization environment.

This systematic and well-documented process should provide others with a similar replication of our results.
