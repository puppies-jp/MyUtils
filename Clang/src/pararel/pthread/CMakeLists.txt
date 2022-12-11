cmake_minimum_required(VERSION 3.16.3)

# プロジェクト名と使用する言語を設定
project(test_cmake CXX)
set(CMAKE_CXX_COMPILER:FILEPATH /usr/bin/g++)

# onUbuntu
set(CMAKE_CXX_FLAGS "-lpthread -std=c++2a -fPIC -Wall ")

# インクルードパスの設定(これもそれぞれの環境に合わせる)
find_package(Threads REQUIRED)

add_executable(SimpleThread.out SimpleThread.cpp)
target_link_libraries(SimpleThread.out Threads::Threads)

add_executable(barrier.out barrier.cpp)
target_link_libraries(barrier.out Threads::Threads)

add_executable(rwlock.out rwlock.cpp)
target_link_libraries(rwlock.out Threads::Threads)

add_executable(rwlock2.out rwlock2.cpp)
target_link_libraries(rwlock2.out Threads::Threads)

# install
set(execs SimpleThread.out barrier.out rwlock.out rwlock2.out)
install(
  TARGETS ${execs}
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)