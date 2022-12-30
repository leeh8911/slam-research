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

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void StereoDepthMap();

int main(int argc, char **argv)
{
    std::cout << "Hello world\n";

    StereoDepthMap();

    return 0;
}

void StereoDepthMap()
{
    int num_disparity = 8;
    int block_size = 5;
    std::string left_image_name =
        "D:\\sangwon\\dataset\\kitti\\odometry\\data_odometry_color\\dataset\\sequences\\00\\image_2\\000000.png";
    std::string right_image_name =
        "D:\\sangwon\\dataset\\kitti\\odometry\\data_odometry_color\\dataset\\sequences\\00\\image_3\\000000.png";

    cv::Mat left = cv::imread(left_image_name);
    cv::Mat right = cv::imread(right_image_name);
    cv::Mat disp, disparity; // Disparity
    std::string disparity_window = "disparity";

    cv::Ptr<cv::StereoSGBM> stereo = cv::StereoSGBM::create();

    stereo->setNumDisparities(num_disparity * 16);
    stereo->setBlockSize(block_size);

    stereo->compute(left, right, disp);
    disp.convertTo(disparity, CV_8U);
    cv::applyColorMap(disparity, disparity, cv::COLORMAP_JET);

    cv::imshow(disparity_window.c_str(), disparity);

    cv::waitKey(0);
}