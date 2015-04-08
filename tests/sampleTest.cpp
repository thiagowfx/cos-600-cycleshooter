#include <gtest/gtest.h>

/*
 * Fixture tests.
 */
class FixtureTest : public ::testing::Test {
    protected:
        virtual void SetUp() {}
        virtual void TearDown() {}
};

TEST_F(FixtureTest, TestName1) {
    EXPECT_EQ(1, 1);
}

/*
 * Discrete tests.
 */
TEST(DiscreteTest, TestName1) {
    EXPECT_EQ(1, 1);
}
