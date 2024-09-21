#include <opencv2/opencv.hpp>
#include <iostream>
#include "image.hpp"

int main()
{
    Image imagem;

    std::string path = "Database/SourceData/IMG_3536.jpg";
    imagem.loadImage(path);
    imagem.cutSquareInMiddle();
    imagem.saveImage();

    imagem.grayImage();
    imagem.saveImage();

    imagem.equalizeImage();
    imagem.saveImage();

    imagem.gaussianBlur();
    imagem.saveImage();

    imagem.threshould();
    imagem.saveImage();

    imagem.polarizeImage();
    imagem.saveImage();

    imagem.getArea();
    imagem.saveImage();

    imagem.circulize();
    imagem.saveImage();

    imagem.cleanNonCircularThings();
    imagem.saveImage();

    imagem.getArea();
    imagem.saveImage();

    return 0;
}