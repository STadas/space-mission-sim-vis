#include <gtest/gtest.h>

TEST(ExampleTest, IsTrue)
{
    // Arrange
    bool shouldBeTrue{};

    // Act
    shouldBeTrue = true;

    // Assert
    ASSERT_EQ(true, shouldBeTrue);
}
