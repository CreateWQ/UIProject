#include "image_processor.hpp"
#include <GLFW/glfw3.h>

ImageProcessor::ImageProcessor(std::shared_ptr<LoggerFacade> log)
    : logger(log), original_texture(0), processed_texture(0), has_image(false),
    grayscale_strategy(std::make_unique<Grayscale>()),
    blur_strategy(std::make_unique<Blur>()),
    canny_strategy(std::make_unique<Canny>()) {

    if (logger) logger->info("ImageProcessor initialized");

}

ImageProcessor::~ImageProcessor() {
    if (original_texture) glDeleteTextures(1, &original_texture);
    if (processed_texture)  glDeleteTextures(1, &processed_texture);
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

// bool ImageProcessor::loadImage(const std::string& path) {
//     try {
//         original_image = cv::imread(path, cv::IMREAD_COLOR);
//         if (original_image.empty()) {
//             if (logger) logger->error("Failed to load image: {}", path);
//             return false;
//         }

//         cv::cvtColor(original_image, original_image, cv::COLOR_BGR2RGBA);
//         cv::cvtColor(original_image, processed_image, cv::COLOR_RGBA2GRAY);
//         cv::cvtColor(processed_image, processed_image, cv::COLOR_GRAY2RGBA);

//         loadTexture(original_image, original_texture);
//         loadTexture(processed_image, processed_texture);

//         has_image = true;
//         if (logger) logger->info("Loaded image: {}", path);
//         return true;
//     } catch (const std::exception& e) {
//         if (logger) logger->error("Failed to process image: {}", e.what());
//         return false;
//     }
// }

bool ImageProcessor::loadImage(const std::string& path) {
    try {
        original_image = cv::imread(path, cv::IMREAD_COLOR);
        if (original_image.empty()) {
            if (logger) logger->error("Failed to load image: {}", path);
            return false;
        }

        cv::cvtColor(original_image, original_image, cv::COLOR_BGR2RGBA);
        processed_image = original_image.clone();

        loadTexture(original_image, original_texture);
        loadTexture(processed_image, processed_texture);

        has_image = true;
        if (logger) logger->info("Loaded image: {}", path);
        return true;
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to process image: {}", e.what());
        return false;
    }
}

// void ImageProcessor::saveChangedImage(const std::string& path) {

//     try {
//         if (!has_image) {
//             if (logger) logger->error("No processed image to save");
//             return;
//         }

//         cv::Mat save_image;
//         cv::cvtColor(processed_image, save_image, cv::COLOR_RGBA2BGR);
//         cv::imwrite(path, save_image);
//         if (logger) logger->info("Processed image save: {}", path);
//     } catch (const std::exception& e) { 
//         if (logger) logger->error("Failed to save grayscale image: {}", e.what());
//     }

// }

void ImageProcessor::processImage(int strategy_index, const std::vector<float>& params) {
    
    try {
        if (!has_image) return;
        
        std::map<int, std::string> strategy_name;
        strategy_name[0] = "Grayscale";
        strategy_name[1] = "Blur";
        strategy_name[2] = "Canny";

        switch (strategy_index) {
            case 0: grayscale_strategy->process(original_image, processed_image, params);
                break;
            case 1: blur_strategy->process(original_image, processed_image, params);
                break;
            case 2: canny_strategy->process(original_image, processed_image, params);
                break;
            default: return;
        }
        loadTexture(processed_image, processed_texture);
        if (logger) logger->info("Applied processing strategy: {}", strategy_name[strategy_index]);
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to process image: {}", e.what());
    }

}

void ImageProcessor::saveProcessed(const std::string& path) {
    try {
        if (!has_image) {
            if (logger) logger->error("No processed image to save");
            return;
        }
        cv::Mat save_image;
        cv::cvtColor(processed_image, save_image, cv::COLOR_RGBA2BGR);
        cv::imwrite(path, save_image);
        if (logger) logger->info("Processed image save: {}", path);

    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to save processed image: {}", e.what());
    }
}

cv::Mat ImageProcessor::generateThumbnail(const std::string& path, int size) {
    try {
        cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
        if (image.empty()) {
            if (logger) logger->error("Failed to load thumbnail: {}", path);
            return cv::Mat();
        }

        cv::Mat thumbnail;
        cv::resize(image, thumbnail, cv::Size(size, size), 0, 0, cv::INTER_AREA);
        cv::cvtColor(thumbnail, thumbnail, cv::COLOR_BGR2RGBA);
        if (logger) logger->debug("Generated thumbnail for: {}", path);
        return thumbnail;
    } catch (const std::exception& e) {
        if (logger) logger->error("Failed to generate thumbnail: {}", e.what());
        return cv::Mat();
    }
}