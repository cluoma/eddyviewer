cmake_minimum_required(VERSION 3.7)

include(ExternalProject)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project(eddyviewer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(eddyviewer src/main.cpp src/CameraShot.cpp src/CameraShot.h src/ViewerApplication.cpp src/ViewerApplication.h src/ImageFlip.cpp src/ImageFlip.h)

add_subdirectory(wt-4.4.0)
include_directories(wt-4.4.0/src)
include_directories(cmake-build-release/wt-4.4.0)
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/wt-4.0.0)
#message( STATUS "${CMAKE_CURRENT_BINARY_DIR}/wt-4.0.0" )
target_link_libraries(${PROJECT_NAME} PRIVATE wt wthttp wtdbo wtdbosqlite3)
target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
