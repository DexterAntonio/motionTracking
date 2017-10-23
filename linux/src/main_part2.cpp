#include "ps3eye.h"
#include "context.h"
#include "frame_function.h"
#include <iostream> 
#include "../include/context.h"
#define STRINGMACRO "||======================================||" //example of macro 
int main(){
	//STRINGMACRO; 
	std::string color;
	std::cout << STRINGMACRO <<endl;

	std::cout << "HW4: MOVEMENT TRACKING" << std::endl;
	std::cout << "Instructions: Please keep camera still. To activate movement tracking " << endl;
	std::cout << "Click on output window and press t then " <<endl;
	std::cout << "enter color you would like to track images with in the command window" <<endl;
  	std::cout<< "OTHER COMMANDS: press a to average, i to invert, f to flip, n to go back to normal, esc to quit" <<std::endl;
  	std::cout << "if you move the camera press r to reset the background used to detect movement " <<endl;
    	std::cout << STRINGMACRO <<endl;

    Context ctx;
    run_camera(ctx);
}
