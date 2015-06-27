cmake_minimum_required(VERSION 2.8)
project(move_pioneer2dx)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(GAZEBO gazebo)
endif()
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    move_p2dx.cpp)

add_executable(move_pioneer2dx ${SOURCE_FILES})
target_link_libraries(move_pioneer2dx ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})