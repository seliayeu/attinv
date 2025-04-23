cmake_minimum_required(VERSION 3.15...4.0)

project(
    AttInv
    VERSION 1.0
    LANGUAGES CXX)

add_library(AttInvLib src/attinv/attinvconf.cpp src/attinv/utils.cpp src/attinv/attinvconf.hpp src/attinv/utils.hpp)
add_executable(AttInv src/main.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/keycodes.txt
    ${CMAKE_CURRENT_BINARY_DIR}/keycodes.txt COPYONLY)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(APPINDICATOR REQUIRED appindicator3-0.1)
target_include_directories(AttInv PRIVATE ${GTK3_INCLUDE_DIRS} ${APPINDICATOR_INCLUDE_DIRS})
target_link_libraries(AttInv PRIVATE AttInvLib udev input ${GTK3_LIBRARIES} ${APPINDICATOR_LIBRARIES})
