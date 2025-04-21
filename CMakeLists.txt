cmake_minimum_required(VERSION 3.10)
project(proctor_engine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vcpkg toolchain integration
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find packages
find_package(OpenCV REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Collect all .cpp source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(proctor_engine ${SOURCES})

# Link libraries common to all platforms
target_link_libraries(proctor_engine
    PRIVATE
    ${OpenCV_LIBS}
    portaudio
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# macOS specific frameworks
if(APPLE)
    find_library(CORE_GRAPHICS_LIBRARY CoreGraphics)
    find_library(QUARTZ_LIBRARY Quartz)
    find_library(APPKIT_LIBRARY AppKit)

    target_link_libraries(proctor_engine
        PRIVATE
        ${CORE_GRAPHICS_LIBRARY}
        ${QUARTZ_LIBRARY}
        ${APPKIT_LIBRARY}
    )
endif()

# Windows-specific additions (if any)
if(WIN32)
    target_link_libraries(proctor_engine
        PRIVATE
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_objdetect
    )
endif()
