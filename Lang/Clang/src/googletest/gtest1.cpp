#include <gtest/gtest.h>
#include <gmock/gmock.h>

/*
 * g++ gtest1.cpp -lgtest_main -lgtest -lpthread -o test
 * */

//テスト対象の関数
int sum(int a, int b)
{
    return a + b;
}

//テストコード
TEST(TestCase, sum)
{
    EXPECT_EQ(2, sum(1, 1));        // OK
    EXPECT_EQ(1000, sum(123, 877)); // OK
    EXPECT_EQ(-8, sum(-12, 3));     // NG
}
