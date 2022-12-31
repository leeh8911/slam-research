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

#include <string>
#include <vector>

namespace research::interface
{
class DataLoader
{
  public:
    DataLoader(std::string base, size_t sequence);

    size_t Cam0Size() const;
    size_t Cam1Size() const;
    size_t Cam2Size() const;
    size_t Cam3Size() const;
    size_t VelodyneSize() const;

  private:
    std::vector<std::string> cam0_files{};
    std::vector<std::string> cam1_files{};
    std::vector<std::string> cam2_files{};
    std::vector<std::string> cam3_files{};
    std::vector<std::string> velodyne_files{};
};
} // namespace research::interface

#endif // SRC_DATALOADER_H_
