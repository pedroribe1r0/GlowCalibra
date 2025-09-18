#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace std;

enum RGB{
    Red,
    Blue,
    Green
};

class Imagem
{
private:
    int counter;
public:
    Imagem();
    ~Imagem();
    cv::Mat load_image(string path);
    void saveImage(cv::Mat image);
    void saveImage(cv::Mat image, const std::string name);
    void crop_square(cv::Mat &image);
    void color_treatment(cv::Mat &image);
    void gray_scale(cv::Mat &image);
    void aply_threshold(cv::Mat &image);
    cv::Mat apply_convolution(const cv::Mat& img, const cv::Mat& kernel);
};
