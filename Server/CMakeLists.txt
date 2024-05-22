cmake_minimum_required(VERSION 3.0)
project(Server)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable target
add_executable(server server.cpp)

# Link OpenCV libraries
target_link_libraries(server ${OpenCV_LIBS})
