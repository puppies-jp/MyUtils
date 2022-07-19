#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    auto isDirectry1 = fs::is_directory(std::string("./"));
    auto isDirectry2 = fs::is_directory("./a");
    auto isDirectry3 = fs::is_directory("./a/b");
    auto isDirectry4 = fs::is_directory("./a/b/c");
    auto isDirectry4_ = fs::is_directory("./a/b/*");
    auto isDirectry4_1 = fs::is_directory("./a/b/../");
    auto isDirectry4_2 = fs::is_directory("./a/b/?/");
    auto isDirectry4_3 = fs::is_directory("./a/bslink");
    auto isDirectry5 = fs::is_directory("./a/b/c.txt");

    std::cout << "./          :" << (isDirectry1 == true) << std::endl;
    std::cout << "./a         :" << (isDirectry2 == true) << std::endl;
    std::cout << "./a/b       :" << (isDirectry3 == true) << std::endl;
    std::cout << "./a/b/c     :" << (isDirectry4 == true) << std::endl;
    std::cout << "./a/b/*     :" << (isDirectry4_ == true) << std::endl;
    std::cout << "./a/b/../   :" << (isDirectry4_1 == true) << std::endl;
    std::cout << "./a/b/?/    :" << (isDirectry4_2 == true) << std::endl;
    std::cout << "./a/bslink  :" << (isDirectry4_3 == true) << std::endl;
    std::cout << "./a/b/c.txt :" << (isDirectry5 == true) << std::endl;
    return 0;
}
