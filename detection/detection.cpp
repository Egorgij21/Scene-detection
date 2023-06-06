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
        timestamp = cap.get(cv::CAP_PROP_POS_MSEC) / 1000.0; // convert milliseconds to seconds
    }

    return timestamp;
}


void Detector::processWithThreshold(float threshold) {
    cv::Mat prevFrame, curFrame, diffFrame;

    // Read the first frame
    reader.getNextFrame(prevFrame);

    // Initialize the current frame and difference frame
    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;
    while (reader.hasNextFrame()) {
        // Get the current frame
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }
        // Compute the absolute difference between the frames
        cv::absdiff(curFrame, prevFrame, diffFrame);

        // Compute the average pixel value of the difference image
        float diffAverage = cv::mean(diffFrame)[0];

        // If the difference is above the threshold, it suggests a scene change
        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
            //cv::imshow("Video", curFrame);
            //cv::waitKey(1);
            // Do something to mark the scene change (e.g. draw a rectangle on the frame)
        }

        // Save the current frame for the next iteration
        curFrame.copyTo(prevFrame);

        // Display the current frame
        
        //cv::imshow("Video", curFrame);
        //cv::waitKey(1);
        

        i++;
    }
    timecodes.push_back(i);
}

void Detector::processWithGaussianBlur(float threshold, int kernelSize) {

    cv::Mat prevFrame, curFrame, diffFrame;

    // Read the first frame
    reader.getNextFrame(prevFrame);

    // Initialize the current frame and difference frame
    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;

    while (reader.hasNextFrame()) {
        // Get the current frame
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }
        // Apply Gaussian blur to both frames
        cv::GaussianBlur(curFrame, curFrame, cv::Size(kernelSize, kernelSize), 0);
        cv::GaussianBlur(prevFrame, prevFrame, cv::Size(kernelSize, kernelSize), 0);

        // Compute the absolute difference between the frames
        cv::absdiff(curFrame, prevFrame, diffFrame);

        // Compute the average pixel value of the difference image
        float diffAverage = cv::mean(diffFrame)[0];

        // If the difference is above the threshold, it suggests a scene change
        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
            // Do something to mark the scene change (e.g. draw a rectangle on the frame)
        }
        curFrame.copyTo(prevFrame);

        

        i++;
    }
    timecodes.push_back(i);
}

void Detector::processWithCannyEdge(float threshold, int threshold1, int threshold2) {
    cv::Mat prevFrame, curFrame, diffFrame;

    // Read the first frame
    reader.getNextFrame(prevFrame);
    // Initialize the current frame and difference frame
    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;

    while (reader.hasNextFrame()) {
        // Get the current frame
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }            

        // Convert both frames to grayscale
        cv::cvtColor(curFrame, curFrame, cv::COLOR_BGR2GRAY);
        cv::cvtColor(prevFrame, prevFrame, cv::COLOR_BGR2GRAY);

        // Apply Canny edge detection to both frames
        cv::Canny(curFrame, curFrame, threshold1, threshold2);
        cv::Canny(prevFrame, prevFrame, threshold1, threshold2);

        // Compute the absolute difference between the frames
        cv::absdiff(curFrame, prevFrame, diffFrame);

        // Compute the average pixel value of the difference image
        float diffAverage = cv::mean(diffFrame)[0];

        // If the difference is above the threshold, it suggests a scene change
        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
            // Do something to mark the scene change (e.g. draw a rectangle on the frame)
        }

        // Save the current frame for the next iteration
        curFrame.copyTo(prevFrame);

        // Display the current frame
        //cv::imshow("Video", curFrame);
        //cv::waitKey(1);

        i++;
    }
    timecodes.push_back(i);
}


void Detector::processWithBackgroundSubtraction(float threshold) {
    cv::Mat prevFrame, curFrame, diffFrame, background;

    // Read the first frame and use it as the background image
    reader.getNextFrame(background);

    // Initialize the current frame and difference frame
    reader.getNextFrame(prevFrame);
    curFrame = prevFrame.clone();
    diffFrame = cv::Mat::zeros(prevFrame.size(), prevFrame.type());

    int i = 0;
    while (reader.hasNextFrame()) {
        // Get the current frame
        reader.getNextFrame(curFrame);
        if(i == 0) {
            timecodes.push_back(i);
        }

        // Subtract the background image from the current frame to isolate moving objects
        cv::absdiff(curFrame, background, curFrame);

        // Convert the result to grayscale
        cv::cvtColor(curFrame, curFrame, cv::COLOR_BGR2GRAY);

        // Compute the absolute difference between the frames
        cv::absdiff(curFrame, prevFrame, diffFrame);

        // Compute the average pixel value of the difference image
        float diffAverage = cv::mean(diffFrame)[0];

        // If the difference is above the threshold, it suggests a scene change
        if (diffAverage > threshold) {
            timecodes.push_back(i);
            std::cout << "Scene change detected at frame " << i << std::endl;
            // Do something to mark the scene change (e.g. draw a rectangle on the frame)
        }

        // Save the current frame for the next iteration
        curFrame.copyTo(prevFrame);

        // Display the current frame
        //cv::imshow("Video", curFrame);
        //cv::waitKey(1);

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
