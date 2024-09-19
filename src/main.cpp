#include <opencv2/opencv.hpp>
#include <iostream>
#include "image.hpp"

int main()
{
    Image imagem;
    std::string path = "Database/SourceData/testerect.jpeg";
    imagem.loadImage(path);
    imagem.grayImage();
    imagem.getArea();
    imagem.saveImage();
    return 0;
}