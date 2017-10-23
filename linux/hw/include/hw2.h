#ifndef __HW2__
#define __HW2__

#include "hw1.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

template <class U> class Image{
public:
    int height;
    int width;
    vector<U> local_buffer;

public:
    Image<U>(){}
    Image(int x, int y): height(y), width(x){}
    Image(vector<U> &copy_from, int x, int y)
     : height(y), width(x)
    {
    	this->local_buffer=copy_from;
    }
    virtual ~Image<U>(){}
    virtual void populate_image() = 0;
    virtual void print_image() = 0;
    virtual void average_image() = 0;
    virtual void invert_image() = 0;
    virtual int *index_image(int x_val, int y_val) = 0;
    virtual void replace_in_image(int *start, char color, U value) = 0;
    virtual void flip_image() = 0;
    virtual void copy_from(vector<U> &copy_from) = 0;
   
};


template <typename T> 
class CameraImage: public Image<T>{
public:
    CameraImage() : Image<T>(){}
    CameraImage(int x, int y) : Image<T>(x,y){}
    CameraImage(vector<T> &copy_from, int x, int y) : Image<T>(copy_from, x,y){}
    virtual ~CameraImage(){}
    
    
    virtual void populate_image(){
        hw::populate_vector(this->local_buffer);
    }

    virtual void print_image(){
        hw::print_vector(this->local_buffer, this->width, this->height);
    }

    virtual void average_image(){
        hw::average_vector(this->local_buffer);
    }

    virtual void invert_image(){
        hw::invert_vector(this->local_buffer);
    }

    virtual int *index_image(int x_val, int y_val){
        return hw::index_vector(this->local_buffer, this->width, this->height, x_val, y_val);
    }

    virtual void replace_in_image(int *start, char color, T value){
        hw::replace_in_vector(this->local_buffer, this->width, this->height, start, color, value);
    }

    virtual void flip_image(){
        hw::flip_vector(this->local_buffer, this->width, this->height);
    }


	virtual void copy_from(vector<T> &cf) {
 	  this->local_buffer = cf;
    }


	//CameraImage perator<<(const CameraImage&, const CameraImage&); 
	//CameraImage operator<<(ostream& out, const CameraImage& ci){
	//	return out; 
	//}

	friend ostream& operator <<(ostream& os, const CameraImage<T> &object){
	int length = object.width; 
	object.local_buffer;
	//cout << object.local_buffer.size(); 
	os << endl; 
        os << "|| ";
        for(unsigned i=0; i< (unsigned) object.local_buffer.size();i++){
          os<< object.local_buffer.at(i) << " ";   //std::setw(3) << std::setfill('0') 
          if((i+1)%3 ==0){ 
            os <<"|| ";
            if( (i+1)%(3*length)==0){
               os <<  std::endl << "|| ";
            }
          }
       }
    os << std::endl;
	return os;	
  }
friend bool operator <(const CameraImage<T> &img1, const CameraImage<T> &img2){
   long dif = imgDif(img1,img2); 
   return dif<0; 
}


friend bool operator >(const CameraImage<T> &img1, const CameraImage<T> &img2){
   long avgImg1 = 0;
   long avgImg2 = 0; 
   for(int i =0; i<img1.local_buffer.size();i++){
   	avgImg1 = avgImg1+img1.local_buffer.at(i);
	avgImg2 = avgImg2+img2.local_buffer.at(i);
   }
   avgImg1 = avgImg1/img1.local_buffer.size();
   avgImg2 = avgImg2/img1.local_buffer.size();
   return avgImg1>avgImg2; 
}

friend bool operator ==(const CameraImage<T> &img1, const CameraImage<T> &img2){
  long dif = imgDif(img1,img2);  
  return dif==0; 
}

friend bool operator !=(const CameraImage<T> &img1, const CameraImage<T> &img2){
  long dif =0; 
  for(unsigned i =0; i<img1.local_buffer.size(); i++){
    dif = dif+img1.local_buffer.at(i)-img2.local_buffer.at(i);
    //cout<<"dif equals " << dif<<endl; 
  }
  return dif!=0; 
}

friend long imgDif(const CameraImage<T> &img1, const CameraImage<T> &img2){
  long dif =0; 
  for(unsigned i =0; i<img1.local_buffer.size(); i++){
    dif = dif+(img1.local_buffer.at(i)-img2.local_buffer.at(i));
  }
  return dif; 
}

	//TODO copy from!
};

#endif
