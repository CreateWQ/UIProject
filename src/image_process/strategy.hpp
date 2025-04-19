#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <vector>

class ImageProcessor;

template <typename Derived>
class ProcessingStrategy {
public:
    inline void process(const cv::Mat& input, cv::Mat& output, const std::vector<float>& params) {
        static_cast<Derived*>(this)->processImpl(input, output, params);
    }

};

class Grayscale : public ProcessingStrategy<Grayscale> {
public:
    void processImpl(const cv::Mat& input, cv::Mat& output, const std::vector<float>& params);
};

class Blur : public ProcessingStrategy<Blur> {
public:
    void processImpl(const cv::Mat& input, cv::Mat& output, const std::vector<float>& params);
};

class Canny : public ProcessingStrategy<Canny> {
public:
    void processImpl(const cv::Mat& input, cv::Mat& output, const std::vector<float>& parms);
};