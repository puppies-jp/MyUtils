cmake_minimum_required(VERSION 3.16.3)

# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++)
project(test_cmake CXX)

# on Ubuntu
message(install\ to\ ${CMAKE_BINARY_DIR})
add_subdirectory(./googletest)
add_subdirectory(./pararel/pthread)
add_subdirectory(./pararel/semaphore)
add_subdirectory(./pararel/shareMem)
add_subdirectory(./opengl)