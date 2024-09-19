#include <opencv2/opencv.hpp>
#include <iostream>
#include "Tuple.hpp"
#include "Buble.hpp"
using namespace Tuples;

class Image
{
private:
    cv::Mat matrix;

public:
    Image();
    ~Image();
    void loadImage(std::string path);
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
void Image::grayImage()
{
    int i, j, cols, rows;
    cols = matrix.cols;
    rows = matrix.rows;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            cv::Vec3b pixel = matrix.at<cv::Vec3b>(i, j);
            int red = pixel[2];
            int green = pixel[1];
            int blue = pixel[0];

            int sum = (blue + green + blue) / 3;
            sum = sum > 150 ? 255 : 0;
            pixel[2] = sum;                     // assign sum to red channel
            pixel[1] = sum;                     // assign sum to green channel
            pixel[0] = sum;                     // assign sum to blue channel
            matrix.at<cv::Vec3b>(i, j) = pixel; // update the pixel in the matrix
        }
    }
}
