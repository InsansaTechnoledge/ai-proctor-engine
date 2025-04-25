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
# if ($LASTEXITCODE -eq 0) {
#     Write-Host "Build successful!"

#     # Define source and destination for entire Release folder
#     $source = "./Release"
#     $destination = "../../gyapak-test-series/electron/bin/win"

#     # Create destination directory if needed
#     if (-Not (Test-Path -Path $destination)) {
#         New-Item -ItemType Directory -Path $destination | Out-Null
#     }

#     # Copy the entire Release folder content into win directory
#     Copy-Item -Path "$source\*" -Destination $destination -Recurse -Force
#     Write-Host "Moved contents of Release folder to $destination"

#     # Optional: Run the engine (from new location)
#     $enginePath = Join-Path $destination "proctor_engine.exe"
#     Write-Host "Running Proctor Engine..."
#     & $enginePath

#     # Run with arguments if needed
#     # & $enginePath 67f6a65f334e40ba01ab2924 exam123 event456

#     Set-Location ../
# } else {
#     Write-Host "Build failed. Check above for errors."
# }


if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful!"

    # Define paths
    $source = Resolve-Path "./Release"
    $destinationPath = "../../gyapak-test-series/electron/bin/win"

    # Remove the 'win' folder if it exists
    if (Test-Path $destinationPath) {
        Remove-Item -Path $destinationPath -Recurse -Force
        Write-Host "Removed existing win folder."
    }

    # Copy the entire contents of Release (including model folder) to 'win'
    Copy-Item -Path $source -Destination $destinationPath -Recurse -Force
    Write-Host "Copied Release folder (including model) to win"

    # Optional: Run the engine from the new location
    $enginePath = Join-Path $destinationPath "proctor_engine.exe"
    Write-Host "Running Proctor Engine..."
    & $enginePath

    # Run with arguments if needed
    & $enginePath 67f6a65f334e40ba01ab2924 exam123 event456

    Set-Location ../
} else {
    Write-Host "Build failed. Check above for errors."
}
