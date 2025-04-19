cmake_minimum_required(VERSION 3.10)
project(proctor_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------
# Vcpkg Toolchain (Optional for Windows)
# ---------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

# -------------------------
# Include Directories
# -------------------------
include_directories(include)

# -------------------------
# Dependencies
# -------------------------
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)
find_package(nlohmann_json CONFIG REQUIRED)

# -------------------------
# Source Files
# -------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")

# -------------------------
# Build Target
# -------------------------
add_executable(proctor_engine ${SOURCES})

# -------------------------
# Common Link Libraries
# -------------------------
target_link_libraries(proctor_engine
    PRIVATE
    ${OpenCV_LIBS}
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)

# -------------------------
# Platform-Specific Handling
# -------------------------

# ✅ macOS-specific frameworks
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

# ✅ Windows-specific libraries (example: User32, Gdi32, etc.)
if(WIN32)
  target_link_libraries(proctor_engine
    PRIVATE
    user32
    gdi32
    shell32
  )
endif()
