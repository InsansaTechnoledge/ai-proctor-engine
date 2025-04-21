Write-Host "Building AI Proctor Engine..."

# Create build directory if it doesn't exist
if (-Not (Test-Path -Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
Set-Location build

# Configure with vcpkg toolchain
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . --config Release

# Check if build succeeded
if ($LASTEXITCODE -eq 0) {
    Write-Host "Running Proctor Engine..."
    .\Release\proctor_engine.exe
    Set-Location ../
} else {
    Write-Host "Build failed. Check above for errors."
}
