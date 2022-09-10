cmake_minimum_required(VERSION 3.16.3)

# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
project(test_cmake CXX)

# OpenCVに必要(Linuxの場合)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# インクルードパスの設定(これもそれぞれの環境に合わせる)
add_executable(main test2.cpp)
set(CMAKE_CXX_FLAGS " -std=c++2a -framework GLUT -framework OpenGL")

add_definitions("-Wall ")
target_link_libraries(main ${OpenCV_LIBS})
