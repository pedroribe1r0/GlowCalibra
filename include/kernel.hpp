#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <opencv2/opencv.hpp>
#include <stdexcept>

enum class KernelType {
    SHARPEN,
    SOBEL_X,
    SOBEL_Y,
    BOX_3X3,
    GAUSSIAN_3X3,
    GAUSSIAN_5X5,
    LAPLACE_3X3_A,
    LAPLACE_3X3_B,
    PREWITT_HORIZONTAL,
    PREWITT_VERTICAL,
    EMBOSS_A,
    EMBOSS_B,
    EDGE_45,
    HIGH_PASS,
    ROBERTS_CROSS_X,
    ROBERTS_CROSS_Y,
    MEDIAN_3X3,
    IDENTITY,
    W_AVG_3x3,
    GAUSSIAN_160_05,
    GAUSSIAN_160_20,
    LOG
};

class Kernel {
    public:
        static cv::Mat get_preset_kernel(KernelType kernel);

        static cv::Mat create_kernel(const std::vector<float>& data, int rows, int cols, bool normalize_lag);

        static void normalize(cv::Mat& kernel);

    };
#endif