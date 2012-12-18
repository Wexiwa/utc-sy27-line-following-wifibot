// Include onpencv
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#define __IPL_H__ 
#include "maps.hpp" 

#include <algorithm>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

// Includes maps sdk library header
#include "maps.hpp"

class MAPSLineFinding : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLineFinding)
private :
	// Place here your specific methods and attributes

	//outputs
	int rho;
	int theta;

	//inputs
	//IplImage *img;

	//local variables
	cv::Mat img; // image from file
	cv::Mat cut; //cutted image for the Region of Interest
    cv::Mat bw; // Black and white image for threshold filter
    cv::Mat canny; // image for canny filter	
	cv::Mat withLines; //collored image with line draw on it (for testing)
	
	cv::Vec2f oldLine; // variable to store the last line found

	cv::Rect ROI; //region of interest for cutting

	char filename[200];//string to save the adress of the image for testing
	int i;

	int MAPSLineFinding::findLine (Mat img, Vec2f *finalLine );
	cv::Vec2f MAPSLineFinding::findMedian(vector<Vec2f> lines, int counter);
	bool MAPSLineFinding::compareLines(Vec2f i,Vec2f j);

};

struct myclass {
  bool operator() (cv::Vec2f i,cv::Vec2f j){ //function used to compare 2 lines so sort function can work and we can find the median
	double theta1 = i[1];
	double theta2 = j[1];
	double alpha1,alpha2; //angles that will be effectivly be used for comparing, it's the angle of the line with the lower part of the image
	if((theta1<CV_PI/2 && theta1>-CV_PI/2) || (theta1>CV_PI  && theta1<3*CV_PI/2))
			alpha1 = abs(theta1+CV_PI/2);

	else
		alpha1 = abs(theta1-CV_PI/2);
		
	if(theta2<CV_PI/2 && theta2>-CV_PI/2 || (theta2>CV_PI  && theta2<3*CV_PI/2))
			alpha2 = abs(theta2+CV_PI/2);
		
	else
			alpha2 = abs(theta2-CV_PI/2);

	return (alpha1<alpha2);
		

	
	
}
} myobject;