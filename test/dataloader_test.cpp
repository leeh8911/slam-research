/// @file data_loader_test.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-31
///
/// @copyright Copyright (c) 2022
///
///

#include "src/dataloader.h"

#include <gtest/gtest.h>

namespace research::interface::test
{
TEST(DataLoaderTest, LoadSequence00Case)
{
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry", 0);

    EXPECT_EQ(4541, dataloader.Cam0Size());
    EXPECT_EQ(4541, dataloader.Cam1Size());
    EXPECT_EQ(4541, dataloader.Cam2Size());
    EXPECT_EQ(4541, dataloader.Cam3Size());
    EXPECT_EQ(4541, dataloader.VelodyneSize());
}

TEST(DataLoaderIntegrateTest, LoadSequenceAllCase)
{
    for (size_t seq_index = 0; seq_index <= 21; ++seq_index)
    {

        DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry", seq_index);

        EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam1Size());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam2Size());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam3Size());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.VelodyneSize());
    }
}
} // namespace research::interface::test
