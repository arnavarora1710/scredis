cmake_minimum_required(VERSION 3.10)
project(tcp_echo_server)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost components
find_package(Boost REQUIRED COMPONENTS system thread)

# Include directories
include_directories(${Boost_INCLUDE_DIRS} include)

# Add executable
add_executable(echo_server 
    src/server/lib_init.cpp
)

# Link libraries
target_link_libraries(echo_server ${Boost_LIBRARIES})

# Print Boost locations for debugging
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

# Install target
install(TARGETS echo_server DESTINATION bin) 