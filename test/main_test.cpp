/// @file main_test.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-30
///
/// @copyright Copyright (c) 2022
///
///
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

#ifndef TEST_ALL
    ::testing::GTEST_FLAG(filter) = "*";
#endif
    return RUN_ALL_TESTS();
}