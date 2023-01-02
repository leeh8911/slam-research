/// @file dataloader.h
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-31
///
/// @copyright Copyright (c) 2022
///
///

#ifndef SRC_DATALOADER_H_
#define SRC_DATALOADER_H_

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "opencv2/core.hpp"

namespace research::interface {

using CalibrationData = std::unordered_map<std::string, cv::Mat>;
using PointCloud = std::vector<cv::Point3d>;

struct FrameData {
    FrameData(const cv::Mat& cam0, const cv::Mat& cam1, const cv::Mat& cam2, const cv::Mat& cam3,
              const PointCloud& pointcloud, const CalibrationData& calibration);
    cv::Mat cam0_;
    cv::Mat cam1_;
    cv::Mat cam2_;
    cv::Mat cam3_;
    PointCloud pointcloud_;
    CalibrationData calibration_;
};

class DataLoader {
 public:
    DataLoader(std::string base, size_t sequence);

    FrameData operator[](size_t index);

    CalibrationData Calibration();
    std::vector<double> Timestamps();

    size_t Cam0Size() const;
    size_t Cam1Size() const;
    size_t Cam2Size() const;
    size_t Cam3Size() const;
    size_t VelodyneSize() const;
    size_t TimeSize() const;

 private:
    CalibrationData ReadCalibration(std::filesystem::path base_path);
    CalibrationData ReadCalibrationFile(std::filesystem::path base_path);
    std::vector<double> ReadTimestamp(std::filesystem::path base_path);

    std::vector<std::string> GlobFiles(std::filesystem::path path);
    std::string ToString(size_t src, size_t pad_size);

    std::filesystem::path base_path_{};
    std::vector<double> timestamps_{};
    std::vector<std::string> cam0_files_{};
    std::vector<std::string> cam1_files_{};
    std::vector<std::string> cam2_files_{};
    std::vector<std::string> cam3_files_{};
    std::vector<std::string> velodyne_files_{};
    CalibrationData calibration_{};

    static const std::string kCam0Name;
    static const std::string kCam1Name;
    static const std::string kCam2Name;
    static const std::string kCan3Name;
    static const std::string kVelodyneName;
};
}  // namespace research::interface

#endif  // SRC_DATALOADER_H_
