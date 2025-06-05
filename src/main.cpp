#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "./imagem.hpp"
#include "./kernel.hpp"
#include <cmath>
#include <numeric>
#include <fstream>

void encontrarCentro(const cv::Mat &binMask,
                     cv::Point2f &outCenter,
                     float &outRadius,
                     float margin = 20.0f)
{
    CV_Assert(binMask.type() == CV_8UC1);

    int W = binMask.cols;
    int H = binMask.rows;

    cv::Point2f imgCenter(W / 2.0f, H / 2.0f);
    std::vector<cv::Point2f> startPoints = {
        {W / 2.0f, 0},               // topo
        {W / 2.0f, float(H - 1)},    // base
        {0, H / 2.0f},               // esquerda
        {float(W - 1), H / 2.0f},    // direita
        {0, 0},                      // canto sup. esquerdo
        {float(W - 1), 0},           // canto sup. direito
        {0, float(H - 1)},           // canto inf. esquerdo
        {float(W - 1), float(H - 1)} // canto inf. direito
    };

    std::vector<cv::Point2f> hitPoints;

    for (const auto &start : startPoints)
    {
        cv::Point2f dir = imgCenter - start;
        float norm = std::hypot(dir.x, dir.y);
        if (norm == 0)
            continue;

        dir /= norm;

        for (float t = 0; t <= norm; t += 1.0f)
        {
            int xi = cvRound(start.x + dir.x * t);
            int yi = cvRound(start.y + dir.y * t);

            if (xi < 0 || xi >= W || yi < 0 || yi >= H)
                break;

            if (binMask.at<uchar>(yi, xi) == 255)
            {
                hitPoints.emplace_back(xi, yi);
                break;
            }
            if (cv::norm(cv::Point2f(xi, yi) - imgCenter) < 1.0f)
            {
                hitPoints.emplace_back(xi, yi);
                break;
            }
        }
    }

    if (hitPoints.size() < 3)
    {
        outCenter = imgCenter;
        outRadius = 0;
        return;
    }
    cv::Point2f center(0, 0);
    for (auto &pt : hitPoints)
        center += pt;
    center *= (1.0f / hitPoints.size());

    std::vector<float> radii;
    for (auto &pt : hitPoints)
        radii.push_back(cv::norm(pt - center));
    float mean = std::accumulate(radii.begin(), radii.end(), 0.0f) / radii.size();

    std::vector<float> filtered;
    for (float r : radii)
        if (std::abs(r - mean) <= margin)
            filtered.push_back(r);

    float finalRadius = 0;
    if (!filtered.empty())
        finalRadius = std::accumulate(filtered.begin(), filtered.end(), 0.0f) / filtered.size();
    else
        finalRadius = mean;

    outCenter = center;
    outRadius = finalRadius;
}

