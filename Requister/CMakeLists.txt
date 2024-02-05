
cmake_minimum_required(VERSION 3.10)
project(requester)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost libraries
find_package(Boost 1.71 REQUIRED COMPONENTS log log_setup system date_time)

# Add include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

# Add executable
add_executable(requester requester.cpp)
target_link_libraries(requester ${Boost_LIBRARIES})