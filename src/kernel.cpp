#include "kernel.hpp"
#include <stdexcept>
#include <cstring>

cv::Mat Kernel::get_preset_kernel(KernelType kernelType) {
    switch (kernelType) {

        case KernelType::EMBOSS_A:
            return create_kernel({  0, 0, 0,
                                    0, 1, 0,
                                    0, 0, -1}, 3, 3, true);
        case KernelType::SHARPEN:
            return create_kernel({ 0, -1,  0,
                                  -1,  5, -1,
                                   0, -1,  0 }, 3, 3, false);

        case KernelType::SOBEL_X:
            return create_kernel({ -1,  0,  1,
                                  -2,  0,  2,
                                  -1,  0,  1 }, 3, 3, false);

        case KernelType::SOBEL_Y:
            return create_kernel({ -1, -2, -1,
                                   0,  0,  0,
                                   1,  2,  1 }, 3, 3, false);

        case KernelType::BOX_3X3:
            return create_kernel({ 1, 1, 1,
                                   1, 1, 1,
                                   1, 1, 1 }, 3, 3, true);

        case KernelType::GAUSSIAN_3X3:
            return create_kernel({ 1, 2, 1,
                                   2, 4, 2,
                                   1, 2, 1 }, 3, 3, true);

        case KernelType::GAUSSIAN_5X5:
            return create_kernel({ 1,  4,  6,  4, 1,
                                   4, 16, 24, 16, 4,
                                   6, 24, 36, 24, 6,
                                   4, 16, 24, 16, 4,
                                   1,  4,  6,  4, 1 }, 5, 5, true);

        case KernelType::LAPLACE_3X3_A:
            return create_kernel({ 0,  1,  0,
                                   1, -4,  1,
                                   0,  1,  0 }, 3, 3, false);

        case KernelType::LAPLACE_3X3_B:
            return create_kernel({ 1,  1,  1,
                                   1, -8,  1,
                                   1,  1,  1 }, 3, 3, false);

        case KernelType::PREWITT_HORIZONTAL:
            return create_kernel({ -1,  0,  1,
                                   -1,  0,  1,
                                   -1,  0,  1 }, 3, 3, false);

        case KernelType::PREWITT_VERTICAL:
            return create_kernel({ -1, -1, -1,
                                    0,  0,  0,
                                    1,  1,  1 }, 3, 3, false);

        case KernelType::EMBOSS_B:
            return create_kernel({ -2, -1,  0,
                                   -1,  1,  1,
                                    0,  1,  2 }, 3, 3, false);

        case KernelType::EDGE_45:
            return create_kernel({ -1, -1,  2,
                                   -1,  2, -1,
                                    2, -1, -1 }, 3, 3, false);

        case KernelType::HIGH_PASS:
            return create_kernel({ -1, -1, -1,
                                   -1,  8, -1,
                                   -1, -1, -1 }, 3, 3, false);

        case KernelType::ROBERTS_CROSS_X:
            return create_kernel({ 1, 0,
                                   0, -1 }, 2, 2, false);

        case KernelType::ROBERTS_CROSS_Y:
            return create_kernel({ 0,  1,
                                  -1,  0 }, 2, 2, false);

        case KernelType::IDENTITY:
            return create_kernel({ 0, 0, 0,
                                   0, 1, 0,
                                   0, 0, 0 }, 3, 3, false);
        case KernelType::W_AVG_3x3:
                return create_kernel({  1,2,1,
                                        2,4,2,
                                        1,2,1 }, 3, 3,true);
        
        case KernelType::GAUSSIAN_160_05:
        /*kernel gaussiano com sigma == 0.5*/
                    return create_kernel({ 0.0113, 0.0838, 0.0113,
                                            0.0838, 0.6193, 0.0838,
                                            0.0113, 0.0838, 0.0113}, 3, 3, true);
        case KernelType::GAUSSIAN_160_20:
        /*kernel gaussiano com sigma == 20*/
                    return create_kernel({  0.0392, 0.0393, 0.0393, 0.0393, 0.0392,
                                            0.0393, 0.0394, 0.0394, 0.0394, 0.0393,
                                            0.0393, 0.0394, 0.0395, 0.0394, 0.0393,
                                            0.0393, 0.0394, 0.0394, 0.0394, 0.0393,
                                            0.0392, 0.0393, 0.0393, 0.0393, 0.0392 }, 5, 5, true);
        case KernelType::LOG:
                    return create_kernel({  0,   0, -1,  0,  0,
                                            0, -1, -2, -1,  0,
                                            -1, -2, 16, -2, -1,
                                            0, -1, -2, -1,  0,
                                            0,  0, -1,  0,  0},5, 5,false);


        default:
            throw std::invalid_argument("kernel inválido!");
    }
}

cv::Mat Kernel::create_kernel(const std::vector<float>& data, int rows, int cols, bool normalize_flag) {
    cv::Mat kernel(rows, cols, CV_32F);
    std::memcpy(kernel.data, data.data(), data.size() * sizeof(float));
    if (normalize_flag)
        normalize(kernel);
    return kernel;
}

void Kernel::normalize(cv::Mat& kernel) {
    double sum = cv::sum(kernel)[0];
    if (sum != 0)
        kernel /= sum;
}
