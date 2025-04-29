
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "./imagem.hpp"
#define CIRCULARITY_MIN 0.4f
#define CIRCULARITY_MAX 0.6f
#define BUBBLE_AREA_CUT 40

int main()
{
    Imagem img;
    cv::Mat matrix = img.load_image("Database/SourceData/IMG_new.jpeg");
    img.crop_square(matrix);
    img.color_treatment(matrix);
    img.saveImage(matrix);
    img.gray_scale(matrix);
    img.aply_threshold(matrix);
    img.cleanNonCircularThings(INT_MAX, CIRCULARITY_MIN, matrix);
    img.saveImage(matrix);

    return 0;
}

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