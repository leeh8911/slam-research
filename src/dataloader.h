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

#include <opencv2/core.hpp>

namespace research::interface
{
struct FrameData
{
    cv::Mat cam0;
    cv::Mat cam1;
    cv::Mat cam2;
    cv::Mat cam3;
};

class DataLoader
{
  public:
    DataLoader(std::string base, size_t sequence);

    FrameData operator[](size_t index);

    std::unordered_map<std::string, cv::Mat> Calibration();
    size_t Cam0Size() const;
    size_t Cam1Size() const;
    size_t Cam2Size() const;
    size_t Cam3Size() const;
    size_t VelodyneSize() const;

  private:
    std::unordered_map<std::string, cv::Mat> ReadCalibration(std::filesystem::path base_path);
    std::unordered_map<std::string, cv::Mat> ReadCalibrationFile(std::filesystem::path base_path);

    std::filesystem::path base_path_{};
    std::vector<double> timestamps_{};
    std::vector<std::string> cam0_files_{};
    std::vector<std::string> cam1_files_{};
    std::vector<std::string> cam2_files_{};
    std::vector<std::string> cam3_files_{};
    std::vector<std::string> velodyne_files_{};
    std::unordered_map<std::string, cv::Mat> calibration_;
};
} // namespace research::interface

#endif // SRC_DATALOADER_H_
