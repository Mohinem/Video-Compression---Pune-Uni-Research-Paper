#include "opencv2/opencv.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace cv;


// Convert RGB to YUV
Mat convert_to_yuv(Mat image) {
    Mat yuv;
    cvtColor(image, yuv, COLOR_RGB2YUV);
    return yuv;
}

int main(){

  // Capture the video from the file

  VideoCapture cap("1280_RL1nIR7Q.avi");

  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  // Vector of frames (to contain the frames converted to YUV)
  vector<Mat> yuv_frames;  

  // Go frame by frame and convert to YUV and store in the vector

  while(1){
    Mat frame;
    cap >> frame;
    if(frame.empty()){
      cout << "End of the video" << endl;
      break;
    }

    // Display the frame
    imshow("Frame", frame);
    yuv_frames.push_back(convert_to_yuv(frame));
  } 

  // Setting key frames
  set <int> key_frames;
  key_frames.insert(0);

  int last_key_frame = 0;
	for(int i=1;i<yuv_frames.size();i++){
    //calculate similarity between two frames after preprocessing
    
    //normalize the histogram
    Mat hist_frame1 = yuv_frames[last_key_frame];
    Mat hist_frame2 = yuv_frames[i];
    Mat hist_frame1_norm, hist_frame2_norm;

    hist_frame1.convertTo(hist_frame1, CV_32F);
    hist_frame2.convertTo(hist_frame2, CV_32F);

    normalize(hist_frame1, hist_frame1_norm, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(hist_frame2, hist_frame2_norm, 0, 1, NORM_MINMAX, -1, Mat());

    double similarity = compareHist(hist_frame1, hist_frame2, 0);
    
    //if similarity is greater than 0.7, then insert the frame number in the key_frames set
    if(similarity > 0.7){
      key_frames.insert(i);
      last_key_frame = i;
    }
  }

  vector <Mat> diff_frames;

  for(int i=0;i<yuv_frames.size();i++){
    // cout<<"i = "<<i<<endl;
    if(key_frames.find(i) == key_frames.end() and i < yuv_frames.size()-1){
      diff_frames.push_back(yuv_frames[i]-yuv_frames[i+1]);
    }
  }

// Convert diff_frames to byte stream and store in file diff_frames.txt
  ofstream outfile;
  outfile.open("diff_frames.txt",ios::binary);
  for(int i=0;i<diff_frames.size();i++){
    Mat diff_frame = diff_frames[i];
    diff_frame.convertTo(diff_frame, CV_8U);
    outfile << diff_frame;
  }
  outfile.close();
  cout<<"Difference frames stored in diff_frames.txt"<<endl;
 // Read diff_frames.txt and save its run length encoded version in diff_frames_rle.txt
  ifstream infile;
  infile.open("diff_frames.txt", ios::binary);
  ofstream outfile2;
  outfile2.open("diff_frames_rle.txt");
  int count = 0;
  int cur_val = 0;
  string cur_string = "";
  int prev_val = 0;
  int cnt=0;
  while(infile >> cur_string){
    cout<<cur_string<<endl;
    if(cur_string[0]=='['){
      outfile2<<"[";
      prev_val = 0;
      cur_val = 0;
      cnt = 0;
    }
    else{
      int n=cur_string.size();
      if(cur_string[n-1]==']'){
        outfile2<<"("<<prev_val<<","<<cnt<<")]";
        cout<<"\n";
      }
      else{
        cur_val = 0;
        for(int i=0;i<n-1;i++){
          cur_val = cur_val*10 + (cur_string[i] - '0');
        }
        if(cur_val == prev_val){
          cnt++;
        }
        else{
          outfile2<<"("<<prev_val<<","<<cnt<<")";
          prev_val = cur_val;
          cnt = 1;
        }
      }
    }
  }

  outfile2.close();
  infile.close();


  cout<<"Byte stream and run length encoding file generated"<<endl;
  return 0;
}