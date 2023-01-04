/// @file calibration.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-04
///
/// @copyright Copyright (c) 2023
///
///

#include "src/calibration.h"

#include <iostream>
#include <vector>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

namespace research::domain {
Calibration::Calibration(const cv::Mat& P, const cv::Mat& Tr) {
    cv::Mat T = cv::Mat::eye(4, 4, CV_64F);
    T.at<double>(0, 3) = P.at<double>(0, 3) / P.at<double>(0, 0);

    cv::Mat T_cam_velo = Tr;
    cv::vconcat(T_cam_velo, cv::Mat1d(1, 4, std::vector<double>({0, 0, 0, 1}).data()), T_cam_velo);

    T_cam_velo = T * T_cam_velo;
    cv::Mat transform = P * T_cam_velo;

    cv::Mat intrinsic(3, 3, cv::DataType<double>::type);
    cv::Mat rotation(3, 3, cv::DataType<double>::type);
    cv::Mat translation_homogeneous(4, 1, cv::DataType<double>::type);

    cv::decomposeProjectionMatrix(transform, intrinsic, rotation, translation_homogeneous);

    cv::Mat translation(3, 1, cv::DataType<double>::type);
    cv::convertPointsFromHomogeneous(translation_homogeneous.reshape(4), translation);
    translation = translation.reshape(1).t();

    cv::Mat rotation_vector(3, 1, cv::DataType<double>::type);  // rodrigues rotation matrix
    cv::Rodrigues(rotation, rotation_vector);

    cv::Mat no_distortion = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

    rotation_vector_ = rotation_vector.clone();
    translation_vector_ = translation.clone();
    intrinsic_ = intrinsic.clone();
    distortion_ = no_distortion.clone();
}

cv::Mat Calibration::Rotation() const { return rotation_vector_; }

cv::Mat Calibration::Translation() const { return translation_vector_; }

cv::Mat Calibration::Intrinsic() const { return intrinsic_; }

cv::Mat Calibration::Distortion() const { return distortion_; }

}  // namespace research::domain
