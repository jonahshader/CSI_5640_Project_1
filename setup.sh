#!/bin/bash

# Exit on any error
set -e

echo "Setting up development environment for Cellular Automata project..."

# Function to check if a command succeeded
check_status() {
    if [ $? -eq 0 ]; then
        echo "✓ $1 successful"
    else
        echo "✗ $1 failed"
        exit 1
    fi
}

# Update package list
echo "Updating package list..."
sudo apt-get update
check_status "Package list update"

# Install basic development tools
echo "Installing build essentials..."
sudo apt-get install -y build-essential
check_status "Build essentials installation"

# Install CMake
echo "Installing CMake..."
sudo apt-get install -y cmake
check_status "CMake installation"

# Install specific GCC version and OpenACC support packages
echo "Installing GCC/G++ and OpenACC support..."
sudo apt-get install -y gcc g++ gcc-offload-nvptx nvidia-cuda-toolkit
check_status "GCC/G++ and OpenACC installation"

# Install NVIDIA drivers if not present
if ! command -v nvidia-smi &> /dev/null; then
    echo "Installing NVIDIA drivers..."
    sudo apt-get install -y ubuntu-drivers-common
    sudo ubuntu-drivers autoinstall
    check_status "NVIDIA drivers installation"
    echo "NOTE: You will need to reboot after driver installation"
else
    echo "✓ NVIDIA drivers already installed"
fi

# Install Python and venv
echo "Installing Python and dependencies..."
sudo apt-get install -y python3 python3-pip python3-venv
check_status "Python installation"

# Create and activate Python virtual environment
echo "Setting up Python virtual environment..."
if [ ! -d "visualization/venv" ]; then
    python3 -m venv visualization/venv
    check_status "Virtual environment creation"
else
    echo "✓ Virtual environment already exists"
fi

# Activate virtual environment and install requirements
echo "Installing Python requirements..."
source visualization/venv/bin/activate
if [ -f "visualization/requirements.txt" ]; then
    pip install -r visualization/requirements.txt
    check_status "Python requirements installation"
else
    echo "⚠ Warning: visualization/requirements.txt not found"
fi
deactivate

# Print versions for verification
echo -e "\nVerifying installations:"
gcc --version
g++ --version
nvcc --version
nvidia-smi
python3 --version
cmake --version

echo -e "\nChecking OpenACC offloading support..."
gcc -fopenacc -foffload=nvptx-none -cpp -dM -E - < /dev/null | grep -i openacc
check_status "OpenACC support check"

echo -e "\nSetup complete! You can now build the project using make."

# Print some helpful instructions
echo -e "\nTo build the project:"
echo "1. Navigate to the project directory"
echo "2. Run 'make' for release build"
echo "   or 'make debug' for debug build"
echo "   or 'make clean' to clean build files"
echo "3. Or use CMake:"
echo "   mkdir build && cd build"
echo "   cmake .."
echo "   make"

echo -e "\nTo use the visualization component:"
echo "1. Activate the virtual environment:"
echo "   source visualization/venv/bin/activate"
echo "2. When finished:"
echo "   deactivate"

# Check if reboot might be needed
if [ ! -f /tmp/nvidia_driver_installed ]; then
    echo -e "\nNOTE: If this is your first time installing NVIDIA drivers,"
    echo "      you should reboot your system before proceeding."
    touch /tmp/nvidia_driver_installed
fi