void contarGotasEExportarCSV(const cv::Mat &imagemBinaria, const std::string &nomeCSV, cv::Mat &mascaraRotulos, cv::Mat &visualizacao)
{
    using namespace cv;
    using namespace std;

    if (imagemBinaria.empty() || imagemBinaria.type() != CV_8UC1)
    {
        cerr << "Imagem inválida. Ela deve ser binária (CV_8UC1)!" << endl;
        return;
    }

    // Encontra contornos
    vector<vector<Point>> contornos;
    findContours(imagemBinaria, contornos, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Cria máscara de rótulos (mesmo tamanho, pixels 32-bit)
    mascaraRotulos = Mat::zeros(imagemBinaria.size(), CV_32SC1);
    visualizacao = Mat::zeros(imagemBinaria.size(), CV_8UC3);

    // Abre arquivo CSV
    ofstream outFile(nomeCSV);
    if (!outFile.is_open())
    {
        cerr << "Erro ao abrir " << nomeCSV << endl;
        return;
    }
    outFile << "Index,X,Y,Area\n";

    RNG rng(12345); // para cores aleatórias na visualização
    int index = 1;

    for (const auto &contorno : contornos)
    {
        double area = contourArea(contorno);
        if (area < 1)
            continue; // ignora pequenos ruídos

        Moments M = moments(contorno);
        if (M.m00 == 0)
            continue;

        int cx = static_cast<int>(M.m10 / M.m00);
        int cy = static_cast<int>(M.m01 / M.m00);

        // Escreve no CSV
        outFile << index << "," << cx << "," << cy << "," << area << "\n";

        // Desenha o número na visualização
        drawContours(visualizacao, vector<vector<Point>>{contorno}, -1, Scalar(255, 255, 255), 1);
        putText(visualizacao, to_string(index), Point(cx - 5, cy), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);

        // Preenche a máscara com o valor do índice
        drawContours(mascaraRotulos, vector<vector<Point>>{contorno}, -1, Scalar(index), FILLED);

        index++;
    }

    outFile.close();
    cout << "Total de gotas rotuladas: " << index - 1 << endl;
}

int main()
{
    Imagem img;
    cv::Mat matrix = img.load_image("Database/SourceData/imagem_simulada_gotas_azul_destaque.png");
    img.saveImage(matrix, "original");

    // --- Pré‑processamento de cor e binarização ---
    std::vector<cv::Mat>
        color_channels;
    cv::split(matrix, color_channels);
    color_channels[0].setTo(0);
    color_channels[1].setTo(0);
    cv::Mat noBlue;
    cv::merge(color_channels, noBlue);
    cv::Mat gray;
    cv::cvtColor(noBlue, gray, cv::COLOR_BGR2GRAY);
    img.saveImage(gray, "gray scale");

    cv::Mat blur;
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 2);

    cv::Mat laplacian;
    cv::Laplacian(blur, laplacian, CV_16S, 5);
    cv::convertScaleAbs(laplacian, laplacian);
    img.saveImage(laplacian, "laplacian filter");

    cv::threshold(blur, laplacian, 1, 255, cv::THRESH_BINARY);
    cv::GaussianBlur(laplacian, blur, cv::Size(9, 9), 2);
    cv::Mat abs;
    cv::bitwise_not(laplacian, abs);
    img.saveImage(abs, "blur (low pass filter)");

    // --- Componentes conectados e filtragem por área ---
    cv::Mat labels, stats, centroids;
    int nLabels = cv::connectedComponentsWithStats(abs, labels, stats, centroids, 8, CV_32S);
    int minArea = 20000;
    cv::Mat filtered = cv::Mat::zeros(abs.size(), CV_8U);
    for (int lbl = 1; lbl < nLabels; ++lbl)
    {
        int area = stats.at<int>(lbl, cv::CC_STAT_AREA);
        if (area >= minArea)
            filtered.setTo(255, labels == lbl);
    }
    img.saveImage(filtered, "find the bigges contour (leaf)");

    cv::Point2f center;
    float radius = 0;
        imagem.gaussianBlur();
    encontrarCentro(filtered, center, radius, 10.0f);
    cv::Mat circMask = cv::Mat::zeros(matrix.size(), CV_8U);
    std::cout << radius << "raio" << std::endl;
    cv::circle(circMask, center, static_cast<int>(std::round(radius)), cv::Scalar(255), cv::FILLED);
    cv::Mat roi;
    bitwise_and(matrix, matrix, roi, circMask);
    img.saveImage(roi, "folha final");
    //--------------------------------------------------------ENCONTRAR GOTAS---------------------------------------------------------------//

    // HSV azul
    //  1. Converter imagem para HSV
    cv::Mat hsv;
    cv::cvtColor(roi, hsv, cv::COLOR_BGR2HSV);

    // 2. Criar máscara para faixa azul-roxo (gotas fluorescentes)
    cv::Mat maskHSV;
    cv::inRange(hsv, cv::Scalar(90, 50, 50), cv::Scalar(160, 255, 255), maskHSV);

    // 3. Aplicar a máscara sobre a imagem original (ou cinza)
    cv::Mat fluorescencia;
    bitwise_and(roi, roi, fluorescencia, maskHSV);
    img.saveImage(fluorescencia, "hsv");

    img.gray_scale(fluorescencia);

    cv::Mat thresh_final;
    cv::threshold(fluorescencia, thresh_final, 1, 255, cv::THRESH_OTSU);
    img.saveImage(thresh_final, "thresh_final");

    // operação de closing
    cv::dilate(thresh_final, thresh_final, cv::Mat(), cv::Point(-1, -1), 2);
    cv::erode(thresh_final, thresh_final, cv::Mat(), cv::Point(-1, -1), 2);
    img.saveImage(thresh_final, "morphologi");
    cv::Mat mascara, visual;
    contarGotasEExportarCSV(thresh_final, "resultado.csv", mascara, visual);
    img.saveImage(visual, "visual");
    img.saveImage(mascara, "mascara");

    return 0;
}
