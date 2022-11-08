cmake_minimum_required(VERSION 3.16.3)

# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
# set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++)
project(test_cmake CXX)

# mac
# set(CMAKE_CXX_FLAGS " -std=c++2a -framework GLUT -framework OpenGL")
set(CMAKE_CXX_FLAGS "-std=c++2a -fPIC -Wall ")

# add_definitions(" -lglut -lgl -legl -I/usr/local/include/ -lOpenGL")
# OpenCVに必要(Linuxの場合)

# Include OpenCV header files
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL)

link_directories(${OPENGL_LIBRARIES_DIRS} ${OPENGL_LIBRARIES_DIRS})
include_directories(${CMAKE_SOURCE_DIR} ${OPENGL_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})


add_library(ContextManager SHARED 
  render_context.cpp
)
target_link_libraries(ContextManager
  PRIVATE ${OpenCV_LIBRARIES}
  PRIVATE OpenGL::OpenGL OpenGL::EGL
)
add_executable(rendering_main.out rendering_main.cpp)
target_link_libraries(rendering_main.out
PRIVATE
  ContextManager
  PRIVATE ${OpenCV_LIBRARIES}
  PRIVATE OpenGL::OpenGL OpenGL::EGL
)

# インクルードパスの設定(これもそれぞれの環境に合わせる)
add_executable(egl_offscreen.out egl_offscreen.cpp)
target_link_libraries(egl_offscreen.out
  PRIVATE ${OpenCV_LIBRARIES}
  PRIVATE OpenGL::OpenGL OpenGL::EGL
)

set(execs egl_offscreen.out)
install(
  TARGETS ${execs}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)

set(plygonfile vData.txt fData.txt)
install(
  FILES ${plygonfile}
  DESTINATION ${CMAKE_BINARY_DIR}
)
