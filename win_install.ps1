# Check if running as administrator
if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Warning "Please run as Administrator!"
    exit
}

# Install Chocolatey if not present
if (!(Get-Command choco -ErrorAction SilentlyContinue)) {
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
}

# Function to check MinGW availability
function Test-MinGW {
    if (Get-Command "gcc" -ErrorAction SilentlyContinue) {
        $gccPath = (Get-Command "gcc").Source
        if ($gccPath -match "mingw64") {
            Write-Host "Found MinGW GCC at: $gccPath"
            return $true
        }
    }
    return $false
}

# Install dependencies and build tools
choco install -y git cmake
if (-not (Test-MinGW)) {
    Write-Host "Installing MinGW..."
    choco install -y mingw
    # Set up environment variables for MinGW
    $env:PATH = "C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;" + $env:PATH
} else {
    Write-Host "MinGW already installed, skipping installation..."
}

# Install other dependencies
choco install -y ffmpeg opencv-libs sdl2 pdcurses

# Clone and build CMD-Media-Player
$repoPath = "${env:ProgramFiles}\CMD-Media-Player"
if (!(Test-Path $repoPath)) {
    git clone https://github.com/HNRobert/CMD-Media-Player.git $repoPath
}

# Build using CMake with MinGW
Set-Location $repoPath
mkdir build -Force
Set-Location build
$chocoLibPath = "C:\ProgramData\chocolatey\lib"
cmake .. -G "MinGW Makefiles" `
    -DCMAKE_PREFIX_PATH="$chocoLibPath\ffmpeg;$chocoLibPath\opencv-libs;$chocoLibPath\sdl2;$chocoLibPath\pdcurses" `
    -DCMAKE_C_COMPILER=gcc `
    -DCMAKE_CXX_COMPILER=g++
cmake --build .

# Add to PATH
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if (!$userPath.Contains($repoPath)) {
    [Environment]::SetEnvironmentVariable("Path", $userPath + ";$repoPath\build\Release", "User")
}

Write-Host "Installation completed! Please restart your terminal to use cmdp."
