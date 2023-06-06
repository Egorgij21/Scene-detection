#include <iostream>
#include <vector>
#include <sstream>
#include <detection/detection.hpp>


int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Invalid number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <src_file_path> <dest_folder_path> <threshold>" << std::endl;
        return 1;
    }
    else {
        const std::string srcPath(argv[1]), destDir(argv[2]);
        const float threshold(std::stof(argv[3]));

        std::string FT = std::string("DEFAULT_THRESHOLD");
    
        if (argc >= 5) {
            FT = std::string(argv[4]);
        }

        if (FT == std::string("DEFAULT_THRESHOLD")) {
            FilterType filterType = FilterType::DEFAULT_THRESHOLD;

            VideoCutter videoCutter(srcPath, destDir);
            videoCutter.cut(threshold, filterType);

            std::cerr << "Done!" << std::endl;
        }
        else if (FT == std::string("GAUSSIAN_BLUR")) {
            FilterType filterType = FilterType::GAUSSIAN_BLUR;
            try {
                int kernelSize = std::stoi(argv[5]);

                VideoCutter videoCutter(srcPath, destDir);
                videoCutter.cut(threshold, filterType, kernelSize);

                std::cerr << "Done!" << std::endl;
            }
            catch(...) {
                std::cerr << "for GAUSSIAN_BLUR put Kernel size into parameters" << std::endl;
            }
        }
        else if (FT == std::string("CANNY_EDGE")) {
            FilterType filterType = FilterType::CANNY_EDGE;
            try {
                int arg1 = std::stoi(argv[5]);
                int arg2 = std::stoi(argv[6]);

                VideoCutter videoCutter(srcPath, destDir);
                videoCutter.cut(threshold, filterType, arg1=arg1, arg2=arg2);

                std::cerr << "Done!" << std::endl;
            }
            catch(...) {
                std::cerr << "for CANNY_EDGE put threshold one and two into parameters" << std::endl;
            }
        }
        else if (FT == std::string("BACKGROUND_SUBTRACTION")) {
            FilterType filterType = FilterType::BACKGROUND_SUBTRACTION;
            try {
                VideoCutter videoCutter(srcPath, destDir);
                videoCutter.cut(threshold, filterType);

                std::cerr << "Done!" << std::endl;
            }
            catch(...) {
                std::cerr << "invalid input for BACKGROUND_SUBTRACTION" << std::endl;
            }
        }
        else {
            std::cerr << "Invalid filter type argument" << std::endl;
        }
    }
    return 0;
}
