/// @file visualizer.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-01
///
/// @copyright Copyright (c) 2023
///
///

#include "src/visualizer.h"

#include <algorithm>
#include <vector>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "src/dataloader.h"

namespace research::visualizer {

Visualizer::Visualizer() { cv::namedWindow(kMainWindowName); }
Visualizer::~Visualizer() { cv::destroyWindow(kMainWindowName); }

cv::Mat Project(const cv::Mat& image, const interface::PointCloud& pointcloud, const cv::Mat& transformation) {
    cv::Mat result = image.clone();

    std::vector<cv::Point2d> projected{};
    projected.reserve(pointcloud.size());

    cv::Mat intrinsic(3, 3, cv::DataType<double>::type);
    cv::Mat rotation(3, 3, cv::DataType<double>::type);
    cv::Mat translation_homo(4, 1, cv::DataType<double>::type);

    cv::decomposeProjectionMatrix(transformation, intrinsic, rotation, translation_homo);

    cv::Mat translation(3, 1, cv::DataType<double>::type);  // translation vector
    cv::convertPointsFromHomogeneous(translation_homo.reshape(4, 1), translation);

    cv::Mat no_distortion = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

    cv::Mat rotation_vector(3, 1, cv::DataType<double>::type);  // rodrigues rotation matrix
    cv::Rodrigues(rotation, rotation_vector);

    cv::projectPoints(pointcloud, rotation_vector, translation, intrinsic, no_distortion, projected);
    for (const auto& p : projected) {
    }

    return result;
}

void Visualizer::Update(const interface::FrameData& frame_data) const {
    cv::Mat gray_view;
    cv::Mat project_cam0 =
        Project(frame_data.cam0_, frame_data.pointcloud_, frame_data.calibration_.find("P_rect_00")->second,
                frame_data.calibration_.find("T_cam0_velo")->second);
    cv::hconcat(frame_data.cam0_, frame_data.cam1_, gray_view);
    cv::cvtColor(gray_view, gray_view, cv::COLOR_GRAY2RGB);

    cv::Mat rgb_view;
    cv::hconcat(frame_data.cam2_, frame_data.cam3_, rgb_view);

    cv::Mat view;
    cv::vconcat(gray_view, rgb_view, view);

    cv::imshow(kMainWindowName, view);
}
}  // namespace research::visualizer
