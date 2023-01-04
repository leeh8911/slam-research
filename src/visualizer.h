/// @file visualizer.h
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-01
///
/// @copyright Copyright (c) 2023
///
///

#ifndef SRC_VISUALIZER_H_
#define SRC_VISUALIZER_H_

#include <string>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "src/dataloader.h"

namespace research::visualizer {
class Visualizer {
 public:
    Visualizer();
    ~Visualizer();

    void Update(const inf::FrameData& frame) const;

 private:
    const std::string kMainWindowName = "Main Window";
};
}  // namespace research::visualizer
#endif  // SRC_VISUALIZER_H_
