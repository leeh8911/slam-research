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
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <vector>

#include "opencv2/core.hpp"


int main(int argc, char **argv) {
    using namespace std::string_literals;  // NOLINT
    std::cout << "Hello world" << std::endl;

    const std::string image2_path =
        "D:\\sangwon\\dataset\\kitti\\odometry\\data_odometry_color\\dataset\\sequences\\00\\image_2\\000000.png"s;

    const std::string image3_path =
        "D:\\sangwon\\dataset\\kitti\\odometry\\data_odometry_color\\dataset\\sequences\\00\\image_3\\000000.png"s;

    cv::Mat image2 = cv::imread(image2_path);
    cv::Mat image3 = cv::imread(image3_path);

    cv::Ptr<cv::SIFT> detector = nullptr;
    detector = cv::SiftFeatureDetector::create();

    cv::Ptr<cv::SIFT> extractor = nullptr;
    extractor = cv::SiftDescriptorExtractor::create();

    std::vector<cv::KeyPoint> keypoints2;
    std::vector<cv::KeyPoint> keypoints3;
    detector->detect(image2, keypoints2);
    detector->detect(image3, keypoints3);

    cv::Mat descriptions2;
    cv::Mat descriptions3;
    extractor->compute(image2, keypoints2, descriptions2);
    extractor->compute(image3, keypoints3, descriptions3);

    cv::drawKeypoints(image2, keypoints2, image2);
    cv::drawKeypoints(image3, keypoints3, image3);

    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(descriptions2, descriptions3, matches);

    double max_dist = 0.0, min_dist = 1000.0;
    for (size_t i = 0; i < matches.size(); ++i) {
        double dist = matches[i].distance;
        if (dist < min_dist) {
            min_dist = dist;
        }
        if (dist > max_dist) {
            max_dist = dist;
        }
    }

    std::vector<cv::DMatch> good_matches;
    for (size_t i = 0; i < matches.size(); ++i) {
        if (matches[i].distance < 10 * min_dist) {
            good_matches.push_back(matches[i]);
        }
    }

    cv::Mat matched_image;
    cv::drawMatches(image2, keypoints2, image3, keypoints3, good_matches, matched_image, cv::Scalar::all(-1),
                    cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    cv::namedWindow("image", 0);
    cv::imshow("image", matched_image);
    cv::waitKey();

    return 0;
}
