/// @file dataloader.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-31
///
/// @copyright Copyright (c) 2022
///
///

#include "src/dataloader.h"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>  // to be removed
#include <iterator>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "src/calibration.h"
#include "src/timer.h"

namespace research::inf {
FrameData::FrameData(const std::array<cv::Mat, 4>& cam_list, const PointCloud& pointcloud,
                     const std::array<Calibration, 4>& calib_list)
    : cam_list_(cam_list), pointcloud_(pointcloud), calib_list_{calib_list} {}

const std::array<std::string, 4> DataLoader::kCamNames = {"image_0", "image_1", "image_2", "image_3"};
const std::string DataLoader::kVelodyneName = "velodyne";

std::string DataLoader::ToString(size_t src, size_t pad_size) {
    std::string result = std::to_string(src);
    if (result.length() < pad_size) {
        result = std::string(pad_size - result.length(), '0') + result;
    }
    return result;
}

std::vector<std::filesystem::path> DataLoader::GlobFiles(std::filesystem::path path) {
    std::vector<std::filesystem::path> result{};

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        result.emplace_back(entry.path());
    }
    return result;
}

DataLoader::DataLoader(std::string base, size_t sequence)
    : base_path_(std::filesystem::path(base).append("sequences").append(ToString(sequence, 2))) {
    cam_files_[0] = GlobFiles(std::filesystem::path(base_path_).append(kCamNames[0]));
    cam_files_[1] = GlobFiles(std::filesystem::path(base_path_).append(kCamNames[1]));
    cam_files_[2] = GlobFiles(std::filesystem::path(base_path_).append(kCamNames[2]));
    cam_files_[3] = GlobFiles(std::filesystem::path(base_path_).append(kCamNames[3]));
    velodyne_files_ = GlobFiles(std::filesystem::path(base_path_).append(kVelodyneName));

    auto calibration_map = ReadCalibrationFile(base_path_);
    calibration_list_[0] = Calibration(calibration_map["P0"], calibration_map["Tr"]);
    calibration_list_[1] = Calibration(calibration_map["P1"], calibration_map["Tr"]);
    calibration_list_[2] = Calibration(calibration_map["P2"], calibration_map["Tr"]);
    calibration_list_[3] = Calibration(calibration_map["P3"], calibration_map["Tr"]);

    timestamps_ = ReadTimestamp(base_path_);
}

PointCloud ReadPointCloud(std::filesystem::path file_path) {
    std::ifstream fs;
    fs.open(file_path, std::ios::in | std::ios::binary);
    PointCloud result{};

    char buf[16];
    while (fs.read(buf, 16)) {
        cv::Point3d point{
            *reinterpret_cast<float*>(buf),
            *reinterpret_cast<float*>(buf + 4),
            *reinterpret_cast<float*>(buf + 8),
        };
        result.emplace_back(point);
    }

    return result;
}

FrameData DataLoader::operator[](size_t index) {
    return FrameData({cv::imread(cam_files_[0][index].string().c_str(), cv::ImreadModes::IMREAD_GRAYSCALE),
                      cv::imread(cam_files_[1][index].string().c_str(), cv::ImreadModes::IMREAD_GRAYSCALE),
                      cv::imread(cam_files_[2][index].string().c_str(), cv::ImreadModes::IMREAD_COLOR),
                      cv::imread(cam_files_[3][index].string().c_str(), cv::ImreadModes::IMREAD_COLOR)},
                     ReadPointCloud(velodyne_files_[index]), calibration_list_);
}

std::vector<double> DataLoader::Timestamps() { return timestamps_; }

size_t DataLoader::TimeSize() const { return timestamps_.size(); }

std::vector<double> DataLoader::ReadTimestamp(std::filesystem::path base_path) {
    std::ifstream fs;
    fs.open(base_path.append("times.txt"), std::ios::in);

    std::vector<double> result;
    std::string value;

    while (std::getline(fs, value)) {
        result.emplace_back(std::atof(value.c_str()));
    }
    return result;
}

std::unordered_map<std::string, cv::Mat> DataLoader::ReadCalibrationFile(std::filesystem::path base_path) {
    std::unordered_map<std::string, cv::Mat> result{};
    std::ifstream fs;
    fs.open(base_path.append("calib.txt"), std::ios::in);

    std::string l1;
    while (getline(fs, l1)) {
        std::stringstream ss(l1);
        std::string key;
        ss >> key;
        key.pop_back();  // remove ";"

        std::vector<double> v;
        v.reserve(12);

        std::string value;
        while (ss >> value) {
            v.emplace_back(std::atof(value.c_str()));
        }
        result.emplace(key, cv::Mat1d(3, 4, v.data()).clone());
    }

    fs.close();
    return result;
}
}  // namespace research::inf
