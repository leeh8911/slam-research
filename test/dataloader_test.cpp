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

#include <iostream>
#include <unordered_map>

#include "opencv2/calib3d.hpp"

namespace research::inf::test {
TEST(DataLoaderIntegrateTest, LoadSequence00Case) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    EXPECT_EQ(4541, dataloader.TimeSize());
}

TEST(DataLoaderIntegrateTest, LoadSequenceAllCase) {
    for (size_t seq_index = 0; seq_index <= 21; ++seq_index) {
        DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", seq_index);

        // EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam1Size());
        // EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam2Size());
        // EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam3Size());
        // EXPECT_EQ(dataloader.Cam0Size(), dataloader.VelodyneSize());
        // EXPECT_EQ(dataloader.Cam0Size(), dataloader.TimeSize());
    }
    EXPECT_FALSE(true);
}

TEST(DataLoaderIntegrateTest, CalibrationCase) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);
    EXPECT_FALSE(true);
    // auto calib_map = dataloader.Calibration();

    // EXPECT_EQ(0.5417205509475317, calib_map["b_gray"].dot(1));
    // EXPECT_EQ(0.5368457017809494, calib_map["b_rgb"].dot(1));
}

TEST(DataLoaderIntegrateTest, TimestampCase) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    auto timestamps = dataloader.Timestamps();

    EXPECT_EQ(0.0, timestamps[0]);
}

TEST(DataLoaderIntegrateTest, ReadDataCase) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);
    auto result = dataloader[0];
}
}  // namespace research::inf::test
