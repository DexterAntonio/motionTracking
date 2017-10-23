#include "opencv2/opencv.hpp"
#include "context.h"
#include "frame_function.h"
#include <iostream>

using namespace cv;
using namespace std;

int run_camera(Context& context)
{
    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened()){  // check if we succeeded
        std::cout << "No Camera detected." << std::endl;
        return -1;
    }
    // Mat edges;
    namedWindow("Input",1);
    namedWindow("Output",1);

   Mat frame(480, 640, CV_8UC3);
    //Mat frame(64,96,CV_8UC3); //doesn't work 
    for(;;)
    {
        cap >> frame; // get a new frame from camera

        imshow("Input", frame);
        
        int size = context.height*context.width*3;

        vector<uint8_t> destimg((uint8_t*) frame.data, (uint8_t*)frame.data+size);
        frame_function(context, destimg);

        Mat m(480, 640, CV_8UC3, (uint8_t*) &destimg[0]); 
        //Mat m(64, 96, CV_8UC3, (uint8_t*) &destimg[0]); 
        imshow("Output", m);
        char c = waitKey(1);

if (c == 27)
{
break;
}
if(c == 105){ //i
   context.invert = true; 
}
if(c == 102){ //f 
   context.flip = true; 
}
if(c ==97){  //a
  context.average = true; 
}
if (c == 110){ //n, i.e. normal 
   context.invert = false;
   context.flip = false; 
   context.average = false;     
}

//this section resets the background encase you move the camera 
if (c == 114){
  context.reset = true;
}


//this section turns on the tracker if the "t" key is pressed  
if (c == 116){ //tracking 
  std::string color;
  context.color = std::vector<int> (3,255);
  std::cout << "please enter tracking color, RED, GREEN,BLUE, WHITE or BLACK" << std::endl; 
  std::cin >> color;
  if(color.compare("RED")==0){
    context.color[0] = 255;
    context.color[1] = 0;
    context.color[2] = 0;
    std::cout << "You selected red " << endl;
  }
  else if(color.compare("BLUE")==0){
    context.color[0] = 0;
    context.color[1] = 0;
    context.color[2] = 255;
    std::cout << "You selected blue " << endl;
  }
  else if(color.compare("GREEN")==0){
    context.color[0] = 0;
    context.color[1] = 255;
    context.color[2] = 0;
    std::cout <<"you selected GREEN" <<endl;
  }
  else if(color.compare("WHITE")==0){
    context.color[0] = 255;
    context.color[1] = 255;
    context.color[2] = 255;
    std::cout << "you selected WHITE " << endl;
  }

  else if(color.compare("BLACK")==0){
    context.color[0] = 0;
    context.color[1] = 0;
    context.color[2] = 0;
  }
  else{
    std::cout << "you did not select a valid color,set to white" <<endl;
  }
  if(context.trackMov){
    context.trackMov = false;
  }
  else{
    context.trackMov = true;
  }
}

}    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;

}
