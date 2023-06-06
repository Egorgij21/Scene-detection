#pragma once


#ifndef DETECTION_HPP
#define DETECTION_HPP


#include <iosfwd>
#include <cstddef>
#include <opencv2/opencv.hpp>

enum FilterType {
    DEFAULT_THRESHOLD,
    GAUSSIAN_BLUR,
    CANNY_EDGE,
    BACKGROUND_SUBTRACTION
};

class VideoReader {
public:
    VideoReader(const std::string &filename);

    bool hasNextFrame() const;

    void getNextFrame(cv::Mat &frame);

    double getFrameTimestamp() const;

private:
    mutable cv::VideoCapture cap;
};


class Detector {
public:
    Detector(const std::string &filename) : reader(filename), frame() {}

    void processWithThreshold(float threshold);
    void processWithGaussianBlur(float threshold, int kernelSize);
    void processWithCannyEdge(float threshold, int threshold1, int threshold2);
    void processWithBackgroundSubtraction(float threshold);

    void process(float threshold, FilterType filterType, int kernelSize = 1, float arg1 = 0.0, float arg2 = 0.0);
    std::vector<double> getTimeCodes();

private:
    VideoReader reader;
    cv::Mat frame;
    std::vector<double> timecodes;
};


class VideoCutter {
public:
    VideoCutter(const std::string &srcPath, const std::string &destDir);
    void cut(float threshold, FilterType filterType, int kernelSize = 1, float arg1 = 0.0, float arg2 = 0.0);

private:
    std::string srcPath;
    std::string destDir;
};






#endif
