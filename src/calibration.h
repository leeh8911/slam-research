/// @file calibration.h
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-04
///
/// @copyright Copyright (c) 2023
///
///

#ifndef SRC_CALIBRATION_H_
#define SRC_CALIBRATION_H_

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

namespace research::domain {
class Calibration {
 public:
    Calibration() = default;
    Calibration(const cv::Mat& P, const cv::Mat& Tr);

    cv::Mat Rotation() const;
    cv::Mat Translation() const;
    cv::Mat Intrinsic() const;
    cv::Mat Distortion() const;

 private:
    cv::Mat rotation_vector_{};
    cv::Mat translation_vector_{};
    cv::Mat intrinsic_{};
    cv::Mat distortion_{};
};
}  // namespace research::domain

#endif  // SRC_CALIBRATION_H_
