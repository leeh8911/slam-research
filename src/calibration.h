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
    Calibration(const cv::Mat& P, const cv::Mat& Tr);

 private:
};
}  // namespace research::domain

#endif  // SRC_CALIBRATION_H_
