cmake_minimum_required(VERSION 3.10)
project(ProctorEngine VERSION 1.0.0)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
# vcpkg integration
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()
 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(OpenCV REQUIRED)
find_package(portaudio CONFIG REQUIRED)
find_package(CURL REQUIRED)
 
# set(SOURCES
#     # src/main.cpp
#     # src/proctor_engine.cpp
#     # src/utils.cpp
#     # src/face_detector.cpp
#     # Add others like audio_monitor.cpp, screen_monitor.cpp if needed
# )

file(GLOB SOURCES "src/*.cpp")
 
add_executable(proctor_engine ${SOURCES})
 
target_link_libraries(proctor_engine
    opencv_core
    opencv_imgproc
    opencv_highgui
    opencv_objdetect
    portaudio
    CURL::libcurl
)
 