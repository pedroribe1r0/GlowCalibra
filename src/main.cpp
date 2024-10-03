
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "image.hpp"
#define CIRCULARITY_MIN 0.4f
#define CIRCULARITY_MAX 0.6f
#define BUBBLE_AREA_CUT 40

int main()
{
    Image imagem;

    std::string directoryPath = "Database/SourceData";

    // Iterate over all files in the directory
    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        std::string path = entry.path().string();

        // Load image
        cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);
        if (image.empty())
        {
            std::cout << "Failed to load image: " << path << std::endl;
            continue;
        }

        // Perform image processing steps
        imagem.loadImage(path);
        imagem.cutSquareInMiddle();
        imagem.saveImage();

        imagem.grayImage();
        imagem.equalizeImage();

        imagem.gaussianBlur();

        imagem.threshould();
        imagem.polarizeImage();

        imagem.getArea();
        imagem.circulize();
        imagem.cleanNonCircularThings(INT_MAX, CIRCULARITY_MIN);
        imagem.cleanNonCircularThings(BUBBLE_AREA_CUT, CIRCULARITY_MAX);
        imagem.getArea();

        imagem.saveImage();
    }

    return 0;
}
