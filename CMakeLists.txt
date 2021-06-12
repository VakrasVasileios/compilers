cmake_minimum_required(VERSION 3.12) 

project(compilers) 

add_subdirectory(util)

add_subdirectory(libs)

add_subdirectory(src)
link_directories(src util)

add_subdirectory(test)
link_directories(test libs)
link_directories(test util)
link_directories(test src)
