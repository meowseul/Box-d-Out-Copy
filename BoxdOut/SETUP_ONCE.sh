#!/usr/bin/env bash
# Run this once. It configures the project with CMake, which triggers
# FetchContent to clone and build SFML 3.1.0 into build/_deps.
# That clone/build is slow, so it happens here — not on every `make exe`.
set -e

REQUIRED_PKGS=(
    build-essential
    cmake
    git
    libx11-dev
    libxrandr-dev
    libxcursor-dev
    libxi-dev
    libudev-dev
    libgl1-mesa-dev
    libegl1-mesa-dev
    libfreetype-dev
)

MISSING_PKGS=()
for pkg in "${REQUIRED_PKGS[@]}"; do
    if ! dpkg -s "$pkg" >/dev/null 2>&1; then
        MISSING_PKGS+=("$pkg")
    fi
done

if [ ${#MISSING_PKGS[@]} -gt 0 ]; then
    echo "Installing missing dependencies: ${MISSING_PKGS[*]}"
    sudo apt-get update
    sudo apt-get install -y "${MISSING_PKGS[@]}"
fi

echo "Configuring project into ./build ..."
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

echo "Setup complete."
echo "Build with:  make exe"
echo "Run with:    ./bin/main"