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
#include <iostream> // to be removed
#include <string>
#include <vector>

namespace research::interface
{
const std::string cam0_name = "image_0";
const std::string cam1_name = "image_1";
const std::string cam2_name = "image_2";
const std::string cam3_name = "image_3";
const std::string velodyne_name = "velodyne";

const auto ToString = [](size_t src, size_t pad_size) {
    std::string result = std::to_string(src);
    if (result.length() < pad_size)
    {
        result = std::string(pad_size - result.length(), '0') + result;
    }
    return result;
};

const auto GlobFiles = [](std::filesystem::path path) {
    std::vector<std::string> result{};

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        result.emplace_back(entry.path().string());
    }
    return result;
};

DataLoader::DataLoader(std::string base, size_t sequence)
{
    std::filesystem::path base_path = base;
    base_path.append("dataset").append("sequences").append(ToString(sequence, 2));

    cam0_files = GlobFiles(std::filesystem::path(base_path).append(cam0_name));
    cam1_files = GlobFiles(std::filesystem::path(base_path).append(cam1_name));
    cam2_files = GlobFiles(std::filesystem::path(base_path).append(cam2_name));
    cam3_files = GlobFiles(std::filesystem::path(base_path).append(cam3_name));
    velodyne_files = GlobFiles(std::filesystem::path(base_path).append(velodyne_name));
}

size_t DataLoader::Cam0Size() const
{
    return cam0_files.size();
}

size_t DataLoader::Cam1Size() const
{
    return cam1_files.size();
}

size_t DataLoader::Cam2Size() const
{
    return cam2_files.size();
}

size_t DataLoader::Cam3Size() const
{
    return cam3_files.size();
}

size_t DataLoader::VelodyneSize() const
{
    return velodyne_files.size();
}
} // namespace research::interface
