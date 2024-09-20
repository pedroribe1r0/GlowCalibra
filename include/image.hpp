#include <opencv2/opencv.hpp>
#include <iostream>
#include "Tuple.hpp"
#include "Buble.hpp"

using namespace Tuples;

class Image
{
private:
    cv::Mat matrix;
    static int counter;
    bool polarized;
    bool equalized;

public:
    Image();
    ~Image();
    void loadImage(std::string path);
    void saveImage();
    void grayImage();
    void getArea();
    void equalizeImage();
    void polarizeImage();
};
int Image::counter = 0;

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
    counter++;
    std::string path = "Database/RefinedData/refinedImage" + std::to_string(counter) + ".jpeg";
    cv::imwrite(path, matrix);
}

void Image::grayImage()
{
    int cols = matrix.cols;
    int rows = matrix.rows;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cv::Vec3b pixel = matrix.at<cv::Vec3b>(i, j);
            int red = pixel[2];
            int green = pixel[1];
            int blue = pixel[0];

            int gray = (red + green + blue) / 3;
            matrix.at<cv::Vec3b>(i, j) = cv::Vec3b(gray, gray, gray);
        }
    }
    cv::cvtColor(matrix, matrix, cv::COLOR_BGR2GRAY);
}

void Image::polarizeImage()
{
    cv::adaptiveThreshold(matrix, matrix, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, -30);
}
void Image::getArea()
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    // Encontrar contornos na imagem binária
    cv::findContours(matrix, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Converter a imagem para colorida para desenhar contornos
    cv::Mat colorImage;
    cv::cvtColor(matrix, colorImage, cv::COLOR_GRAY2BGR);

    // Iterar sobre os contornos detectados
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        // Desenhar os contornos em vermelho (com espessura maior para visualização melhor)
        cv::drawContours(colorImage, contours, static_cast<int>(i), cv::Scalar(0, 0, 255), 3); // Aumentar espessura para 3
    }

    // Exibir a imagem com contornos
    cv::imshow("Contornos Vermelhos", colorImage);
    // Salvar a imagem com contornos
    cv::imwrite("saida_com_contornos.jpg", colorImage);

    std::ofstream outputFile("areaspaint.txt");

    // Iterate over the contours
    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calculate the area of the contour
        double area = cv::contourArea(contours[i]);

        // Check if the area is greater than 1 pixel
        if (area > 1)
        {
            // Write the area to the output file
            outputFile << "Área da gota " << i + 1 << ": " << area << " pixels" << std::endl;
        }
    }

    // Close the output file
    outputFile.close();
}

void Image::equalizeImage()
{
    if (matrix.channels() == 3)
    {
        cv::cvtColor(matrix, matrix, cv::COLOR_BGR2GRAY);
    }

    cv::equalizeHist(matrix, matrix);
}