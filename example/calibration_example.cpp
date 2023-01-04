/// @file calibration_example.cpp
/// @author sangwon lee (leeh8911@gmail.com)
/// @brief
/// @version 0.1
/// @date 2023-01-04
///
/// @copyright Copyright (c) 2023
///
///

int main(int argc, char** argv) {
    // DataLoader dataloader = DataLoader("D:\\sangwon\\dataset\\kitti\\odometry\\dataset", 0);

    // auto cal = dataloader.Calibration();

    // cv::Mat projection_matrix = cal["P_rect_03"] * cal["T_cam3_velo"];
    // cv::Mat cam_instrinsic(3, 3, cv::DataType<double>::type);
    // cv::Mat rotation(3, 3, cv::DataType<double>::type);
    // cv::Mat translation_homogeneous(4, 1, cv::DataType<double>::type);

    // cv::decomposeProjectionMatrix(projection_matrix, cam_instrinsic, rotation, translation_homogeneous);

    // cv::Mat translation(3, 1, cv::DataType<double>::type);
    // cv::convertPointsFromHomogeneous(translation_homogeneous.reshape(4), translation);

    // std::cout << "projection matrix\n";
    // std::cout << projection_matrix << "\n";

    // std::cout << "cam_instrinsic matrix\n";
    // std::cout << cam_instrinsic << "\n";
    // std::cout << cal["K_cam3"] << "\n";

    // cv::Mat Transform;
    // cv::Mat translation2 = translation.reshape(1).t();
    // cv::hconcat(rotation, translation2, Transform);

    // std::cout << "Transform matrix\n";
    // std::cout << Transform << "\n";
    // std::cout << cal["T_cam3_velo"] << "\n";
}