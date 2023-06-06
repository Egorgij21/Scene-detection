# Scene-detection

### How to start the code in terminal:

0) git clone https://github.com/Egorgij21/Scene-detection <path/to/repo>
1) cd <path/to/repo>
2) mkdir build
3) cd build
4) cmake ..
5) make
6) cd ..
7) cd bin
8) ./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <filter name> <kernel size> <threshold1> <threshold2>
  
### Parameters
  
Filter names:     DEFAULT_THRESHOLD,
                  GAUSSIAN_BLUR,
                  CANNY_EDGE,
                  BACKGROUND_SUBTRACTION
Types of parameters:
                  threshold: int     (for start choose from 10 to 50)
                  kernel size: int   (5 is a good variant for this parameter)
                  threshold1: float  ()
                  threshold2: float  ()

for DEFAULT_THRESHOLD you need to put give parameters:
8) ./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold>
  
for GAUSSIAN_BLUR you need to put give parameters:
8) ./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <GAUSSIAN_BLUR> <kernel size>
  
for CANNY_EDGE you need to put give parameters:
8) ./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <CANNY_EDGE> <threshold1> <threshold2>
  
for BACKGROUND_SUBTRACTION you need to put give parameters:
8) ./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <BACKGROUND_SUBTRACTION>
  
* input video formats: mp4
* output video formats: avi
