#include "imagem.hpp"

Imagem::Imagem() : 
    counter(0)
{
}

Imagem::~Imagem()
{
}

cv::Mat Imagem::load_image(const string path){
    cv::Mat image = cv::imread(path);
    if (image.empty()) {
        std::cerr << "falha carragar imagem" << std::endl;
        exit(1);
    }
    return image;
}

void Imagem::saveImage(cv::Mat image)
{
    std::string path = "Database/RefinedData/refinedImage" + std::to_string(counter++) + ".jpeg";
    cv::imwrite(path, image);
}


void Imagem::crop_square(cv::Mat &image) {
    if (image.empty()) {
        std::cerr << "Imagem invalida!" << std::endl;
        return;
    }

    int height = image.rows;
    int width = image.cols;
    
    //tam do quadrado 
    int square_size = std::min(height, width) / 2;

    //inicio do quadrado 
    int x_start = (width - square_size) / 2;
    int y_start = (height - square_size) / 2;

    //criar retangulo
    cv::Rect rect(x_start, y_start, square_size, square_size);

    //substitui
    image = image(rect);
}


void Imagem::color_treatment(cv::Mat &image){ //receba passagem por ref
    if (image.empty() || image.channels() != 3) {
        std::cerr << "Imagem invalida!" << std::endl;
        exit(1);
    }

    std::vector<cv::Mat> color_channels; // cria um vetor de matrizes de img, R - G - B

    cv::split(image, color_channels);

    color_channels[RGB::Red].setTo(0); //vermelho morto
    color_channels[RGB::Green] = color_channels[RGB::Green].mul(color_channels[RGB::Blue] / 255.0); //verde depende do azul
    cv::merge(color_channels, image);
}

void Imagem::gray_scale(cv::Mat &image){
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
}

void Imagem::aply_threshold(cv::Mat &image){
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    /*cv::adaptiveThreshold(image, image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
        cv::THRESH_BINARY, 11, 2); */
}