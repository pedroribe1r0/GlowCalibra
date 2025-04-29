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

void Imagem::cleanNonCircularThings(int size, float minumum, cv::Mat &img)
{
    // Encontrar contornos
    std::vector<std::vector<cv::Point>> contornos;
    cv::findContours(img, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Criar uma máscara para manter apenas os contornos suficientemente circulares
    cv::Mat img_filt = cv::Mat::zeros(img.size(), CV_8UC1);

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
    img = img_filt;
}

void Imagem::removeDropsBySize(double limit, cv::Mat& matrix, bool lower, std::vector<std::vector<cv::Point>> contours){
    cv::Mat img_filt = cv::Mat::zeros(matrix.size(), CV_8UC1);

    for (size_t i = 0; i < contours.size(); i++)
    {
        // Calcular a área e o perímetro (comprimento do contorno)
        double area = cv::contourArea(contours[i]);

        if(lower){
            if(area < limit)
                cv::drawContours(img_filt, contours, (int)i, cv::Scalar(255), cv::FILLED);
        }
        else{
            if(area > limit)
                cv::drawContours(img_filt, contours, (int)i, cv::Scalar(255), cv::FILLED);
        }        
    }

    matrix = img_filt;
}