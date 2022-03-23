# Video-Compression---Pune-Uni-Research-Paper
An implementation of a video compression algorithm based on a research paper from Pune University professors, using Run Length Encoding on video frames represented in YUV format.
It uses OpenCV to handle the videos. It makes use of run length encoding to compress the byte streams for each frame.
Link to the paper-
http://www.ijeert.org/pdf/v3-i3/4.pdf

## Prerequisites
1. C++
2. OpenCV framework

## Steps to run the program

1. Copy AVI file in the directory of the repository.
2. Open code.cpp, in line 18, replace the name in the brackets with the name to the avi video you just posted.
3. Compile and run.

## Expected output
1. diff_frames.txt - Intermediate byte stream for each frame.
2. diff_frames_rle.txt - Final Run Length Encoded version of diff_frames.txt.

## Results of compression
1. Run length encoding successfully compresses the intermediate file.
2. The intermediate file generated is too large. It can be a mistake in the research paper, or in my implementation.
3. The process of selecting the key frames is quite unintuitive, as mentioned in paper. I would have checked for significant differences instead of similarities between key frame and last sub frame.
