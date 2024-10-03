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
    bool saved;

public:
    Image();
    ~Image();
    void loadImage(std::string path);
    void saveImage();
    void grayImage();
    void getArea();
    void equalizeImage();
    void polarizeImage();
    void cutSquareInMiddle();
    void gaussianBlur();
    void threshould();
    void cleanNonCircularThings(int size, float minimum);
    void circulize();
};
int Image::counter = 0;

Image::Image(/* args */)
{
}

Image::~Image()
{
}

void Image::circulize()
{
    // Aplicar a operação de morfologia closing (dilatação seguida de erosão)
    cv::Mat img_morph;
    int morph_size = 5; // Tamanho do kernel para as operações morfológicas
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                cv::Size(2 * morph_size + 1, 2 * morph_size + 1),
                                                cv::Point(morph_size, morph_size));

    // Closing: dilatação seguida de erosão
    cv::morphologyEx(matrix, img_morph, cv::MORPH_CLOSE, element);

    // Aplicar preenchimento de buracos (Flood fill)
    cv::Mat img_flood = img_morph.clone();
    cv::floodFill(img_flood, cv::Point(0, 0), cv::Scalar(255)); // Preenche o fundo

    // Inverter a imagem preenchida
    cv::bitwise_not(img_flood, img_flood);

    // Combinar a imagem original com a imagem preenchida
    matrix = img_morph | img_flood;
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

            int gray = (blue + green + blue + green) / 4;
            matrix.at<cv::Vec3b>(i, j) = cv::Vec3b(gray, gray, gray);
        }
    }
    cv::cvtColor(matrix, matrix, cv::COLOR_BGR2GRAY);
}

void Image::polarizeImage()
{
    // cv::threshold(matrix, matrix, 235, 255, cv::THRESH_BINARY);
    cv::Mat thresholded;
    cv::adaptiveThreshold(matrix, thresholded, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, -16);
    matrix = thresholded;
}

void Image::threshould()
{
    cv::threshold(matrix, matrix, 30, 255, cv::THRESH_TOZERO);
}
void Image::getArea()
{
    // Encontrar contornos
    std::vector<std::vector<cv::Point>> contornos;
    cv::findContours(matrix, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Criar uma máscara para manter apenas os contornos maiores
    cv::Mat img_filt = cv::Mat::zeros(matrix.size(), CV_8UC1);

    // Definir a área mínima para manter os contornos
    double area_minima = 10.0; // Defina o valor mínimo de área para eliminar bolhas pequenas

    // Percorrer todos os contornos encontrados
    for (size_t i = 0; i < contornos.size(); i++)
    {
        double area = cv::contourArea(contornos[i]);

        // Manter apenas os contornos cuja área seja maior que o mínimo
        if (area > area_minima)
        {
            cv::drawContours(img_filt, contornos, (int)i, cv::Scalar(255), cv::FILLED);
        }
    }
    matrix = img_filt;

    // Gerar arquivo txt com contagem e área das bolhas
    std::string txtPath = "Database/RefinedData/imageInfo" + std::to_string(counter) + ".txt";
    std::ofstream txtFile(txtPath);
    if (txtFile.is_open())
    {
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(matrix, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        txtFile << "Count: " << contours.size() << std::endl;
        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            txtFile << "Bubble " << i + 1 << " - Area: " << area << std::endl;
        }
        txtFile.close();
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }
}
void Image::equalizeImage()
{
    cv::Mat aux;
    cv::bilateralFilter(matrix, aux, 21, 25, 50);
    matrix = aux;
}

void Image::cutSquareInMiddle()
{
    int cols = matrix.cols;
    int rows = matrix.rows;

    int x = (cols) / 2;
    int y = (rows) / 2;
    int width = 600;  // Largura do retângulo
    int height = 600; // Altura do retângulo
    cv::Rect roi(x, y, width, height);
    matrix = matrix(roi);
}

void Image::gaussianBlur()
{
    // Aplicar suavização Gaussiana com dois diferentes sigmas
    cv::Mat gauss1, gauss2;
    double sigma1 = 1.0; // Suavização leve
    double sigma2 = 6.0; // Suavização mais forte

    cv::GaussianBlur(matrix, gauss1, cv::Size(0, 0), sigma1); // Gaussiano mais suave
    cv::GaussianBlur(matrix, gauss2, cv::Size(0, 0), sigma2); // Gaussiano mais forte

    // Subtrair as duas imagens (Diferença de Gaussianas)
    cv::subtract(gauss1, gauss2, matrix);

    // Normalizar a imagem resultante para a faixa de 0 a 255
    cv::normalize(matrix, matrix, 0, 255, cv::NORM_MINMAX);

    // Aplicar um ajuste de contraste (opcional)
    cv::Mat aux;
    cv::convertScaleAbs(matrix, aux, 4.0, 0); // Aumentar contraste
    matrix = aux;
}

void Image::cleanNonCircularThings(int size, float minumum)
{
    // Encontrar contornos
    std::vector<std::vector<cv::Point>> contornos;
    cv::findContours(matrix, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Criar uma máscara para manter apenas os contornos suficientemente circulares
    cv::Mat img_filt = cv::Mat::zeros(matrix.size(), CV_8UC1);

    // Definir a circularidade mínima
    double circularidade_minima = minumum; // Ajuste esse valor para o quão "redondo" deseja que seja

    // Percorrer todos os contornos encontrados
    for (size_t i = 0; i < contornos.size(); i++)
    {
        // Calcular a área e o perímetro (comprimento do contorno)
        double area = cv::contourArea(contornos[i]);
        double perimetro = cv::arcLength(contornos[i], true);

        if (area < size)
        {
            // Calcular a circularidade
            double circularidade = 4 * CV_PI * area / (perimetro * perimetro);

            // Manter contornos que são suficientemente circulares
            if (circularidade > circularidade_minima)
            {
                cv::drawContours(img_filt, contornos, (int)i, cv::Scalar(255), cv::FILLED);
            }
        }
        else
        {
            cv::drawContours(img_filt, contornos, (int)i, cv::Scalar(255), cv::FILLED);
        }
    }
    matrix = img_filt;
}