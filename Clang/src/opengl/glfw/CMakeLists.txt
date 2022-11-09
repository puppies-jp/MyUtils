cmake_minimum_required(VERSION 3.16.3)

# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
# set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++)
project(test_cmake CXX)

# mac
set(CMAKE_CXX_FLAGS " -std=c++2a -framework Cocoa -framework IOKit  -framework OpenGL")
#set(CMAKE_CXX_FLAGS " -std=c++2a -framework GLFW -framework OpenGL")
#set(CMAKE_CXX_FLAGS "-std=c++2a -fPIC -Wall ")

# add_definitions(" -lglut -lgl -legl -I/usr/local/include/ -lOpenGL")
# OpenCVに必要(Linuxの場合)

# Include OpenCV header files
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

link_directories(${OPENGL_LIBRARIES_DIRS} ${OPENGL_LIBRARIES_DIRS})
include_directories(${CMAKE_SOURCE_DIR} ${OPENGL_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

#
include_directories("/usr/local/include")
message("#build rendering_main ---------------")
add_executable(offscreenglfw.out offscreenglfw.cpp)
target_link_libraries(offscreenglfw.out
  PRIVATE ${OpenCV_LIBRARIES}
  PRIVATE glfw
)