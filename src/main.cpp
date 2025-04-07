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

    cv::Mat img = cv::imread("Database/SourceData/teste PDI.png", cv::IMREAD_GRAYSCALE);
    cursor.saveImage(img);
    Kernel filtro;
    cv::Mat kernel;

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