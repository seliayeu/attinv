cmake_minimum_required(VERSION 3.15...4.0)

project(
    AttInv
    VERSION 1.0
    LANGUAGES CXX)

add_library(AttInvLib src/attinv/attinvconf.cpp src/attinv/utils.cpp src/attinv/attinvconf.hpp src/attinv/utils.hpp)
add_executable(AttInv src/main.cpp)
target_link_libraries(AttInv PRIVATE AttInvLib udev input)
