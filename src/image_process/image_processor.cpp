#include "image_processor.hpp"
#include <GLFW/glfw3.h>

ImageProcessor::ImageProcessor(std::shared_ptr<LoggerFacade> log)
    : logger(log), original_texture(0), changed_texture(0), has_image(false) {

    if (logger) logger->info("ImageProcessor initialized");

}

ImageProcessor::~ImageProcessor() {
    if (original_texture) glDeleteTextures(1, &original_texture);
    if (changed_texture)  glDeleteTextures(1, &changed_texture);
    if (logger) logger->info("ImageProcessor destroyed");
}

void ImageProcessor::loadTexture(const cv::Mat& image, unsigned int& texture) {
    if (texture) glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool ImageProcessor::loadImage(const std::string& path) {
    try {
        original_image = cv::imread(path, cv::IMREAD_COLOR);
        if (original_image.empty()) {
            if (logger) logger->error("Failed to load image: {}", path);
            return false;
        }

        cv::cvtColor(original_image, original_image, cv::COLOR_BGR2RGBA);
        cv::cvtColor(original_image, changed_image, cv::COLOR_RGBA2GRAY);
        cv::cvtColor(changed_image, changed_image, cv::COLOR_GRAY2RGBA);

        loadTexture(original_image, original_texture);
        loadTexture(changed_image, changed_texture);

        has_image = true;
        if (logger) logger->info("Loaded image: {}", path);
        return true;
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to process image: {}", e.what());
    }
}

void ImageProcessor::saveChangedImage(const std::string& path) {

    try {
        if (!has_image) {
            if (logger) logger->error("No processed image to save");
            return;
        }

        cv::Mat save_image;
        cv::cvtColor(changed_image, save_image, cv::COLOR_RGBA2BGR);
        cv::imwrite(path, save_image);
        if (logger) logger->info("Processed image save: {}", path);
    } catch (const std::exception& e) { 
        if (logger) logger->error("Failed to save grayscale image: {}", e.what());
    }

}
