#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "./imagem.hpp"
#include "./kernel.hpp"
#define CIRCULARITY_MIN 0.4f
#define CIRCULARITY_MAX 0.6f
#define BUBBLE_AREA_CUT 40



int main() {   
    Imagem cursor;

    cv::Mat img = cursor.load_image("Database/SourceData/IMG_3529.jpg");
    cursor.saveImage(img);
    cursor.crop_square(img);
    cursor.color_treatment(img);
    cursor.gray_scale(img);

    cv::Mat gradiente_x, gradiente_y, sobel, prewitt, roberts, laplaciano;

    //Sobel
    cv::Sobel(img, gradiente_x, CV_64F, 1, 0, 3);
    cv::Sobel(img, gradiente_y, CV_64F, 0, 1, 3);
    cv::magnitude(gradiente_x, gradiente_y, sobel);
    cv::normalize(sobel, sobel, 0, 255, cv::NORM_MINMAX, CV_8U);
    cursor.aply_threshold(sobel);
    cursor.saveImage(sobel, "sobel");

    // prewitt
    cv::Mat kernel_prewitt_x = (cv::Mat_<float>(3, 3) <<
        -1, 0, 1,
        -1, 0, 1,
        -1, 0, 1);
    cv::Mat kernel_prewitt_y = (cv::Mat_<float>(3, 3) <<
        -1, -1, -1,
         0,  0,  0,
         1,  1,  1);
    cv::filter2D(img, gradiente_x, CV_32F, kernel_prewitt_x);
    cv::filter2D(img, gradiente_y, CV_32F, kernel_prewitt_y);
    cv::magnitude(gradiente_x, gradiente_y, prewitt);
    cv::normalize(prewitt, prewitt, 0, 255, cv::NORM_MINMAX, CV_8U);
    cursor.aply_threshold(prewitt);
    cursor.saveImage(prewitt, "prewitt");

    //roberts
    cv::Mat kernel_roberts_x = (cv::Mat_<float>(2, 2) <<
        1,  0,
        0, -1);
    cv::Mat kernel_roberts_y = (cv::Mat_<float>(2, 2) <<
        0,  1,
        -1,  0);

    cv::filter2D(img, gradiente_x, CV_32F, kernel_roberts_x);
    cv::filter2D(img, gradiente_y, CV_32F, kernel_roberts_y);
    cv::magnitude(gradiente_x, gradiente_y, roberts);

    cv::normalize(roberts, roberts, 0, 255, cv::NORM_MINMAX, CV_8U);
    cursor.aply_threshold(roberts);

    cursor.saveImage(roberts, "roberts");

    //laplaciano

    cv::Laplacian(img, laplaciano, CV_64F);
    cv::convertScaleAbs(laplaciano, laplaciano);
    cursor.aply_threshold(laplaciano);

    cursor.saveImage(laplaciano, "laplaciano");



    return 0;
}

/*    std::vector<std::pair<KernelType, std::string>> filtros = {
        {KernelType::SHARPEN,          "sharpen"},
        {KernelType::SOBEL_X,          "sobel_x"},
        {KernelType::SOBEL_Y,          "sobel_y"},
        {KernelType::BOX_3X3,            "media"},
        {KernelType::GAUSSIAN_3X3,     "gaussian_3x3"},
        {KernelType::GAUSSIAN_5X5,     "gaussian_5x5"},
        {KernelType::LAPLACE_3X3_A,    "laplace_3x3_a"},
        {KernelType::LAPLACE_3X3_B,    "laplace_3x3_b"},
        {KernelType::PREWITT_HORIZONTAL, "prewitt_horizontal"},
        {KernelType::PREWITT_VERTICAL,   "prewitt_vertical"},
        {KernelType::EMBOSS_A,           "emboss_A"},
        {KernelType::EMBOSS_B,           "emboss_B"},
        {KernelType::EDGE_45,          "edge_45"},
        {KernelType::HIGH_PASS,        "high_pass"},
        {KernelType::ROBERTS_CROSS_X,  "roberts_cross_x"},
        {KernelType::ROBERTS_CROSS_Y,  "roberts_cross_y"},
        {KernelType::IDENTITY,         "identity"},
        {KernelType::W_AVG_3x3,         "weighted_AVG"},
        {KernelType::GAUSSIAN_160_05,         "gaussian_05"},
        {KernelType::GAUSSIAN_160_20,         "gaussian_20"},
        {KernelType::LOG,                       "LoG"}
    };

    for (const auto& filtroPair : filtros) {
        cv::Mat output;
        cv::Mat kernel = filtro.get_preset_kernel(filtroPair.first);
        output = cursor.apply_convolution(img, kernel);
        cursor.saveImage(output, filtroPair.second);
    }

    return 0;
    
    
    
    
    */
/*    Imagem img;
cv::Mat matrix = img.load_image("Database/SourceData/IMG_3525.jpg");
img.crop_square(matrix);
img.color_treatment(matrix);
img.saveImage(matrix);
img.gray_scale(matrix);
img.aply_threshold(matrix);
img.saveImage(matrix);
*/


/*    Image imagem;

    std::string directoryPath = "Database/SourceData";

    // Iterate over all files in the directory
    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        std::string path = entry.path().string();
        
        // Perform image processing steps
        imagem.loadImage(path);
        imagem.findScale(106);
        imagem.cutSquareInMiddle();
        //imagem.saveImage();

        imagem.grayImage();
        imagem.equalizeImage();
        //imagem.saveImage();

        imagem.gaussianBlur();
        //imagem.saveImage();

        imagem.threshould();
        imagem.polarizeImage();
        //imagem.saveImage();

        imagem.getArea();
        imagem.saveImage();
        imagem.circulize();
        imagem.cleanNonCircularThings(INT_MAX, CIRCULARITY_MIN);
        //imagem.saveImage();
        

        float distribuction = imagem.calculateDistribution();
        cout << distribuction << endl;

        //imagem.saveImage();
    }*/