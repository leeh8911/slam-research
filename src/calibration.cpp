/// @file calibration.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-04
///
/// @copyright Copyright (c) 2023
///
///

#include "src/calibration.h"

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"

namespace research::domain {
Calibration::Calibration(const cv::Mat& P, const cv::Mat& Tr) {}

std::tuple<cv::Mat, cv::Mat, cv::Mat, cv::Mat> ParseCalibration(const cv::Mat& P0, const cv::Mat& P,
                                                                const cv::Mat& Tr) {
    cv::Mat T = cv::Mat::eye(4, 4, CV_64F);
    T.at<double>(0, 3) = P.at<double>(0, 3) / P.at<double>(0, 0);

    cv::Mat Tr_homogeneous = Tr;
    cv::vconcat(Tr, cv::Mat1d(1, 4, std::vector<double>({0, 0, 0, 1}).data()).clone(), Tr_homogeneous);
    T *= Tr_homogeneous;

    cv::Mat projection_matrix = P * Tr_homogeneous;

    cv::Mat intrinsic(3, 3, cv::DataType<double>::type);
    cv::Mat rotation(3, 3, cv::DataType<double>::type);
    cv::Mat translation_homogeneous(4, 1, cv::DataType<double>::type);

    cv::decomposeProjectionMatrix(projection_matrix, intrinsic, rotation, translation_homogeneous);

    cv::Mat translation(3, 1, cv::DataType<double>::type);
    cv::convertPointsFromHomogeneous(translation_homogeneous.reshape(4), translation);
    translation = translation.reshape(1).t();

    cv::Mat rotation_vector(3, 1, cv::DataType<double>::type);  // rodrigues rotation matrix
    cv::Rodrigues(rotation, rotation_vector);

    cv::Mat no_distortion = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

    return std::make_tuple(rotation_vector, translation, intrinsic, no_distortion);
}

CalibrationData DataLoader::ReadCalibration(std::filesystem::path base_path) {
    auto file_data = ReadCalibrationFile(base_path);
    CalibrationData result{};

    result["P_rect_00"] = file_data["P0"];
    result["P_rect_01"] = file_data["P1"];
    result["P_rect_02"] = file_data["P2"];
    result["P_rect_03"] = file_data["P3"];
    result["Tr"] = file_data["Tr"];

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

    result["T_img0_velo"] = result["P_rect_00"] * result["T_cam0_velo"];
    result["T_img1_velo"] = result["P_rect_01"] * result["T_cam1_velo"];
    result["T_img2_velo"] = result["P_rect_02"] * result["T_cam2_velo"];
    result["T_img3_velo"] = result["P_rect_03"] * result["T_cam3_velo"];

    // rotation_vector, translation, intrinsic, no_distortion

    cv::Mat p_cam = cv::Mat1d(4, 1, std::vector<double>({0, 0, 0, 1}).data()).clone();
    cv::Mat p_velo0 = result["T_cam0_velo"].inv() * (p_cam);
    cv::Mat p_velo1 = result["T_cam1_velo"].inv() * (p_cam);
    cv::Mat p_velo2 = result["T_cam2_velo"].inv() * (p_cam);
    cv::Mat p_velo3 = result["T_cam3_velo"].inv() * (p_cam);

    result["b_gray"] = cv::Mat1d(1, 1, cv::norm(p_velo1 - p_velo0, 2));
    result["b_rgb"] = cv::Mat1d(1, 1, cv::norm(p_velo3 - p_velo2, 2));

    return result;
}
}  // namespace research::domain
