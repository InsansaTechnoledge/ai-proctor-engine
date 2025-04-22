cmake_minimum_required(VERSION 3.10)
project(proctor_engine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Vcpkg support
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# ✅ Include headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ✅ Dependencies
find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# ✅ Optional: PortAudio support with fallback path
option(USE_AUDIO "Enable microphone support with PortAudio" ON)
if(USE_AUDIO)
    # Manually include portaudio paths from Homebrew (macOS)
    include_directories("/opt/homebrew/include")
    link_directories("/opt/homebrew/lib")
    set(PORTAUDIO_LIB "portaudio") # not portaudio_static
endif()

# ✅ Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# ✅ Define binary
add_executable(proctor_engine ${SOURCES})

# ✅ Link standard libs
target_link_libraries(proctor_engine
    PRIVATE
    ${OpenCV_LIBS}
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# ✅ Conditionally link audio
if(USE_AUDIO)
    target_link_libraries(proctor_engine PRIVATE ${PORTAUDIO_LIB})
endif()

# ✅ macOS frameworks
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

# ✅ Windows fallback OpenCV link
if(WIN32)
    target_link_libraries(proctor_engine
        PRIVATE
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_objdetect
    )
endif()

# ✅ Copy binary to Electron's bin/mac (optional)
add_custom_command(TARGET proctor_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:proctor_engine>
        ${CMAKE_SOURCE_DIR}/../gyapak-test-series/electron/bin/mac/proctor_engine
)
