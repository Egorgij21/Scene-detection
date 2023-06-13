# Scene-detection

Detection of scene changing with c++ and opencv 

## How to start the code in terminal:

 ```
 git clone https://github.com/Egorgij21/Scene-detection <path/to/repo>
 
 cd <path/to/repo>
 
 mkdir build
 
 cd build
 
 cmake ..
 
 make
 
 cd ../bin
  
 ./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <filter name> <kernel size> <threshold1> <threshold2>
 ```
  
## Parameters:
  
### Filter names:     
- DEFAULT_THRESHOLD,
- GAUSSIAN_BLUR,
- CANNY_EDGE,
- BACKGROUND_SUBTRACTION
### Types of parameters:
- threshold: float     (for start choose from 10 to 50)
- kernel size: int   (5 is a good variant for this parameter)
- threshold1: int  ()
- threshold2: int  ()
### Choose Filter:

- for DEFAULT_THRESHOLD you need to put these parameters:
```
./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold>
```
  
- for GAUSSIAN_BLUR you need to put these parameters:
```
./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <GAUSSIAN_BLUR> <kernel size>
```
  
- for CANNY_EDGE you need to put these parameters:
```
./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <CANNY_EDGE> <threshold1> <threshold2>
```
  
- for BACKGROUND_SUBTRACTION you need to put these parameters:
```
./detect <path/to/video> <path/to/folder_for_cutted_videos> <threshold> <BACKGROUND_SUBTRACTION>
```
### Formats:
- input video formats: mp4
- output video formats: avi
