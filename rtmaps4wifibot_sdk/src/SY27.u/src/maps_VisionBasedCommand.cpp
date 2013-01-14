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
    MAPS_PROPERTY("pRho0",90,false,false)
	MAPS_PROPERTY("pTheta0",MAPS_PI,false,false)
	MAPS_PROPERTY("pSpeed0",12.5,false,false)
	MAPS_PROPERTY("pIntegral",true,false,false)
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
			  5, // Nb of properties
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
	m_integralE = cv::Mat::zeros(2,1,CV_MAT_DOUBLE); // values initialised to zero
}

void MAPSVisionBasedCommand::Core() 
{
    
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
		MAPSStreamedString sstr;
		sstr << "Synchronized reading aborted by timeout : synchronisation tolerance mulitplide by 2 => " << m_synch_tolerance;
		ReportInfo(MAPSString(sstr));
		return;
	}
	else if (m_ioElts[0] == NULL || m_ioElts[1] == NULL)
		return;

	m_rho = m_ioElts[0]->Integer();
	m_theta = m_ioElts[1]->Float();

	MAPSStreamedString iStr;
	iStr << "INPUTS (from HoughLines) : Rho = " << m_rho << " | Theta = " << m_theta;
	ReportInfo(MAPSString(iStr));

	// now we have to translate houglines polar coordinate (from upper-left corner) to our polar coordinate (center of the image)

	// due to the polar coordinate we have to
    // take the sign of the radius into account
    // in order to not disturb the command calculation
    if(m_rho < 0) {
        m_rho *= -1;
        m_theta -= MAPS_PI;
    }

    // it's the same for the multiple value of theta
    // which could point out the same position
    // so we have to use the modulo of PI
    // (we get an result within [-PI; PI]
    if(m_theta > 0) {
        m_theta = fmod(m_theta+MAPS_PI, 2*MAPS_PI)-MAPS_PI;
    }
    else {
        m_theta = fmod(m_theta-MAPS_PI, 2*MAPS_PI)+MAPS_PI;
    }
        
    // line equation (the normal) : x*cos(?) + y*sin(?) = r
    
	//the image size is 320x140, so the center is at :
    int w = 159;
    int h = 69;
        
    //compute the crossing point between the line and the Y axis (i.e the middle-width of the image)
    int xOrd = w;
    int yOrd = (m_rho-xOrd*cos(m_theta))/sin(m_theta);
        
    //compute the crossing point between the line and the X axis (i.e the middle-height of the image)
    int yAbs = h;
    int xAbs = (m_rho-yAbs*sin(m_theta))/cos(m_theta);
        
    // then we compute the image of the origine (0,0) on this line
    /* 
        * http://www.exaflop.org/docs/cgafaq/cga1.html#Subject%201.02:%20How%20do%20I%20find%20the%20distance%20from%20a%20point%20to%20a%20line? 
        * 
        * Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
        * The length of the line segment AB is L:
        * 
        * L= sqrt( (Bx-Ax)^2 + (By-Ay)^2 )
        * 
        * Let P be the point of perpendicular projection of C onto AB.
        * Let r be a parameter to indicate P's location along the line containing AB:
        * 
        *      (Ay-Cy)(Ay-By)-(Ax-Cx)(Bx-Ax)
        * r = -----------------------------
        *                  L^2
        * 
        * The point P can then be found:
        * Px = Ax + r(Bx-Ax)
        * Py = Ay + r(By-Ay)
        */

    float L = sqrt( pow((float) xOrd-xAbs, 2) + pow((float) yOrd-yAbs, 2) );
    float r = ((yAbs-h)*(yAbs-yOrd)-(xAbs-w)*(xOrd-xAbs))/pow(L, 2);
        
    int x = cvRound(xAbs + r*(xOrd-xAbs));
    int y = cvRound(yAbs + r*(yOrd-yAbs));

	 // calulating the point and converting the coordinate
    x = xImgToChart(x, w);
    y = yImgToChart(y, h);
       
    //recalculing the good polar coordinate       
    m_rho = sqrt((float) x*x+y*y); //squareroot(x²+y²)
    m_theta = atan2((float) y,(float) x); // arctan(y/x)

	MAPSStreamedString icStr;
	icStr << "INPUTS (converted) : Rho = " << m_rho << " | Theta = " << m_theta;
	ReportInfo(MAPSString(icStr));

	//ReportInfo("Process error...");
    cv::Mat E = getError(m_rho, m_theta);
	//ReportInfo("Process command...");
    cv::Mat Speed = getCommand(m_LTplus, m_B, E, m_K);
	//ReportInfo("Process outputs conversion...");
	cv::Mat Voltage = getVoltage(Speed);

	MAPSStreamedString oStr;
	oStr << "OUTPUTS : Left set point = " << (int) (Voltage.at<double>(0,0)*10) << " | Right set point = " << (int) (Voltage.at<double>(1,0)*10);
	ReportInfo(MAPSString(oStr));

	//The chosen timestamp is the oldest of the 2 received MAPSIOElts. (if _synch_tolerance is 0, then the 2 MAPSIOElt timestamps are equal).
	//Generate the outputs
	MAPSIOElt* ioEltOut = StartWriting(Output("oLeft_speed"));
	ioEltOut->Integer(0) = (int) (Voltage.at<double>(0,0)*10);
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = t; 
	StopWriting(ioEltOut);

	ioEltOut = StartWriting(Output("oRight_speed"));
	ioEltOut->Integer(0) = (int) (Voltage.at<double>(1,0)*10);
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = t;
	StopWriting(ioEltOut);
}

