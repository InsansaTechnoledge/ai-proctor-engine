#!/bin/bash
#hi this is a test 
# Define paths
BUILD_DIR="build"
TOOLCHAIN_FILE="./vcpkg/scripts/buildsystems/vcpkg.cmake"

echo "🔧 Cleaning previous build..."
rm -rf "$BUILD_DIR"

echo "📁 Creating build directory..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR" || exit

echo "⚙️ Running CMake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE

echo "🏗️ Building project..."
make -j$(sysctl -n hw.ncpu)

if [ $? -eq 0 ]; then
  echo "🚀 Running AI Proctor Engine..."
  ./proctor_engine
else
  echo "❌ Build failed. Please check errors above."
fi
