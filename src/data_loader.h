/// @file data_loader.h
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-30
///
/// @copyright Copyright (c) 2022
///
///

#ifndef SRC_DATA_LOADER_H_
#define SRC_DATA_LOADER_H_

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <opencv2/core.hpp>

namespace slam_research::interface
{

class ImageSequence
{
  public:
    explicit ImageSequence(std::string base_path, size_t sequence = 0);

    size_t Size() const;

    std::pair<cv::Mat, cv::Mat> operator[](size_t index) const;

  private:
    std::vector<std::string> left_files_{};
    std::vector<std::string> right_files_{};
    cv::Mat left_calib{};
    cv::Mat right_calib{};

    inline static const std::string kBaseDirectory = "data_odometry_color\\dataset\\sequences";
    inline static const std::string kLeftImageDirectory = "image_2";
    inline static const std::string kRightImageDirectory = "image_3";
};

class DataLoader
{
  public:
    explicit DataLoader(std::string base, size_t sequence = 0);

  private:
};

} // namespace slam_research::interface
#endif // SRC_DATA_LOADER_H_
