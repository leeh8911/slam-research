/// @file data_loader.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-30
///
/// @copyright Copyright (c) 2022
///
///

#include "src/data_loader.h"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace slam_research::interface
{

const auto find_all = [](std::string path) {
    std::vector<std::string> files{};
    std::filesystem::directory_iterator it(path);

    for (const auto &entry : it)
    {
        files.emplace_back(entry.path().string());
    }

    return files;
};

ImageSequence::ImageSequence(std::string base_path, size_t sequence)
    : left_files_{}, right_files_{}, left_calib{}, right_calib{}
{
    std::stringstream ss;
    ss << base_path << "\\" + kBaseDirectory + "\\";
    ss << std::setfill('0') << std::setw(2) << sequence;
    std::string seq_path = ss.str();

    left_files_ = find_all(seq_path + "\\" + kLeftImageDirectory + "\\");
    right_files_ = find_all(seq_path + "\\" + kRightImageDirectory + "\\");

    if (left_files_.size() != right_files_.size())
    {
        std::invalid_argument("Check number of files in left and right image");
    }
}

size_t ImageSequence::Size() const
{
    return left_files_.size();
}

std::pair<cv::Mat, cv::Mat> ImageSequence::operator[](size_t index) const
{
    return std::make_pair(cv::imread(left_files_[index]), cv::imread(right_files_[index]));
}

} // namespace slam_research::interface
