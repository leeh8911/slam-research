/// @file main.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2022-12-29
///
/// @copyright Copyright (c) 2022
///
///

#include <chrono>
#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "src/dataloader.h"
#include "src/timer.h"
#include "src/visualizer.h"

int main(int argc, char **argv) {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::system_clock;

    research::inf::DataLoader dl("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);
    research::viz::Visualizer gui;

    size_t index = 0;
    size_t max_step = dl.TimeSize() - 1;

    while (index < max_step) {
        auto start = system_clock::now();

        gui.Update(dl[index]);

        auto time_rest = static_cast<size_t>((dl.Timestamps()[index + 1] - dl.Timestamps()[index]) * 1000.);
        if (time_rest > 0) {
            cv::waitKey(static_cast<int>(time_rest));
        }

        index++;
    }

    cv::waitKey(0);

    return 0;
}