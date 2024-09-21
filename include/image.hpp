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
    void cutSquareInMiddle();
    void gaussianBlur();
    void threshould();

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
    cv::adaptiveThreshold(matrix, thresholded, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, -15);
    matrix = thresholded;
}

void Image::threshould()
{
    cv::threshold(matrix, matrix, 45, 255, cv::THRESH_TOZERO);
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
        if (area > 10)
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
    cv::convertScaleAbs(matrix, aux, 5.0, 0); // Aumentar contraste
    matrix = aux;
}