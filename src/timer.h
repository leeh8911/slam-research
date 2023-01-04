/// @file timer.h
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-05
///
/// @copyright Copyright (c) 2023
///
///

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <chrono>
#include <string>

namespace research::utils {

class Timer {
 public:
    explicit Timer(std::string name);
    ~Timer();

 private:
    std::chrono::time_point<std::chrono::system_clock> start_time_{};
    std::string name_{};
};

#define TIMER()                                                                                                        \
    using namespace std::string_literals; /* NOLINT */                                                                 \
    research::utils::Timer timer(__FILE__ + "("s + std::to_string(__LINE__) + ") "s + __FUNCTION__)
}  // namespace research::utils

#endif  // SRC_TIMER_H_
