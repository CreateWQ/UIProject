#pragma once 

#include <logger/logger_facade.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <memory>

#include <image_process/strategy.hpp>

enum class Process {
    Grayscale = 0,
    Blur = 1,
    Canny = 2,
};

class ImageProcessor {
private:

    std::shared_ptr<LoggerFacade> logger;

    cv::Mat original_image;
    cv::Mat processed_image;
    
    // cv::Mat changed_image;
    // unsigned int changed_texture;

    unsigned int original_texture;
    unsigned int processed_texture;

    bool has_image;

    std::unique_ptr<ProcessingStrategy<Grayscale>> grayscale_strategy;
    std::unique_ptr<ProcessingStrategy<Blur>> blur_strategy;
    std::unique_ptr<ProcessingStrategy<Canny>> canny_strategy;

    void loadTexture(const cv::Mat& image, unsigned int& texture);

public:

    ImageProcessor(std::shared_ptr<LoggerFacade> log);
    ~ImageProcessor();

    bool loadImage(const std::string& path);
    // void saveChangedImage(const std::string& path);
    
    void processImage(int strategy_index, const std::vector<float>& params);
    void saveProcessed(const std::string& path);
    inline unsigned int getProcessedTexture() const { return processed_texture; }
    cv::Mat generateThumbnail(const std::string& path, int size = 50);


    inline unsigned int getOriginalTexture() const { return original_texture; }
    // inline unsigned int getChangedTexture() const { return changed_texture; }
    inline cv::Size getImageSize() const { return original_image.size(); }
    inline bool hasImageLoaded() const { return has_image; }

};