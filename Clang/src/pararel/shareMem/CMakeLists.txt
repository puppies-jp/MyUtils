cmake_minimum_required(VERSION 3.16.3)

# プロジェクト名と使用する言語を設定
project(test_cmake CXX)
set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++)

# onUbuntu
set(CMAKE_CXX_FLAGS "-lpthread -std=c++2a -fPIC -Wall ")

# インクルードパスの設定(これもそれぞれの環境に合わせる)
add_executable(forkShareMem.out forkShareMem.cpp)

set(execs forkShareMem.out)
install(
  TARGETS ${execs}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)