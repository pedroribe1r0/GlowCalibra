#include "imagem.hpp"
#include "kernel.hpp"

Imagem::Imagem() : counter(0)
{
}

Imagem::~Imagem()
{
}

cv::Mat Imagem::load_image(const string path)
{
    cv::Mat image = cv::imread(path);
    if (image.empty())
    {
        std::cerr << "falha carragar imagem" << std::endl;
        exit(1);
    }
    return image;
}

cv::Mat Imagem::apply_convolution(const cv::Mat &img, const cv::Mat &kernel)
{
    cv::Mat output;
    // Aplica a convolução utilizando o kernel fornecido
    cv::filter2D(img, output, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
    return output;
}

void Imagem::saveImage(cv::Mat image)
{
    std::string path = "Database/RefinedData/refinedImage" + std::to_string(counter++) + ".jpeg";
    cv::imwrite(path, image);
}
void Imagem::saveImage(cv::Mat image, const std::string name)
{
    std::string path = "Database/RefinedData/" + name + ".jpeg";
    cv::imwrite(path, image);
}

void Imagem::crop_square(cv::Mat &image)
{
    if (image.empty())
    {
        std::cerr << "Imagem invalida!" << std::endl;
        return;
    }

    int height = image.rows;
    int width = image.cols;

    // tam do quadrado
    int square_size = std::min(height, width) / 2;

    // inicio do quadrado
    int x_start = (width - square_size) / 2;
    int y_start = (height - square_size) / 2;

    // criar retangulo
    cv::Rect rect(x_start, y_start, square_size, square_size);

    // substitui
    image = image(rect);
}

void Imagem::color_treatment(cv::Mat &image)
{ // receba passagem por ref
    if (image.empty() || image.channels() != 3)
    {
        std::cerr << "Imagem invalida!" << std::endl;
        exit(1);
    }

    std::vector<cv::Mat> color_channels; // cria um vetor de matrizes de img, R - G - B

    cv::split(image, color_channels);

    // color_channels[1].setTo(0);
    // color_channels[0] = color_channels[0].mul(color_channels[2] / 255.0);
    // color_channels[2].setTo(0);
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    image = color_channels[0] = color_channels[0].mul(color_channels[2] / 255.0);
}

void Imagem::gray_scale(cv::Mat &image)
{
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
}

void Imagem::aply_threshold(cv::Mat &image)
{
    cv::threshold(image, image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}

void Imagem::clear_trash(cv::Mat &image)
{
    cv::Mat temp;
    cv::GaussianBlur(image, temp, cv::Size(5, 5), 0);
    image = temp;
}
