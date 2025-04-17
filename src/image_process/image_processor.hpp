#pragma once 

#include <logger/logger_facade.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <memory>

class ImageProcessor {
private:

    std::shared_ptr<LoggerFacade> logger;

    cv::Mat original_image;
    cv::Mat changed_image;

    unsigned int original_texture;
    unsigned int changed_texture;

    bool has_image;

    void loadTexture(const cv::Mat& image, unsigned int& texture);

public:

    ImageProcessor(std::shared_ptr<LoggerFacade> log);
    ~ImageProcessor();

    bool loadImage(const std::string& path);
    void saveChangedImage(const std::string& path);
    
    inline unsigned int getOriginalTexture() const { return original_texture; }
    inline unsigned int getChangedTexture() const { return changed_texture; }
    inline cv::Size getImageSize() const { return original_image.size(); }
    inline bool hasImageLoaded() const { return has_image; }
    inline void processed() { has_image = false; }
};