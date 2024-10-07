cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 11)

# Find Boost libraries
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Add executable for the server
add_executable(chat_server server.cpp)
target_link_libraries(chat_server Boost::system)

# Add executable for the client
add_executable(chat_client client.cpp)
target_link_libraries(chat_client Boost::system)
