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
#include <unordered_map>
#include <utility>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

namespace research::interface {

FrameData::FrameData(const cv::Mat& cam0, const cv::Mat& cam1, const cv::Mat& cam2, const cv::Mat& cam3,
                     const PointCloud& pointcloud, const CalibrationData& calibration)
    : cam0_(cam0), cam1_(cam1), cam2_(cam2), cam3_(cam3), pointcloud_(pointcloud), calibration_(calibration) {}

const std::string DataLoader::kCam0Name = "image_0";
const std::string DataLoader::kCam1Name = "image_1";
const std::string DataLoader::kCam2Name = "image_2";
const std::string DataLoader::kCan3Name = "image_3";
const std::string DataLoader::kVelodyneName = "velodyne";

std::string DataLoader::ToString(size_t src, size_t pad_size) {
    std::string result = std::to_string(src);
    if (result.length() < pad_size) {
        result = std::string(pad_size - result.length(), '0') + result;
    }
    return result;
}

std::vector<std::string> DataLoader::GlobFiles(std::filesystem::path path) {
    std::vector<std::string> result{};

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        result.emplace_back(entry.path().string());
    }
    return result;
}

DataLoader::DataLoader(std::string base, size_t sequence)
    : base_path_(std::filesystem::path(base).append("sequences").append(ToString(sequence, 2))) {
    cam0_files_ = GlobFiles(std::filesystem::path(base_path_).append(kCam0Name));
    cam1_files_ = GlobFiles(std::filesystem::path(base_path_).append(kCam1Name));
    cam2_files_ = GlobFiles(std::filesystem::path(base_path_).append(kCam2Name));
    cam3_files_ = GlobFiles(std::filesystem::path(base_path_).append(kCan3Name));
    velodyne_files_ = GlobFiles(std::filesystem::path(base_path_).append(kVelodyneName));

    calibration_ = ReadCalibration(base_path_);
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
    return FrameData(cv::imread(cam0_files_[index], cv::ImreadModes::IMREAD_GRAYSCALE),
                     cv::imread(cam1_files_[index], cv::ImreadModes::IMREAD_GRAYSCALE),
                     cv::imread(cam2_files_[index], cv::ImreadModes::IMREAD_COLOR),
                     cv::imread(cam3_files_[index], cv::ImreadModes::IMREAD_COLOR),
                     ReadPointCloud(velodyne_files_[index]), calibration_);
}

CalibrationData DataLoader::Calibration() { return calibration_; }
std::vector<double> DataLoader::Timestamps() { return timestamps_; }

size_t DataLoader::Cam0Size() const { return cam0_files_.size(); }

size_t DataLoader::Cam1Size() const { return cam1_files_.size(); }

size_t DataLoader::Cam2Size() const { return cam2_files_.size(); }

size_t DataLoader::Cam3Size() const { return cam3_files_.size(); }

size_t DataLoader::VelodyneSize() const { return velodyne_files_.size(); }

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

CalibrationData DataLoader::ReadCalibration(std::filesystem::path base_path) {
    auto file_data = ReadCalibrationFile(base_path);
    CalibrationData result{};

    result["P_rect_00"] = file_data["P0"];
    result["P_rect_01"] = file_data["P1"];
    result["P_rect_02"] = file_data["P2"];
    result["P_rect_03"] = file_data["P3"];

    cv::Mat T1 = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat T2 = cv::Mat::eye(4, 4, CV_64F);
    cv::Mat T3 = cv::Mat::eye(4, 4, CV_64F);

    T1.at<double>(0, 3) = file_data["P1"].at<double>(0, 3) / file_data["P1"].at<double>(0, 0);
    T2.at<double>(0, 3) = file_data["P2"].at<double>(0, 3) / file_data["P2"].at<double>(0, 0);
    T3.at<double>(0, 3) = file_data["P3"].at<double>(0, 3) / file_data["P3"].at<double>(0, 0);

    result["T_cam0_velo"] = file_data["Tr"];
    cv::vconcat(result["T_cam0_velo"], cv::Mat1d(1, 4, std::vector<double>({0, 0, 0, 1}).data()).clone(),
                result["T_cam0_velo"]);

    result["T_cam1_velo"] = T1 * result["T_cam0_velo"];
    result["T_cam2_velo"] = T2 * result["T_cam0_velo"];
    result["T_cam3_velo"] = T3 * result["T_cam0_velo"];

    result["K_cam0"] = file_data["P0"].colRange(0, 3).rowRange(0, 3);
    result["K_cam1"] = file_data["P1"].colRange(0, 3).rowRange(0, 3);
    result["K_cam2"] = file_data["P2"].colRange(0, 3).rowRange(0, 3);
    result["K_cam3"] = file_data["P3"].colRange(0, 3).rowRange(0, 3);

    cv::Mat p_cam = cv::Mat1d(4, 1, std::vector<double>({0, 0, 0, 1}).data()).clone();
    cv::Mat p_velo0 = result["T_cam0_velo"].inv() * (p_cam);
    cv::Mat p_velo1 = result["T_cam1_velo"].inv() * (p_cam);
    cv::Mat p_velo2 = result["T_cam2_velo"].inv() * (p_cam);
    cv::Mat p_velo3 = result["T_cam3_velo"].inv() * (p_cam);

    result["b_gray"] = cv::Mat1d(1, 1, cv::norm(p_velo1 - p_velo0, 2));
    result["b_rgb"] = cv::Mat1d(1, 1, cv::norm(p_velo3 - p_velo2, 2));

    return result;
}

CalibrationData DataLoader::ReadCalibrationFile(std::filesystem::path base_path) {
    CalibrationData result{};
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
}  // namespace research::interface
