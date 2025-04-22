#!/bin/bash

# Arguments passed to the script
USER_ID=$1
EXAM_ID=$2
EVENT_ID=$3

# Validate arguments
if [ -z "$USER_ID" ] || [ -z "$EXAM_ID" ] || [ -z "$EVENT_ID" ]; then
  echo "❌ Missing arguments!"
  echo "Usage: ./build_and_run_mac.sh <userId> <examId> <eventId>"
  exit 1
fi

# Set paths
BUILD_DIR="build"
TOOLCHAIN_FILE="./vcpkg/scripts/buildsystems/vcpkg.cmake"
FINAL_BINARY_PATH="${BUILD_DIR}/proctor_engine"

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
  echo "✅ Build completed successfully."
  echo "🚀 Running AI Proctor Engine from $FINAL_BINARY_PATH ..."
  chmod +x "$FINAL_BINARY_PATH"
  "$FINAL_BINARY_PATH" "$USER_ID" "$EXAM_ID" "$EVENT_ID"
else
  echo "❌ Build failed. Please check errors above."
fi
