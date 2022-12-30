/// @file data_loader_test.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-30
///
/// @copyright Copyright (c) 2022
///
///

#include "src/data_loader.h"

#include <gtest/gtest.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

namespace slam_research::interface::test
{

TEST(ImageSequenceTest, GetSize_Sequence0_Case)
{
    ImageSequence seq("D:\\sangwon\\dataset\\kitti\\odometry");

    EXPECT_EQ(4541, seq.Size());
}

TEST(ImageSequenceTest, ImageReadIndex0Case)
{
    ImageSequence seq = ImageSequence("D:\\sangwon\\dataset\\kitti\\odometry");

    auto images = seq[0];

    EXPECT_EQ(376, images.first.rows);
    EXPECT_EQ(1241, images.first.cols);
    EXPECT_EQ(CV_8UC3, images.first.type());
    EXPECT_EQ(3, images.first.channels());
    EXPECT_EQ(376, images.second.rows);
    EXPECT_EQ(1241, images.second.cols);
    EXPECT_EQ(CV_8UC3, images.second.type());
    EXPECT_EQ(3, images.second.channels());
}
} // namespace slam_research::interface::test