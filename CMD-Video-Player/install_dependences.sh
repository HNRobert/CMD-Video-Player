#
//  install_dependences.sh
//  CMD-Video-Player
//
//  Created by Robert He on 2024/9/9.
//

#!/bin/zsh

# Function to check if a command is available in the terminal
check_command() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if Homebrew is installed
check_homebrew() {
    if check_command "brew"; then
        echo "Homebrew is installed."
    else
        echo "Homebrew is not installed. Please install Homebrew first."
        exit 1
    fi
}

# Function to check if the system is running on Apple Silicon (arm64)
check_arm64() {
    arch_name=$(uname -m)
    if [[ "$arch_name" == "arm64" ]]; then
        echo "Running on Apple Silicon (arm64)."
    else
        echo "Not running on Apple Silicon (arm64). This script is meant for arm64 systems."
        exit 1
    fi
}

# Function to check if software with a specific version is installed via Homebrew
check_homebrew_version() {
    local software="$1"
    local version_pattern="$2"
    local installed_version

    if brew list --versions "$software" > /dev/null; then
        installed_version=$(brew list --versions "$software" | awk '{print $2}')
        if [[ $installed_version == $version_pattern ]]; then
            echo "$software version $installed_version is installed via Homebrew."
        else
            echo "$software version $installed_version is installed, but we require version $version_pattern."
            return 1
        fi
    else
        echo "$software is not installed via Homebrew."
        return 1
    fi
}

# Function to prompt user if they want to install a specific version via Homebrew (arm64)
prompt_install_arm64() {
    local software="$1"
    local version_formula="$2"
    read -p "Do you want to install $software version $version_formula using Homebrew (arm64)? (y/n): " response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        echo "Installing $software version $version_formula via Homebrew for arm64..."
        /opt/homebrew/bin/brew install "$version_formula"
    else
        echo "Skipping $software installation."
    fi
}

# Check Homebrew and system architecture
check_homebrew
check_arm64

# List of software to check with specific versions
declare -A software_list
software_list=(
    ["ffmpeg"]="7."
    ["opencv"]="4."
    ["sdl2"]="2."
)

# Loop through each software and check its version
for software in ${(k)software_list}; do
    version_pattern="${software_list[$software]}"
    echo "Checking $software version $version_pattern..."

    # Check if the software with the required version is installed
    if ! check_homebrew_version "$software" "$version_pattern"; then
        # Prompt to install the specific version via Homebrew arm64 version
        prompt_install_arm64 "$software" "$software@$version_pattern"
    fi
    echo "------------------------------------"
done
