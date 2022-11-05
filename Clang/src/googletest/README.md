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
