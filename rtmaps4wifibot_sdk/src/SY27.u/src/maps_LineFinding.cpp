////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LineFinding.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLineFinding)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::LastOrNextReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLineFinding)
    MAPS_OUTPUT("Rho",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("Theta",MAPS::Float,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLineFinding)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLineFinding)
    //MAPS_ACTION("aName",MAPSLineFinding::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LineFinding) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLineFinding,"LineFinding","1.0",128,
			  MAPS::Sequential|MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  2, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSLineFinding::Birth()
{
    //img = cv::imread("C:\\Users\\Marcelo\\Documents\\SY27\\data4\\image\\image-0.png",0);
	cut = cv::Mat(Size(160 , 140),img.depth(), img.channels() );
	bw = cv::Mat(Size(160 , 140),img.depth(), img.channels() );
	canny= cv::Mat(Size(160 , 140),img.depth(), img.channels() );
	ROI = cv::Rect( 0,90, 320,140 );
	//i=1;
}

void MAPSLineFinding::Core() 
{
	// Get the I/O buffer element containing the new image data
	MAPSIOElt* iIOEltImage=StartReading(Input("iImage"));
	// Check the returned pointer
	if (NULL==iIOEltImage)
		return;
	// Inside this I/O element, we get :
	//		- the IPL image
	IplImage &iIPLImage=iIOEltImage->IplImage();
	//		- the timestamp of the data (time of data origin)
	MAPSTimestamp &iTimestamp=iIOEltImage->Timestamp();
	//		- the time of issue of the data (availability time in RTMaps)
	//MAPSTimestamp &iTimeOfIssue=iIOEltImage->TimeOfIssue();

	ReportInfo("Converting IplImage to Mat...");
	img = cv::Mat(&iIPLImage);

	/*
	while(!img.empty())
	{
	*/
		ReportInfo("Processing image...");
		ReportInfo("\t[Process] ROI...");
		cut = img(ROI);						//geting region of interest (cutting upper part and corners

		ReportInfo("\t[Process] Treshold...");
		cv::threshold(cut,bw,200,255, CV_THRESH_BINARY);  // converting to 2 levels image
		
		ReportInfo("\t[Process] Canny...");
		cv::Canny(bw,canny,100,300,3);					  // canny filter
		
		//cv::cvtColor(canny, withLines, CV_GRAY2BGR);		  // image that will have the lines draw in color for testing
		
		ReportInfo("\t[Process] Foundind lines...");
		cv::Vec2f foundLine; 
		int found = findLine(canny,&foundLine);			   //applies HoughLines and chose the best line among all the found in the image
		
		if(found == 0){ // when no line is found use the last found line for calculation
			double rho = oldLine[0], theta = oldLine[1] ;
			
			cv::Point pt1,pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;

			pt1.x = cvRound(x0 + 1000*(-b));  //calculating points to draw the las found line
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));

			ReportInfo("\t\t[Process][Lines] No line found...");

			//line( withLines, pt1, pt2, Scalar(0,0,255), 2, CV_AA);  //if no line has been found, uses the old one
		}
		else{  //if we found a line we save it in an variable in case we don't find one in the next event, and we have an good
					// estimative for the next line
			oldLine = foundLine;
			double rho = foundLine[0], theta = foundLine[1] ;
		
			cv::Point pt1,pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;

			pt1.x = cvRound(x0 + 1000*(-b));	//calculating points to dwar the found line
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));
			//line( withLines, pt1, pt2, Scalar(0,0,255), 2, CV_AA); //draw line for testing porposes

			ReportInfo("\t\t[Process][Lines] OK, line found...");
		}
		
		/*
		sprintf(filename,"C:\\Users\\Marcelo\\Documents\\SY27\\data4\\image\\image-%d.png",i); //calculating new filename
		img = cv::imread(filename, 0);
		i++;

		if(i>=518) break;
		char c= cv::waitKey(1);					  // frame rate defined for testing a sequence of images
		if(c==27) break;						  //stop when ESC is pressed
	}						  //stop when ESC is pressed

	*/

	MAPSStreamedString oStr;
	oStr << "OUTPUTS : Rho = " << rho << " | Theta = " << theta;
	ReportInfo(MAPSString(oStr));

	//Generate the outputs
	MAPSIOElt* ioEltOut = StartWriting(Output("Rho"));
	ioEltOut->Integer(0) = rho;
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = iTimestamp; 
	StopWriting(ioEltOut);

	ioEltOut = StartWriting(Output("Theta"));
	ioEltOut->Integer(0) = theta;
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = iTimestamp;
	StopWriting(ioEltOut);

	img.release();
	cut.release();
	bw.release();
	canny.release();
}

//bool MAPSLineFinding::compareLines(Vec2f i,Vec2f j){ //function used to compare 2 lines so sort function can work and we can find the median
//	double theta1 = i[1];
//	double theta2 = j[1];
//	double alpha1,alpha2; //angles that will be effectivly be used for comparing, it's the angle of the line with the lower part of the image
//	if((theta1<CV_PI/2 && theta1>-CV_PI/2) || (theta1>CV_PI  && theta1<3*CV_PI/2))
//			alpha1 = abs(theta1+CV_PI/2);
//
//	else
//		alpha1 = abs(theta1-CV_PI/2);
//		
//	if(theta2<CV_PI/2 && theta2>-CV_PI/2 || (theta2>CV_PI  && theta2<3*CV_PI/2))
//			alpha2 = abs(theta2+CV_PI/2);
//		
//	else
//			alpha2 = abs(theta2-CV_PI/2);
//
//	return (alpha1<alpha2);
//		
//
//	
//	
//}

void MAPSLineFinding::Death()
{
    
}

cv::Vec2f MAPSLineFinding::findMedian(vector<Vec2f> lines, int counter){
	sort(lines.begin(),lines.begin() + counter, myobject); //sort the lines using the function compareLines

	return lines[(int) counter/2];

}


int MAPSLineFinding::findLine (Mat img, Vec2f *finalLine ){ // receives an image after it's been filtered by canny's filter.
										// returns 0 if no line has been found
		
		int lineCounter=0;				//counter for good lines to calculate de average
		int x1=0,x2=0,y1=0,y2=0,temp,maxIndex,minIndex;		//accumulators to calculate the average
		vector<Vec2f> lines;

		cv::HoughLines(img,lines,1, CV_PI/90, 34, 0, 0 );  //calling houghLines - line detector function
		std::vector<cv::Vec2f> goodLines(lines.size(),0); //vector used for saving the lines with an good inclination

		for( size_t i = 0; i < lines.size(); i++ ) //loop to find one good line out of the many calculated by houghLines
		{
			
		    double  theta = lines[i][1] ;
			if((theta< 0.2*CV_PI && theta>  -0.2*CV_PI) ||(theta< 1.2*CV_PI && theta>  0.8*CV_PI) || (theta<- 0.8*CV_PI && theta > - 1.2*CV_PI)){ // if line is vertical enough
				goodLines[lineCounter] = lines[i];
				
				lineCounter++;
				
				
					
		}	
    }	

		if(lineCounter == 0) //no good line found
			return 0; // code for when no lines are found
		
		
		if(lineCounter == 1){
			*finalLine = goodLines[0];
			return 1;
		}
		else{//find the  median line (comparing by the angle formed by the line with the lower part of the image
			*finalLine = findMedian(goodLines,lineCounter);
			return 1;
		}

		return 0; //code for when no lines are found
}

