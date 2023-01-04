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

#include <array>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "opencv2/core.hpp"
#include "src/calibration.h"

namespace research::inf {

using PointCloud = std::vector<cv::Point3d>;
using Calibration = domain::Calibration;

struct FrameData {
    FrameData() = default;
    FrameData(const std::array<cv::Mat, 4>& cam_list, const PointCloud& pointcloud,
              const std::array<Calibration, 4>& calib_list);
    std::array<cv::Mat, 4> cam_list_{};
    PointCloud pointcloud_{};
    std::array<Calibration, 4> calib_list_{};
};

using FileList = std::vector<std::filesystem::path>;
class DataLoader {
 public:
    DataLoader(std::string base, size_t sequence);

    FrameData operator[](size_t index);

    std::vector<double> Timestamps();

    size_t TimeSize() const;

 private:
    std::unordered_map<std::string, cv::Mat> ReadCalibrationFile(std::filesystem::path base_path);
    std::vector<double> ReadTimestamp(std::filesystem::path base_path);

    std::vector<std::filesystem::path> GlobFiles(std::filesystem::path path);
    std::string ToString(size_t src, size_t pad_size);

    std::filesystem::path base_path_{};
    std::vector<double> timestamps_{};
    std::array<FileList, 4> cam_files_{};
    std::array<Calibration, 4> calibration_list_{};
    FileList velodyne_files_{};

    static const std::array<std::string, 4> kCamNames;
    static const std::string kVelodyneName;
};
}  // namespace research::inf

#endif  // SRC_DATALOADER_H_
