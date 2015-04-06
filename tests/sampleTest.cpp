#include <gtest/gtest.h>

/*
 * Fixture test.
 */
class FixtureTest : public ::testing::Test {
    protected:
        virtual void SetUp() {}
        virtual void TearDown() {}
};

TEST_F(FixtureTest, TestName1) {
    EXPECT_EQ(1, 1);
}

TEST_F(FixtureTest, TestName2) {
    EXPECT_EQ(1, 1);
}

/*
 * Discrete test.
 */
TEST(HelloTest, TestName1) {
    EXPECT_EQ(1, 1);
}
