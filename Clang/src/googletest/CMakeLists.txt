cmake_minimum_required(VERSION 3.16.3)

# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++)
project(test_cmake CXX)

# onUbuntu
set(CMAKE_CXX_FLAGS "-lpthread -std=c++2a -fPIC -Wall ")
find_package(GTest REQUIRED)
find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})


# インクルードパスの設定(これもそれぞれの環境に合わせる)
add_executable(main.out MockTest.cpp)
target_link_libraries(main.out GTest::GTest GTest::Main  GTest::gmock)
add_executable(main2.out gtest1.cpp)
target_link_libraries(main2.out GTest::GTest GTest::Main  GTest::gmock)