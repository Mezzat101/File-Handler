
cmake_minimum_required(VERSION 3.10)
project(FileHandlerApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost libraries
find_package(Boost 1.71 REQUIRED COMPONENTS log log_setup system date_time)

# Add include directories
include_directories(${Boost_INCLUDE_DIRS})

# Add executable for File Handler
add_executable(fh fh.cpp)
target_link_libraries(fh ${Boost_LIBRARIES})

# Add executable for Requester
add_executable(requester requester.cpp)
target_link_libraries(requester ${Boost_LIBRARIES})

# Add executable for tests using gtest
add_executable(main main.cpp)
target_link_libraries(tests ${Boost_LIBRARIES} gtest)

# Add custom target to run tests
add_custom_target(run_tests COMMAND main DEPENDS main)

# Copy log configuration file to build directory
configure_file(log.conf log.conf COPYONLY)