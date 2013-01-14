////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_VisionBasedCommand_H
#define _Maps_VisionBasedCommand_H

// Include onpencv
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#define __IPL_H__
// Includes maps sdk library header
#include "maps.hpp"

#define APP_NAME "WifiBot"

//Regulation
//      Proportionnal : dErreur = -LAMBDA * (BETA * ErreurTHETA + ErreurRADIUS)
#define LAMBDA 1 // The Proportionnal gain : Real number > 0
#define BETA 1 // Allows to change the "priority" between lateral error and direction
//      Integral
#define K1 1 // The first Integral constant : Real number > 0
#define K2 1 // The second Integral constant : Real number > 0

//Wifibot spec
#define WIFIBOT_WHEEL_N 4
#define WIFIBOT_WHEEL_RADIUS 72 // mm (145 for diameter)
#define WIFIBOT_WHEEL_SPREADING 300 // mm 220 (inside) / 300 (average) / 380 (outside) - 170 between front and back
#define WIFIBOT_MOTOR_N 4 // of motors
#define WIFIBOT_MOTOR_MAX_SPEED 156 // rpm
#define WIFIBOT_MOTOR_U_MAX 24.0 // Volt DC
#define WIFIBOT_CAM_FPS_MAX 30 // fps
#define WIFIBOT_CAM_RES_MAX_WIDTH 1600 // px
#define WIFIBOT_CAM_RES_MAX_HEIGHT 1200 // px
#define WIFIBOT_CAM_HEIGHT 320 // mm, camera's average height (relative to the ground)
#define WIFIBOT_CAM_ALPHA MAPS_PI/4 //radian, camera's angle (relative to horizontal)

//opencv
#define CV_MAT_DOUBLE CV_64F


// Declares a new MAPSComponent child class
class MAPSVisionBasedCommand : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSVisionBasedCommand)
private :
	// Place here your specific methods and attributes

	//outputs
	int m_speed_left;
	int m_speed_right;

	//inputs
	int m_rho;
	float m_theta;

	//parameters
	int m_rho0;
	float m_theta0;
	float m_speed0;
	int m_synch_tolerance;

	//local variables
	MAPSIOElt*	m_ioElts[2];
	MAPSInput* m_inputs[2];

	cv::Mat m_B; // Proportional gain matrix
    cv::Mat m_K; // Integral gain matrix
    cv::Mat m_LTplus; // Interaction Matrix Pseudo Inverse
	cv::Mat m_integralE; // Integral value matrix

	cv::Mat getCommand(cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);
	cv::Mat getError (const int, const double);
	cv::Mat getProportional ();
	cv::Mat getIntegral();
	cv::Mat getInteractionMatrixPseudoInverse();
	cv::Mat getVoltage(cv::Mat&);
	double cos2(double); // == cos²(x)
	double sin2(double); // == sin²(x)
	int xImgToChart(const int, const unsigned int);
	int yImgToChart(const int, const unsigned int);
};

#endif
