////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module : Compute the wheels speed by using visual information of a line to follow (represented with polar coordinate {rho, theta} )
////////////////////////////////

#include "maps_VisionBasedCommand.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSVisionBasedCommand)
    MAPS_INPUT("iRho",MAPS::FilterInteger,MAPS::LastOrNextReader)
    MAPS_INPUT("iTheta",MAPS::FilterFloat,MAPS::LastOrNextReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSVisionBasedCommand)
    MAPS_OUTPUT("oLeft_speed",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("oRight_speed",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSVisionBasedCommand)
    MAPS_PROPERTY("pRho0",0,false,false)
	MAPS_PROPERTY("pTheta0",0.0,false,false)
	MAPS_PROPERTY("pSpeed0",12.5,false,false)
	MAPS_PROPERTY("synch_tolerance",100,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSVisionBasedCommand)
    //MAPS_ACTION("aName",MAPSVisionBasedCommand::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (VisionBasedCommand) behaviour
MAPS_COMPONENT_DEFINITION(MAPSVisionBasedCommand,"VisionBasedCommand","1.0",128,
			  MAPS::Sequential|MAPS::Threaded,MAPS::Threaded,
			  2, // Nb of inputs
			  2, // Nb of outputs
			  4, // Nb of properties
			  0) // Nb of actions

void MAPSVisionBasedCommand::Birth()
{
	m_speed_left = m_speed_right = 0;
	m_rho0 = GetIntegerProperty("pRho0");
	m_theta0 = GetFloatProperty("pTheta0");
	m_speed0 = GetFloatProperty("pSpeed0");
	m_synch_tolerance = GetIntegerProperty("synch_tolerance");

	m_rho = m_theta = 0;
	m_B = getProportional();
    m_K = getIntegral();
    m_LTplus = getInteractionMatrixPseudoInverse();
}

void MAPSVisionBasedCommand::Core() 
{
    // Reports this information to the RTMaps console
    ReportInfo("VisionBasedCommand: Passing through Core() method");

	//reading the inputs
	m_inputs[0] = &Input("iRho");
	m_inputs[1] = &Input("iTheta");

	//setting the timer for synchro input reading method
	MAPSEvent timeout;
	timeout.SetTrigger(m_synch_tolerance*1000);

	//Blocks until 2 ioElts are found (1 on each input) with the same
	//timestamp +/- the synchronization tolerance.
	MAPSTimestamp t = SynchroStartReading(2, m_inputs, m_ioElts, m_synch_tolerance, &timeout);
	if (t == -1 || m_ioElts == NULL){ //timeout
		m_synch_tolerance *= 2;
		ReportInfo(MAPSString("Synchronized reading aborted by timeout : synchronisation tolerance mulitplide by 2 = ") + MAPSString(m_synch_tolerance));
		return;
	}
	else if (m_ioElts[0] == NULL || m_ioElts[1] == NULL)
		return;

	m_rho = m_ioElts[0]->Integer();
	m_theta = m_ioElts[1]->Float();

    cv::Mat E = getError(m_rho, m_theta);
    cv::Mat Speed = getCommand(m_LTplus, m_B, m_E, m_K);
	cv::Mat Voltage = getVoltage(Speed);

	//The chosen timestamp is the oldest of the 2 received MAPSIOElts. (if _synch_tolerance is 0, then the 2 MAPSIOElt timestamps are equal).
	//Generate the outputs
	MAPSIOElt* ioEltOut = StartWriting(Output("oLeft_speed"));
	ioEltOut->Integer(0) = (int) Voltage.at<double>(0,0);
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = t; 
	StopWriting(ioEltOut);

	ioEltOut = StartWriting(Output("oRight_speed"));
	ioEltOut->Integer(0) = (int) Voltage.at<double>(1,0);
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = t;
	StopWriting(ioEltOut);
}

void MAPSVisionBasedCommand::Death()
{

}

cv::Mat MAPSVisionBasedCommand::getCommand(cv::Mat LTplus, cv::Mat B, cv::Mat E, cv::Mat K) {
    
    static cv::Mat integralE = cv::Mat::zeros(2,1,CV_MAT_DOUBLE);
    
    integralE += E;

    //kinematic torsor
    cv::Mat T(6,1,CV_MAT_DOUBLE);
    T = LTplus*B*(E+K.mul(integralE));
    
    double phi = T.at<double>(4,0); // == rotation around y axis
    
    //angular speed (rad/s)
    cv::Mat Omega = cv::Mat::ones(2,1,CV_MAT_DOUBLE);
    Omega.at<double>(0,0) = (2*m_speed0 - WIFIBOT_WHEEL_SPREADING*phi)/(2*WIFIBOT_WHEEL_RADIUS); // left wheels
    Omega.at<double>(1,0) = (2*m_speed0 + WIFIBOT_WHEEL_SPREADING*phi)/(2*WIFIBOT_WHEEL_RADIUS); // right wheels

	//speed (rpm)
    cv::Mat Speed;
    Omega.copyTo(Speed);
    Speed *= 60/(2*MAPS_PI);

    //unecessary because the RTMaps block for controlling the Wifibot (wifibot_serial) is doing this cheking already
	//if(abs(Speed.at<double>(0,0)) > WIFIBOT_MOTOR_MAX_SPEED) Speed.at<double>(0,0) = WIFIBOT_MOTOR_MAX_SPEED * (Speed.at<double>(0,0) > 0 ? 1 : -1);
    //if(abs(Speed.at<double>(1,0)) > WIFIBOT_MOTOR_MAX_SPEED) Speed.at<double>(1,0) = WIFIBOT_MOTOR_MAX_SPEED * (Speed.at<double>(1,0) > 0 ? 1 : -1);
    
    return Speed; // return in rpm
}

cv::Mat MAPSVisionBasedCommand::getError (const unsigned int rho, const double theta) {
    
    cv::Mat E(2,1,CV_MAT_DOUBLE);
    
    E.at<double>(0,0) = theta - m_theta0;
    E.at<double>(1,0) = fmod(rho, 2*MAPS_PI) - fmod(m_rho0, 2*MAPS_PI);
    
    return E;
}

cv::Mat MAPSVisionBasedCommand::getProportional () {

    cv::Mat B = cv::Mat::zeros(2,2,CV_MAT_DOUBLE);
    
    B.at<double>(0,0) = -1*LAMBDA*BETA;
    B.at<double>(1,1) = -1*LAMBDA;
    
    return B;
}

cv::Mat MAPSVisionBasedCommand::getIntegral() {
    cv::Mat K(2,1,CV_MAT_DOUBLE);
    
    K.at<double>(0,0) = K1;
    K.at<double>(1,0) = K2;
    
    return K;
}

cv::Mat MAPSVisionBasedCommand::getInteractionMatrixPseudoInverse () {
    
    cv::Mat LTplus = cv::Mat::zeros(6,2,CV_MAT_DOUBLE);

	unsigned int wch2 = pow((float)WIFIBOT_CAM_HEIGHT,2);
    
    LTplus.at<double>(0,0) = -1*cos(WIFIBOT_CAM_ALPHA)*WIFIBOT_CAM_HEIGHT/(1+wch2);
    LTplus.at<double>(0,1) = sin(WIFIBOT_CAM_ALPHA)*WIFIBOT_CAM_HEIGHT/(1+wch2);
    LTplus.at<double>(4,0) = -1*cos(WIFIBOT_CAM_ALPHA)*sin(WIFIBOT_CAM_ALPHA)/(1+wch2);
    LTplus.at<double>(4,1) = -1*(cos2(WIFIBOT_CAM_ALPHA)+wch2)/(1+wch2);
    LTplus.at<double>(5,0) = -1*(sin2(WIFIBOT_CAM_ALPHA)+wch2)/(1+wch2);
    LTplus.at<double>(5,1) = -1*cos(WIFIBOT_CAM_ALPHA)*sin(WIFIBOT_CAM_ALPHA)/(1+wch2);
    
    return LTplus;
}

cv::Mat MAPSVisionBasedCommand::getVoltage(cv::Mat Speed) {
    
    return Speed*WIFIBOT_MOTOR_U_MAX/WIFIBOT_MOTOR_MAX_SPEED; // return in Volt
}

double MAPSVisionBasedCommand::cos2(double x) {
     return (1 + cos(2*x))/2;
}

double MAPSVisionBasedCommand::sin2(double x) {
     return (1 - cos(2*x))/2;
}
