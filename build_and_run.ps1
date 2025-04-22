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
    Write-Host "Build successful!"

    # Define source and destination for binary
    $source = "./Release/proctor_engine.exe"
    $destination = "../../gyapak-test-series/electron/bin/win/proctor_engine.exe"

    # Create destination directory if needed
    $destDir = Split-Path -Path $destination -Parent
    if (-Not (Test-Path -Path $destDir)) {
        New-Item -ItemType Directory -Path $destDir | Out-Null
    }

    # Copy the executable
    Copy-Item -Path $source -Destination $destination -Force
    Write-Host "Moved proctor_engine.exe to $destination"

    # Optional: Run the engine
    # Write-Host "Running Proctor Engine..."
    # & $destination

    Set-Location ../
} else {
    Write-Host "Build failed. Check above for errors."
}
