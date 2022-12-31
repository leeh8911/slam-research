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

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>  // to be removed
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "opencv2/core.hpp"

namespace research::interface {
const std::string kCam0Name = "image_0";
const std::string kCam1Name = "image_1";
const std::string kCam2Name = "image_2";
const std::string kCan3Name = "image_3";
const std::string kVelodyneName = "velodyne";

const auto to_string = [](size_t src, size_t pad_size) {
    std::string result = std::to_string(src);
    if (result.length() < pad_size) {
        result = std::string(pad_size - result.length(), '0') + result;
    }
    return result;
};

const auto glob_files = [](std::filesystem::path path) {
    std::vector<std::string> result{};

    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        result.emplace_back(entry.path().string());
    }
    return result;
};

DataLoader::DataLoader(std::string base, size_t sequence) {
    std::filesystem::path base_path = base;
    base_path.append("sequences").append(to_string(sequence, 2));

    base_path_ = base_path;
    cam0_files_ = glob_files(std::filesystem::path(base_path).append(kCam0Name));
    cam1_files_ = glob_files(std::filesystem::path(base_path).append(kCam1Name));
    cam2_files_ = glob_files(std::filesystem::path(base_path).append(kCam2Name));
    cam3_files_ = glob_files(std::filesystem::path(base_path).append(kCan3Name));
    velodyne_files_ = glob_files(std::filesystem::path(base_path).append(kVelodyneName));

    calibration_ = ReadCalibration(base_path_);
}

std::unordered_map<std::string, cv::Mat> DataLoader::Calibration() { return calibration_; }

size_t DataLoader::Cam0Size() const { return cam0_files_.size(); }

size_t DataLoader::Cam1Size() const { return cam1_files_.size(); }

size_t DataLoader::Cam2Size() const { return cam2_files_.size(); }

size_t DataLoader::Cam3Size() const { return cam3_files_.size(); }

size_t DataLoader::VelodyneSize() const { return velodyne_files_.size(); }

std::unordered_map<std::string, cv::Mat> DataLoader::ReadCalibration(std::filesystem::path base_path) {
    auto file_data = ReadCalibrationFile(base_path);
    std::unordered_map<std::string, cv::Mat> result{};

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
}  // namespace research::interface
