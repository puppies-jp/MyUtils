# Google Test/mockについて

Google test/mockの使い方をまとめる

- installについて

`google test`は`apt install googletest`でインストールできるが、
ライブラリのビルドまではしてくれないらしい、、、  
そのため、install後にビルドがいる。  

手順は以下、これでlibにライブラリが格納され、cmakeからもリンクできるようになる

```sh
(base)root@ubuntu:/usr/src/googletest# ls
CMakeLists.txt googlemock  googletest
(base)root@ubuntu:/usr/src/googletest# mkdir build ; cmake 
(base)root@ubuntu:/usr/src/googletest# cd build
(base)root@ubuntu:/usr/src/googletest/build# cmake ../
(base)root@ubuntu:/usr/src/googletest/build# make install
```

- CMakeでのパッケージのリンクについて
  - [サンプル](https://github.com/puppies-jp/MyUtils/blob/gh-pages/Clang/src/googletest/CMakeLists.txt)

```cmake
find_package(GTest REQUIRED)
find_package(GTest CONFIG REQUIRED)
include_directories(${GTEST_INCLUDE_DIR} ${GMOCK_INCLUDE_DIR})

# インクルードパスの設定(これもそれぞれの環境に合わせる)
add_executable(MockTest.out MockTest.cpp)
target_link_libraries(MockTest.out 
GTest::GTest GTest::Main  GTest::gmock)
```

## カバレッジを出力する

- コンパイルオプションに`--coverage`を追加する

```cmake
# onUbuntu
set(CMAKE_CXX_FLAGS "-lpthread -std=c++2a -fPIC -Wall ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage") # 追加
```

- カバレッジレポートを出力する

```sh
# 🌟1. ビルドしたバイナリを実行
./~~~

# 同じディレクトリで以下を実行
lcov -d ./  -c -o <ファイル名.info>

# 🌟お好みで、いらないパス上の結果を除外できる
lcov -r coverage.info */googletest/* test/* */c++/* -o coverageFiltered.info

genhtml -o <出力先ディレクトリ> --num-spaces 4 -s --legend ./<infoファイル名>
```
