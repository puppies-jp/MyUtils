# Cmake についてまとめる

- まず、`cmake`とは

  - linux などの環境においてパッケージのリンク先などをまとめたり、o,so ファイルをビルドして、実行ファイルにリンクしてビルドするための Makefile を作成してくれる便利な機能
    (cmake 時のオプション次第で gcc/g++コマンドの option を分けることもできる。)
    (release/debug 用のビルドを分けたり、プリプロセッサ用のフラグを分けたり。。。)
  - 便利な機能であることは確かだが、どこぞのサンプルでの使用例以上の使い方以外はよく分からない。
    そのため、ここに使い方をまとめて行く。
    初めてみる使い方もここに追記して行く予定

---

- 作成例

```cmake
cmake_minimum_required(VERSION 3.19)
# set(CMAKE_VERBOSE_MAKEFILE 1) # for debug cmake
# プロジェクト名と使用する言語を設定
project(test_cmake CXX)

# パッケージ
find_package(Boost COMPONENTS thread system log log_setup REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIRS})

# option追加
add_definitions("-Wall -std=c++1y")
add_definitions("-DBOOST_LOG_DYN_LINK")


# if文によりオプションを分岐させる
if(TEST)
    🌟 cmake ../src -DTEST=ON #これでTESTのフラグがTrueになる。
    message(" [preprocessor]test option build   ")
    # ここでg++でのフラグを設定を追加する。
    add_definitions(-DTEST)
else()
    message(" [preprocessor]no   option build"  )

endif()

# ライブラリ定義作成(静的ライブラリ作成)
add_library(
    sub  　🌟第一引数にoファイル名を入れる(libsub.aが作成される。)
    STATIC　🌟 第二引数に STATIC/SHAREを指定することでライブラリの種類を指定。
    sub/base.cpp　🌟以降の引数にビルドするライブラリのファイルを指定(複数指定可)
    share/share.cpp
)

# boost logライブラリ作成(staticライブラリ作成)
# ライブラリに他共有ライブラリリンク追加(boostlogで共有リンクを使うのでlinkがいる)
add_library(blogger STATIC sub/boost_log.cpp)
target_link_libraries(
    blogger
    ${Boost_LIBRARIES} 🌟ビルドするライブラリで他のパッケージのリンクがいる場合、このようにしてリンクを指定する。
    ${CMAKE_THREAD_LIBS_INIT}
    ${ARGN}
)

# boost logライブラリ作成(共有ライブラリ作成)
# ライブラリに他共有ライブラリリンク追加(boostlogで共有リンクを使うのでlinkがいる)
add_library(blogger_share SHARED sub/boost_log.cpp)
target_link_libraries(
    blogger_share ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT} ${ARGN}
)


# main_exe.outという実行ファイルをmain.cppから作成
🌟　ここで実行ファイルをビルド
add_executable(main_exe.out main.cpp)
# main_exe.outを作成する際に使用するライブラリをリンク
🌟　ここで必要なパッケージをリンクする。
target_link_libraries(main_exe.out sub blogger)

```

```sh
gimahirotaka@gimaMacBook build % cmake ../src -DTEST=off
 [preprocessor]no   option build
---------------------------------------------------
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/gimahirotaka/Desktop/WorkSpace2/cpp_boost/build
gimahirotaka@gimaMacBook build % cmake ../src -DTEST=on
---------------------------------------------------
 [preprocessor]test option build
---------------------------------------------------
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/gimahirotaka/Desktop/WorkSpace2/cpp_boost/build
gimahirotaka@gimaMacBook build %

```

---
