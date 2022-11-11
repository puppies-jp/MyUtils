#include <gmock/gmock.h>
#include <gtest/gtest.h>

// テスト対象のクラス
#include "target.h"
// テスト用モッククラス
#include "MockTrtle.h"

using ::testing::AtLeast;

/*
 * g++ MockTest.cpp -lgtest_main -lgtest -lgmock -lpthread -o test
 * */

TEST(PainterTest, CanDrawSomething)
{
  // テスト用Mockクラス
  MockTurtle turtle;

  // turtleクラスのPenDownの呼び出し回数/戻り値などを設定する
  // Mockで関数を置き換えて、テストの対象を動かす
  // コンストラクタ -> 関数呼び出し
  Painter painter(&turtle);
  using ::testing::Return;
  EXPECT_CALL(turtle, PenDown())
      .Times(AtLeast(2))
      .WillOnce(Return(100))
      .WillOnce(Return(200));

  EXPECT_EQ(100, painter.DrawCircle(0, 0, 10));
  EXPECT_EQ(200, painter.DrawCircle(0, 0, 10));

} // #5

// テスト対象の関数
int sum(int a, int b)
{
  return a + b;
}

// テストコード
TEST(TestCase, sum)
{
  EXPECT_EQ(2, sum(1, 1));        // OK
  EXPECT_EQ(1000, sum(123, 877)); // OK
  // EXPECT_EQ(-8, sum(-12, 3));     // NG
  EXPECT_EQ(-9, sum(-12, 3)); // OK
}

/*
int main(int argc, char** argv) {
  // 以下の行は，テスト開始前に Google Mock （と Google Test）
  // を初期化するために必ず実行する必要があります．
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
