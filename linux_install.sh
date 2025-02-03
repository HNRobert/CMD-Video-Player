#!/bin/bash

# Detect package manager
if command -v apt &> /dev/null; then
    PM="apt"
    sudo apt-get update
    sudo apt install build-essential git cmake libopencv-dev ffmpeg libsdl2-dev libreadline-dev libncurses-dev
elif command -v dnf &> /dev/null; then
    PM="dnf"
    sudo dnf update
    sudo dnf install gcc gcc-c++ git cmake opencv-devel ffmpeg-devel SDL2-devel readline-devel ncurses-devel
elif command -v yum &> /dev/null; then
    PM="yum"
    sudo yum update
    sudo yum groupinstall "Development Tools"
    sudo yum install git cmake opencv-devel ffmpeg-devel SDL2-devel readline-devel ncurses-devel
elif command -v pacman &> /dev/null; then
    PM="pacman"
    sudo pacman -Syu
    sudo pacman -S base-devel git cmake opencv ffmpeg sdl2 readline ncurses
elif command -v zypper &> /dev/null; then
    PM="zypper"
    sudo zypper refresh
    sudo zypper install gcc gcc-c++ git cmake opencv-devel ffmpeg-devel libSDL2-devel readline-devel ncurses-devel
else
    echo "No supported package manager found (apt/dnf/yum/pacman/zypper)"
    echo "Please install the following dependencies manually:"
    echo "- build tools (gcc, g++)"
    echo "- git"
    echo "- cmake"
    echo "- opencv"
    echo "- ffmpeg"
    echo "- sdl2"
    echo "- readline"
    echo "- ncurses"
    exit 1
fi

# Clone repository
REPO_PATH="/usr/local/src/cmdp"
if [ ! -d "$REPO_PATH" ]; then
    sudo git clone https://github.com/HNRobert/CMD-Media-Player.git "$REPO_PATH"
fi

# Create build directory and compile
cd "$REPO_PATH"
sudo mkdir -p build && cd build
sudo cmake .. -DDEP_TYPE=A
sudo make -j$(nproc)

# Install to system (binary will go to /usr/local/bin)
sudo make install

echo "Installation completed! CMD Media Player has been installed to /usr/local/bin"

