cmake_minimum_required(VERSION 3.19)

# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
project(test_cmake CXX)

message("[cmake start]--------------------------------------")

# パッケージ
find_package(Boost COMPONENTS thread system log log_setup REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIRS})

# option追加
add_definitions("-Wall -std=c++1y")
add_definitions("-DBOOST_LOG_DYN_LINK")

message("---------------------------------------------------")

# ライブラリ定義作成(静的ライブラリ作成)
# boost logライブラリ作成(staticライブラリ作成)
add_library(blogger STATIC boost_log.cpp)

# boost logライブラリ作成(共有ライブラリ作成)
# add_library(blogger_share SHARED boost/boost_log.cpp)

# ライブラリに他共有ライブラリリンク追加(boostlogで共有リンクを使うのでlinkがいる)
target_link_libraries(
    blogger ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${ARGN}
)

# main_exe.outという実行ファイルをmain.cppから作成
add_executable(boost_sample.out boost_sample.cpp)

# main_exe.outを作成する際に使用するライブラリをリンク
target_link_libraries(boost_sample.out blogger)
message("[cmake end]----------------------------------------")
