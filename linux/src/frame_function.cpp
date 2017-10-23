#include "frame_function.h"
#include "hw2.h"
#include "hw1.h"
#include <cmath>
void frame_function(Context &context, vector<uint8_t> &buffer){
  //emus implimentation if I were to use them for something useful
  //this is how I would go about doing it 
  enum bestColor{red =0,blue,green,yellow,purple};
  if(context.colorinteger==red){
    //set the tracking color to red 
    //context.color[0] =255;
    //context.color[1] =0;
    //context.color[2] =0;
  }

  //this is left over from hw3 but it can still be used 
  if(context.invert){
    hw::invert_vector(buffer);
  }
  if(context.flip){
    hw::flip_vector(buffer,context.width,context.height); 
  } 
  if(context.average){
    hw::average_vector(buffer); 
  }

  //if the track movement button is pressed then movement will start being tracked 
  if(context.trackMov){
    if(!context.vDeclared){ //initilizes the vectors if they haven't been initilized yet 
      context.avgBackground = buffer; 
      context.change = buffer; 
      context.absChange = buffer;

    }
    context.vDeclared = true; //vectors are only initialized once 
    ++context.interations; //tracks iterations since last refresh 
    double distance; //squared distance between COF and pixel being evaulated 
    int rowPos; //determines the row and col position of a certain pixel 
    int colPos; 
    double maxDistance = context.height*context.height+context.width*context.width; //max squared distance 

    //this for loop creates a change vector between the weighted average background and the current buffer
    for(unsigned i = 0; i < context.avgBackground.size();i++){
        rowPos = i - 3*context.width*(i/(3*context.width));
        colPos = (i/(3*context.width)); 
        distance = (rowPos-context.prevXcof)*(rowPos-context.prevXcof)+(colPos-context.prevYcof)*(rowPos-context.prevXcof); //calculates square of distance 
        //this line below is important since it adds more weight to pixels close to the previous COM.
        //30 is just a number that worked really well 
        context.change.at(i) = std::abs(context.avgBackground.at(i)-buffer.at(i))*(1+(maxDistance-distance)/(30*maxDistance)); //computes weighted change vector 

        //possibly skip this step if the change is large
        context.avgBackground.at(i)=(context.avgBackground.at(i)*(context.interations-1)+buffer.at(i))/(context.interations); //updates the background vector
        //after 983 iterations the background is reset 
        //this is also called if the r key is pressed 
        if(context.interations%983==0||context.reset){ //this refreshes the buffer after a certain number of iterations 
          context.reset = false;
          context.avgBackground =buffer; 
          context.interations = 1; 
        }
    }
    //change vector has been created. We now move onto the next part 
    int avgIvalue = 0; 
    int avgIint = 0; 
    int cofB = 1; 
    int xtop = 0;
    int ytop = 0; 
    int k =0; //useful for selecting the correct pixels 


    for(unsigned i = 0; i < context.absChange.size();i++){
      //these k if statements aline the for loop with the start of the rbg vector 
      k = i; 
      if((k+1)%3==1){k = k+2;}
      else if((k+1)%3==2){k = k+1;}

      if(context.change.at(i)>40){ //threshold is set to 40 because it works well.     
          rowPos = k - 3*context.width*(k/(3*context.width));
          colPos = (k/(3*context.width)); 
          ytop = ytop + 1*colPos;
          xtop = xtop + 1*rowPos; 
          cofB++;  
          avgIint++;
          avgIvalue = (i+avgIvalue*avgIint)/(avgIint); 
          context.absChange.at(k) = context.color[0];
          context.absChange.at(k-1) = context.color[1];
          context.absChange.at(k-2) = context.color[2]; 
      }
      else{
          //if the change is not sigificant than the abs change vector gets filled with the buffer pixels
          context.absChange.at(k) = buffer.at(k); 
          context.absChange.at(k-1) = buffer.at(k-1);
          context.absChange.at(k-2) = buffer.at(k-2); 
      }

    }
    //computes cof 
    //integer and double cof are computed because it makes the code cleaner  
    int xcof = xtop/cofB;  
    int ycof = ytop/cofB; 
    double dXcof = (double) xtop/ (double) cofB;
    double dYcof = (double) ytop/ (double) cofB;
    int cofPos = xcof+ycof*context.width; 

    //using the average of these for slopes helps to smooth out the line
    //it still sucks but it is better than just one 
    //the best solution would be to do some matrix algebra to curve fit a 
    //trendline to all those points. I don't know how to do that in C++.

    double m4 = (dYcof-context.p4Ycof)/(dXcof-context.p4Xcof);
    double m3 = (dYcof -context.p3Ycof)/(dXcof-context.p3Xcof);
    double m2 = (dYcof-context.p2Ycof)/(dXcof-context.p2Xcof);
    double m1 = (dYcof-context.prevYcof)/(dXcof-context.prevXcof);
    double m = (m1+m2+m3+m4)/4; 
    //double m = (double) ytop/ (double) xtop; //m_top/m_bottom;
    double b = ((dYcof - m*dXcof)+(context.prevYcof-m*context.prevYcof))/2; //context.height-m*context.width; 
    int lineThickness = 5; //thickness of the white line 


    //std::cout << " m is ... " << m << endl; 
    //std::cout << " b is ... " << b << endl;
    //std::cout << " prevXcof is ..." << context.prevXcof<< endl;
    //std::cout << " prevXcof - xcof is ... " << (context.prevXcof-xcof) << endl;
    //std::cout << " b is ..." << b << endl;
    //std::cout << " m = ytop/xtop = " << ytop/xtop <<endl;;
    //int halfHeight = context.height*3/2;
    //int halfWidth = context.width*3/2; 

    context.absChange.at(cofPos) =0;

    //move all of the previous cof variables back an iteration
    bool movingRight = dXcof > context.p2Xcof;

    context.p4Xcof = context.p3Xcof;
    context.p4Ycof = context.p3Ycof;
    context.p3Xcof = context.p2Xcof;
    context.p3Ycof = context.p2Ycof;
    context.p2Xcof = context.prevXcof; 
    context.p2Ycof = context.prevYcof; 
    context.prevXcof = dXcof;
    context.prevYcof = dYcof;

    bool drawLine = true;

    //EXAMPLE OF DYNAMIC MEMORY ALOCATION 
    bool * pDrawBox = new bool;
    *pDrawBox = true; 

    //this forloop draws the white line on the image 
    //it also draws a green square at the COF 
    for(unsigned i = 0; i <context.absChange.size();i++){
      rowPos = i - 3*context.width*(i/(3*context.width)); //these might be switched confusinly sorry 
      colPos = (i/(3*context.width));
      //DYNAMIC MEMORY EXAMPLE  
      //draws green square at cof
      //I kept it in the final version because I think it is helpful for testing the program
      //you can always turn it off by setting *pDrawBox to false 
      if(cofB<500){ //if only a few pixels are detected as moving don't draw the box
        *pDrawBox = false; 
      }
      if((std::abs(rowPos-xcof)<50)&&*pDrawBox){
        if(std::abs(colPos-ycof)<16){
          if(i>2&&(i+1)%3==0){
            context.absChange.at(i) = 0;
            context.absChange.at(i-1) = 255;
            context.absChange.at(i-2) = 0; 
          }
        }
      }
      //determines what direction the thing is moving and uses that to draw the line
      if(movingRight){
        drawLine = rowPos <xcof; 
      }
      else{
        drawLine = rowPos > xcof;
      }
      if(cofB<500){ //if only a few pixels are detected as moving don't draw the line
        drawLine = false; 
      }
      if((std::abs(rowPos*m+b-colPos)<lineThickness)&&(drawLine)){
          k = i; 
          if((k+1)%3==1){k = k+2;}
          else if((k+1)%3==2){k = k+1;}
          context.absChange.at(k) = 255;
          context.absChange.at(k-1) = 255;
          context.absChange.at(k-2) = 255; 
      }

    }


    //dealocation of dynamic memory
    delete pDrawBox;
    buffer = context.absChange; 
    }
	
}