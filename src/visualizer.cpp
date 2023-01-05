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
#include <cstdlib>
#include <ctime>
#include <vector>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "src/dataloader.h"
#include "src/timer.h"

namespace research::viz {

Visualizer::Visualizer() { cv::namedWindow(kMainWindowName); }
Visualizer::~Visualizer() { cv::destroyWindow(kMainWindowName); }

inf::PointCloud* SamplingPointCloud(inf::PointCloud* pc, double prob) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    pc->erase(std::remove_if(std::begin(*pc), std::end(*pc),
                             [&prob](const cv::Point3d& src) {
                                 return (static_cast<double>(std::rand() % 10000) / 10000.0 <= prob);
                             }),
              std::end(*pc));

    return pc;
}

cv::Mat* Project(cv::Mat* image, inf::PointCloud* pointcloud, domain::Calibration* calibration) {
    pointcloud = SamplingPointCloud(pointcloud, 0.5);

    std::vector<cv::Point2d> projected;
    projected.reserve(pointcloud->size());

    cv::projectPoints(*pointcloud, calibration->Rotation(), calibration->Translation(), calibration->Intrinsic(),
                      calibration->Distortion(), projected);

    for (const auto& p : projected) {
        cv::circle(*image, p, 1, cv::Scalar(0, 0, 255), 1, cv::LineTypes::LINE_AA);
    }

    return image;
}

void Visualizer::Update(inf::FrameData& frame_data) const {
    cv::Mat* img = Project(&(frame_data.cam_list_[2]), &(frame_data.pointcloud_), &(frame_data.calib_list_[2]));

    cv::imshow(kMainWindowName, *img);
}
}  // namespace research::viz
