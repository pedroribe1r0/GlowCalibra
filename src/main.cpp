#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // Carregar a imagem binária (gotas brancas, fundo preto)
    cv::Mat image = cv::imread("database/SourceData/AHHAAHAH.jpeg", cv::IMREAD_GRAYSCALE);
    if (image.empty())
    {
        std::cout << "Erro ao carregar a imagem!" << std::endl;
        return -1;
    }

    // Vetor para armazenar os contornos
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    // Encontrar contornos na imagem
    cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Converter a imagem para colorida para desenhar contornos coloridos
    cv::Mat image_color;
    cv::cvtColor(image, image_color, cv::COLOR_GRAY2BGR);

    // Iterar sobre os contornos detectados
    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calcular a área do contorno
        double area = cv::contourArea(contours[i]);
        std::cout << "Área da gota " << i + 1 << ": " << area << " pixels" << std::endl;

        // Desenhar os contornos em vermelho
        cv::drawContours(image_color, contours, static_cast<int>(i), cv::Scalar(0, 0, 255), 2);
    }

    // Exibir a imagem com os contornos desenhados
    cv::imshow("Contornos Vermelhos", image_color);
    cv::waitKey(0);

    return 0;
}