/// @file timer.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-05
///
/// @copyright Copyright (c) 2023
///
///

#include "src/timer.h"

#include <chrono>
#include <iostream>
#include <string>
#include <utility>

namespace research::utils {

Timer::Timer(std::string name) : name_{std::move(name)}, start_time_{std::chrono::system_clock::now()} {}
Timer::~Timer() {
    std::cout
        << name_ << " - Ellipse("
        << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time_).count()
        << " ms)\n";
}
}  // namespace research::utils