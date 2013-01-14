////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LineFinding_H
#define _Maps_LineFinding_H

// Include onpencv
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "maps_OpenCV_ErrorHandling.h"
#define __IPL_H__
// Includes maps sdk library header
#include "maps.hpp"

#define MAX_DOBJS   1
#define OUT_IMAGE_H 140
#define OUT_IMAGE_W 320

using namespace cv;
using namespace std;

// Declares a new MAPSComponent child class
class MAPSLineFinding : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLineFinding)
private :

	// Place here your specific methods and attributes

	//from opencv4Rtmaps
    bool m_FirstTime;
    CvMemStorage* m_Storage;
    IplImage	  m_GrayImage;

	//local
	CvRect m_ROI;

	//outputs
	int rho;
	int theta;

	//local variables

	cv::Vec2f oldLine; // variable to store the last line found

	int MAPSLineFinding::findLine (CvSeq* lines, Vec2f *finalLine);
	cv::Vec2f MAPSLineFinding::findMedian(vector<Vec2f> lines, int counter);
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

#endif