void MAPSVisionBasedCommand::Death()
{

}

cv::Mat MAPSVisionBasedCommand::getCommand(cv::Mat& LTplus, cv::Mat& B, cv::Mat& E, cv::Mat& K) {
    
	//ReportInfo("[Process command] Process integral...");
	MAPSStreamedString eStr;
	eStr << "[Command] Error = [" << E.at<double>(0,0) << ", " << E.at<double>(1,0) << "]";
	ReportInfo(MAPSString(eStr));

    if(GetBoolProperty("pIntegral") == true) {
		m_integralE += E;
	
		MAPSStreamedString ieStr;
		ieStr << "[Command] Integral = [" << m_integralE.at<double>(0,0) << ", " << m_integralE.at<double>(1,0) << "]";
		ReportInfo(MAPSString(ieStr));
	}

	//ReportInfo("[Process command] Process kinematic torsor...");
    //kinematic torsor
    cv::Mat T(6,1,CV_MAT_DOUBLE);
    T = LTplus*B*(E+K.mul(m_integralE));
    
    double phi = T.at<double>(4,0); // == rotation around y axis
    
	//ReportInfo("[Process command] Process angular speed...");
    //angular speed (rad/s)
    cv::Mat Omega = cv::Mat::ones(2,1,CV_MAT_DOUBLE);
    Omega.at<double>(0,0) = (2*m_speed0 - WIFIBOT_WHEEL_SPREADING*phi)/(2*WIFIBOT_WHEEL_RADIUS); // left wheels
    Omega.at<double>(1,0) = (2*m_speed0 + WIFIBOT_WHEEL_SPREADING*phi)/(2*WIFIBOT_WHEEL_RADIUS); // right wheels
	
	//MAPSStreamedString oStr;
	//oStr << "[Command] Omega = [" <<  Omega.at<double>(0,0) << ", " << Omega.at<double>(1,0) << "]";
	//ReportInfo(MAPSString(oStr));

	//ReportInfo("[Command] Process speed...");
	//speed (rpm)
    cv::Mat Speed;
    Omega.copyTo(Speed);
    Speed *= 60/(2*MAPS_PI);

    //should be unecessary because the RTMaps block for controlling the Wifibot (wifibot_serial) is doing this checking already
	if(abs(Speed.at<double>(0,0)) > WIFIBOT_MOTOR_MAX_SPEED) Speed.at<double>(0,0) = WIFIBOT_MOTOR_MAX_SPEED * (Speed.at<double>(0,0) > 0 ? 1 : -1);
    if(abs(Speed.at<double>(1,0)) > WIFIBOT_MOTOR_MAX_SPEED) Speed.at<double>(1,0) = WIFIBOT_MOTOR_MAX_SPEED * (Speed.at<double>(1,0) > 0 ? 1 : -1);
    
	MAPSStreamedString sStr;
	sStr << "[Command] Speed = [" <<  Speed.at<double>(0,0) << ", " << Speed.at<double>(1,0) << "]";
	ReportInfo(MAPSString(sStr));

    return Speed; // return in rpm
}

cv::Mat MAPSVisionBasedCommand::getError (const int rho, const double theta) {
    
    cv::Mat E(2,1,CV_MAT_DOUBLE);
    
    E.at<double>(0,0) = theta - m_theta0;
    E.at<double>(1,0) = rho - m_rho0;
    
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

cv::Mat MAPSVisionBasedCommand::getVoltage(cv::Mat& Speed) {
    
    return Speed*WIFIBOT_MOTOR_U_MAX/WIFIBOT_MOTOR_MAX_SPEED; // return in Volt
}

double MAPSVisionBasedCommand::cos2(double x) {
     return (1 + cos(2*x))/2;
}

double MAPSVisionBasedCommand::sin2(double x) {
     return (1 - cos(2*x))/2;
}

int MAPSVisionBasedCommand::xImgToChart(const int x, const unsigned int w) {
    return x-floor((float) w/2)+1;
}

int MAPSVisionBasedCommand::yImgToChart(const int y, const unsigned int h) {
    return  floor((float) h/2)-1-y;
}