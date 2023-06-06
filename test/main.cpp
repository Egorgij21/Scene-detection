#include <iostream>
#include <vector>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <detection/detection.hpp>

int main(int argc, char** argv) {
    
    if (argc < 4) {
        std::cerr << "Invalid number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <src_file_path> <dest_folder_path> <threshold>" << std::endl;
        return 1;
    }

    const std::string srcPath(argv[1]), destDir(argv[2]);
    const float threshold(std::stof(argv[3]));

    if (argc == 5) {
        int kernelSize = std::stoi(argv[4]);
    }


    VideoCutter videoCutter(srcPath, destDir);
    int kernelSize = 5;
    videoCutter.cut(threshold, FilterType::GAUSSIAN_BLUR, kernelSize);

    std::cout << "Done!" << std::endl;
    
    return 0;
}
