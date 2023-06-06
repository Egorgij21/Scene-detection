#include <detection/detection.hpp>

#include <iostream>
#include <vector>
#include <sstream>



VideoReader::VideoReader(const std::string &filename) : cap(filename) {
    if (!cap.isOpened()) {
        throw std::runtime_error("Cannot open video file " + filename);
    }
}

bool VideoReader::hasNextFrame() const {
    cv::Mat dummy;
    bool success = cap.read(dummy);
    if (success) {
        cap.set(cv::CAP_PROP_POS_FRAMES, cap.get(cv::CAP_PROP_POS_FRAMES) - 1);
    }
    return success;
}

void VideoReader::getNextFrame(cv::Mat &frame) {
    cap.read(frame);
}

double VideoReader::getFrameTimestamp() const {
    double timestamp = -1;
    cv::Mat frame;

    if(cap.isOpened() && cap.read(frame)) {
        timestamp = cap.get(cv::CAP_PROP_POS_MSEC) / 1000.0;
    }

    return timestamp;
}


void Detector::processWithThreshold(float threshold) {
    std::cerr << "default threshold detection" << std::endl;
    cv::Mat prevFrame, curFrame, diffFrame;

    reader.getNextFrame(prevFrame);

    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;
    while (reader.hasNextFrame()) {

        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }
        cv::absdiff(curFrame, prevFrame, diffFrame);

        float diffAverage = cv::mean(diffFrame)[0];

        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
        }

        curFrame.copyTo(prevFrame);

        i++;
    }
    timecodes.push_back(i);
}

void Detector::processWithGaussianBlur(float threshold, int kernelSize) {
    std::cerr << "gaussian blur detection" << std::endl;

    cv::Mat prevFrame, curFrame, diffFrame;

    reader.getNextFrame(prevFrame);

    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;

    while (reader.hasNextFrame()) {
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }
        cv::GaussianBlur(curFrame, curFrame, cv::Size(kernelSize, kernelSize), 0);
        cv::GaussianBlur(prevFrame, prevFrame, cv::Size(kernelSize, kernelSize), 0);

        cv::absdiff(curFrame, prevFrame, diffFrame);

        float diffAverage = cv::mean(diffFrame)[0];

        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
        }
        curFrame.copyTo(prevFrame);

        

        i++;
    }
    timecodes.push_back(i);
}

void Detector::processWithCannyEdge(float threshold, int threshold1, int threshold2) {
    std::cerr << "canny edge detection" << std::endl;

    cv::Mat prevFrame, curFrame, diffFrame;

    reader.getNextFrame(prevFrame);
    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;

    while (reader.hasNextFrame()) {
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }

        cv::cvtColor(curFrame, curFrame, cv::COLOR_BGR2GRAY);
        if (i == 0) {
            cv::cvtColor(prevFrame, prevFrame, cv::COLOR_BGR2GRAY);
        }

        cv::Canny(curFrame, curFrame, threshold1, threshold2);
        cv::Canny(prevFrame, prevFrame, threshold1, threshold2);

        cv::absdiff(curFrame, prevFrame, diffFrame);

        float diffAverage = cv::mean(diffFrame)[0];
        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
        }

        curFrame.copyTo(prevFrame);

        i++;
    }
    timecodes.push_back(i);
}


void Detector::processWithBackgroundSubtraction(float threshold) {
    std::cerr << "background subtraction detection" << std::endl;

    cv::Mat prevFrame, curFrame, diffFrame, background;

    reader.getNextFrame(background);

    reader.getNextFrame(prevFrame);
    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;
    while (reader.hasNextFrame()) {
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }

        cv::absdiff(curFrame, background, curFrame);

        cv::cvtColor(curFrame, curFrame, cv::COLOR_BGR2GRAY);
        if (i == 0) {
            cv::cvtColor(prevFrame, prevFrame, cv::COLOR_BGR2GRAY);
        }

        cv::absdiff(curFrame, prevFrame, diffFrame);

        float diffAverage = cv::mean(diffFrame)[0];

        if (diffAverage > threshold && i != 0) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
        }

        curFrame.copyTo(prevFrame);


        i++;
    }
    timecodes.push_back(i);
}


void Detector::process(float threshold, FilterType filterType, int kernelSize, float arg1, float arg2) {

    timecodes.clear();

    switch (filterType) {
        case FilterType::DEFAULT_THRESHOLD:
            processWithThreshold(threshold);
            break;
        case FilterType::GAUSSIAN_BLUR:
            processWithGaussianBlur(threshold, kernelSize);
            break;
        case FilterType::CANNY_EDGE:
            processWithCannyEdge(threshold, arg1, arg2);
            break;
        case FilterType::BACKGROUND_SUBTRACTION:
            processWithBackgroundSubtraction(threshold);
            break;
        default:
            std::cout << "Invalid filter type specified" << std::endl;
            break;
    }
}

std::vector<double> Detector::getTimeCodes() {
    return timecodes;
}

VideoCutter::VideoCutter(const std::string &srcPath, const std::string &destDir)
    : srcPath(srcPath), destDir(destDir) {}

void VideoCutter::cut(float threshold, FilterType filterType, int kernelSize, float arg1, float arg2) {
    Detector detector(srcPath);
    detector.process(threshold, filterType, kernelSize, arg1, arg2);
    std::vector<double> timecodes = detector.getTimeCodes();
    cv::VideoCapture videoCapture(srcPath);
    if (!videoCapture.isOpened()) {
        std::cerr << "Failed to open source video file" << std::endl;
        return;
    }
    int frameWidth = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = videoCapture.get(cv::CAP_PROP_FPS);
    std::string destPathPrefix = destDir + "/segment";
    int segmentIndex = 1;
    for (int i = 0; i < timecodes.size() - 1; i++) {
        double startFrame = timecodes[i];
        double endFrame = timecodes[i + 1];
        std::stringstream segmentPathStream;
        segmentPathStream << destPathPrefix << segmentIndex++ << ".avi";
        std::string segmentPath = segmentPathStream.str();
        cv::VideoWriter writer(segmentPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frameWidth, frameHeight), true);
        for (int j = startFrame; j <= endFrame; j++) {
            cv::Mat frame;
            videoCapture.set(cv::CAP_PROP_POS_FRAMES, j);
            videoCapture >> frame;
            if(frame.rows != frameHeight || frame.cols != frameWidth) {
                cv::resize(frame, frame, cv::Size(frameWidth, frameHeight));
            }
            writer.write(frame);
        }
        writer.release();
    }
    videoCapture.release();
}

