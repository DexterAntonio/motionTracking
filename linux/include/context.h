#ifndef __CONTEXT__
#define __CONTEXT__
#include <iostream>
#include <vector> 
class Context{
    //Maintain state if needed

public:
	int height = 480; // for example
	int width = 640; // for example
	//int width = 64;
	//int height = 96; 

	bool invert = false; 
	bool average = false;
	bool flip = false; 
	bool trackMovement = true;
	bool reset = false; 
	int colorinteger =0;
	bool trackMov = false; 

  	//used for fake emun example 
  	int colorInteger =0;



	//these store the last four cof of smoother object tracking
	std::vector<int> color;

	double prevXcof = 0;
	double prevYcof = 0;
	double p2Xcof = 0;
	double p2Ycof = 0; 
	double p3Xcof = 0;
	double p3Ycof = 0; 
	double p4Xcof = 0;
	double p4Ycof = 0; 

	//these double cof is used for object tracking 
	double dXcof = 0;
	double dYcof = 0;


	std::vector<uint8_t> change; 
	std::vector<uint8_t> absChange; 
	int avgIValue = 0;
	int avgIint = 0; 
	bool vDeclared = false;
	int threshold = 60; 
	std::vector<uint8_t> avgBackground; 
	long interations = 0; 

};

#endif
