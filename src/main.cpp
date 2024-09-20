#include <opencv2/opencv.hpp>
#include <iostream>
#include "image.hpp"

int main()
{
    Image imagem;

    std::string path = "Database/SourceData/WhatsApp Image 2024-09-04 at 16.13.35.jpeg";
    imagem.loadImage(path);

    imagem.grayImage();
    imagem.saveImage();

    imagem.polarizeImage();
    imagem.saveImage();

    imagem.getArea();
    imagem.saveImage();

    return 0;
}