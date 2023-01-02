/// @file main.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-29
///
/// @copyright Copyright (c) 2022
///
///

#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "src/dataloader.h"
#include "src/visualizer.h"

void StereoDepthMap();

int main(int argc, char **argv) {
    research::interface::DataLoader dl("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);
    research::visualizer::Visualizer viz;

    viz.Update(dl[0]);

    cv::waitKey(0);

    return 0;
}