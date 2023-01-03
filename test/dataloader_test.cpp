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

namespace research::interface::test {
TEST(DataLoaderIntegrateTest, LoadSequence00Case) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    EXPECT_EQ(4541, dataloader.TimeSize());
}

TEST(DataLoaderIntegrateTest, LoadSequenceAllCase) {
    for (size_t seq_index = 0; seq_index <= 21; ++seq_index) {
        DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", seq_index);

        EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam1Size());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam2Size());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.Cam3Size());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.VelodyneSize());
        EXPECT_EQ(dataloader.Cam0Size(), dataloader.TimeSize());
    }
}

TEST(DataLoaderIntegrateTest, CalibrationCase) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    auto calib_map = dataloader.Calibration();

    EXPECT_EQ(0.5417205509475317, calib_map["b_gray"].dot(1));
    EXPECT_EQ(0.5368457017809494, calib_map["b_rgb"].dot(1));
}

TEST(DataLoaderIntegrateTest, TimestampCase) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    auto timestamps = dataloader.Timestamps();

    EXPECT_EQ(0.0, timestamps[0]);
}

TEST(DataLoaderIntegrateTest, ReadDataCase) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);
    auto result = dataloader[0];

    EXPECT_EQ(376, result.cam0_.rows);
    EXPECT_EQ(1241, result.cam0_.cols);
    EXPECT_EQ(124668, result.pointcloud_.size());
}

TEST(DataLoaderIntegrateTest, ViewCalibration) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    auto cal = dataloader.Calibration();

    std::cout << "Cam 00\n";
    std::cout << cal["P_rect_00"] << "\n";
    std::cout << cal["T_cam0_velo"] << "\n";
    std::cout << cal["K_cam0"] << "\n";

    std::cout << "Cam 01\n";
    std::cout << cal["P_rect_01"] << "\n";
    std::cout << cal["T_cam1_velo"] << "\n";
    std::cout << cal["K_cam1"] << "\n";

    std::cout << "Cam 02\n";
    std::cout << cal["P_rect_02"] << "\n";
    std::cout << cal["T_cam2_velo"] << "\n";
    std::cout << cal["K_cam2"] << "\n";

    std::cout << "Cam 03\n";
    std::cout << cal["P_rect_03"] << "\n";
    std::cout << cal["T_cam3_velo"] << "\n";
    std::cout << cal["K_cam3"] << "\n";

    std::cout << "Tr??\n";
    std::cout << cal["Tr"] << "\n";
}

TEST(DataLoaderIntegrateTest, ViewCalibration2) {
    DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    auto cal = dataloader.Calibration();

    cv::Mat projection_matrix = cal["P_rect_03"] * cal["T_cam3_velo"];
    cv::Mat cam_instrinsic(3, 3, cv::DataType<double>::type);
    cv::Mat rotation(3, 3, cv::DataType<double>::type);
    cv::Mat translation_homogeneous(4, 1, cv::DataType<double>::type);

    cv::decomposeProjectionMatrix(projection_matrix, cam_instrinsic, rotation, translation_homogeneous);

    cv::Mat translation(3, 1, cv::DataType<double>::type);
    cv::convertPointsFromHomogeneous(translation_homogeneous.reshape(4), translation);

    std::cout << "projection matrix\n";
    std::cout << projection_matrix << "\n";

    std::cout << "cam_instrinsic matrix\n";
    std::cout << cam_instrinsic << "\n";
    std::cout << cal["K_cam3"] << "\n";

    cv::Mat Transform;
    cv::Mat translation2 = translation.reshape(1).t();
    cv::hconcat(rotation, translation2, Transform);

    std::cout << "Transform matrix\n";
    std::cout << Transform << "\n";
    std::cout << cal["T_cam3_velo"] << "\n";
}
}  // namespace research::interface::test
