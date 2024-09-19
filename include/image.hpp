#include <opencv2/opencv.hpp>
#include <iostream>
#include "Tuple.hpp"
#include "Buble.hpp"
using namespace Tuples;

class Image
{
private:
    cv::Mat matrix;
    TupleI resolution;

public:
    Image();
    ~Image();
    void loadImage(std::string path);
    void printImage();
    void saveImage();
    void grayImage();
};

Image::Image(/* args */)
{
}

Image::~Image()
{
}
void Image::loadImage(std::string path)
{
    matrix = cv::imread(path, cv::IMREAD_COLOR);
}

void Image::saveImage()
{
    cv::imwrite("saida.jpg", matrix);
}
