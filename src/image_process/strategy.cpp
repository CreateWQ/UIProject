#include "strategy.hpp"


void Grayscale::processImpl(const cv::Mat& input, cv::Mat& output, const std::vector<float>& params) {

    cv::cvtColor(input, output, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(output, output, cv::COLOR_GRAY2RGBA);

}

void Blur::processImpl(const cv::Mat& input, cv::Mat& output, const std::vector<float>& params) {
    int kernel_size = static_cast<int>(params[0]);
    if (kernel_size % 2 == 0) ++kernel_size;
    cv::GaussianBlur(input, output, cv::Size(kernel_size, kernel_size), 0);
}

void Canny::processImpl(const cv::Mat& input, cv::Mat& output, const std::vector<float>& params) {
    cv::Mat gray, edges;
    cv::cvtColor(input, gray, cv::COLOR_RGBA2GRAY);
    cv::Canny(gray, edges, params[0], params[1]);
    cv::cvtColor(edges, output, cv::COLOR_GRAY2RGBA);
}