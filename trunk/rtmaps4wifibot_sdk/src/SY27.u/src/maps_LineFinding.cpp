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
	//MAPS_OUTPUT("oResultImage",MAPS::IplImage,NULL,NULL,1)
	MAPS_OUTPUT("linesObjects",MAPS::DrawingObject,NULL,NULL,MAX_DOBJS)
    MAPS_OUTPUT("edgesImage",MAPS::IplImage,NULL,NULL,0)
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
			  4, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSLineFinding::Birth()
{
	OpenCV_ResetErrors();
	m_Storage = cvCreateMemStorage(0);
	m_FirstTime = true;
	m_GrayImage.imageData = NULL;

	//region of interest for cutting the image
	m_ROI.x = 0;
	m_ROI.y = 90;
	m_ROI.width = OUT_IMAGE_W;
	m_ROI.height = OUT_IMAGE_H;
}

void MAPSLineFinding::Core() 
{
	double theta = 0 ;
	int rho = 0;

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

	//parameters for the Houghlines function
	double rhoRes = 1;
	double thetaRes = MAPS_PI/180;
	int threshold = 30;
	double param1=0.0, param2=0.0;
	cvClearMemStorage( m_Storage );
	
	if (m_FirstTime) {//condition for first time execution
		m_FirstTime = false;
		if (*(int*)iIPLImage.channelSeq != MAPS_CHANNELSEQ_GRAY) {
			Error("This component only accepts GRAY images on its input. (8 bpp)");
		}
		m_GrayImage = MAPS::IplImageModel(OUT_IMAGE_W,OUT_IMAGE_H,MAPS_CHANNELSEQ_GRAY);
		Output("edgesImage").AllocOutputBufferIplImage(m_GrayImage);
		m_GrayImage.imageData = NULL;
	}

	//ReportInfo("Processing image...");
	//ReportInfo("[Process] ROI...");

	IplImage& m_SubImg = iIPLImage; //cut image for the Region of Interest
	cvSetImageROI(&m_SubImg, m_ROI);

	//parameters for canny filter (edge detecting)
	int edgesThreshold1 = 100;
	int edgesThreshold2 = 300;
	int edgesAperture = 3;

	/* Start Writing edges image ****************************************************************/
	
	//ReportInfo("[Output] Start Writing Edges Image...");

	IplImage* edgesImagePtr;
	MAPSIOElt* ioEltW2;
	ioEltW2 = StartWriting(Output("edgesImage"));
	IplImage& edgesImage = ioEltW2->IplImage();
	edgesImagePtr = &edgesImage;

	//ReportInfo("[Process] Canny...");	
	//calculating edges image
	cvCanny(&m_SubImg,edgesImagePtr,edgesThreshold1,edgesThreshold2,edgesAperture);

	//ReportInfo("[Process] Hough lines...");
	// hough lines function call
	CvSeq* lines = cvHoughLines2(edgesImagePtr,m_Storage,CV_HOUGH_STANDARD,rhoRes,thetaRes,threshold,param1,param2);

	cvResetImageROI(&m_SubImg);
	ioEltW2->Timestamp() = iTimestamp;
	StopWriting(ioEltW2);

	/* End Writing edges image ******************************************************************/

	/* Start Writing object *********************************************************************/

	MAPSIOElt* ioEltW = StartWriting(Output("linesObjects"));

	//ReportInfo("[Process] Founding lines...");
	//best line found of all the lines found by hough lines
	cv::Vec2f foundLine; 
	//choses the bet line among all found by hough.
	int found = findLine(lines,&foundLine);		

	//in case no line was found, take parameters from previous line
	if(found == 0) 
	{
		//taking parameters fron ancient line
		rho = oldLine[0];
		theta = oldLine[1];

		ReportInfo("[Process][Lines] No line found...");
	}
	else
	{
		//if we found a line we save it in an variable in case we don't find one in the next event, and we have an good
		// estimative for the next line
		oldLine = foundLine;
		rho = foundLine[0];
		theta = foundLine[1];
		
		ReportInfo("[Process][Lines] OK, line found...");
	}

	//ReportInfo("[Output] Start Drawing Object...");
	//writing line in the object output.
	MAPSDrawingObject& dobj = ioEltW->DrawingObject(0);
	dobj.kind = MAPSDrawingObject::Line;
	dobj.color = MAPS_RGB(255,0,0);
	dobj.width = 1;

	double a = cos(theta), b = sin(theta);

	double x0 = a*rho, y0 = b*rho;
	dobj.line.x1 = cvRound(x0 + 1000*(-b));
	dobj.line.y1 = cvRound(y0 + 1000*(a));
	dobj.line.x2 = cvRound(x0 - 1000*(-b));
	dobj.line.y2 = cvRound(y0 - 1000*(a));

	ioEltW->VectorSize() = 1;
	ioEltW->Timestamp() = iTimestamp;

	StopWriting(ioEltW);

	/* End Writing object ***********************************************************************/

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
	ioEltOut->Float(0) = theta;
	ioEltOut->VectorSize() = 1;
	ioEltOut->Timestamp() = iTimestamp;
	StopWriting(ioEltOut);
	
}

void MAPSLineFinding::Death()
{
	if (m_GrayImage.imageData != NULL)
		delete [] m_GrayImage.imageData;
	m_GrayImage.imageData = NULL;
	OpenCV_ReleaseErrors();
}

cv::Vec2f MAPSLineFinding::findMedian(vector<Vec2f> lines, int counter){
	sort(lines.begin(),lines.begin() + counter, myobject); //sort the lines using the function compareLines

	return lines[(int) counter/2];

}


int MAPSLineFinding::findLine (CvSeq* lines, Vec2f *finalLine){ // receives an image after it's been filtered by canny's filter.
										// returns 0 if no line has been found
		
		int lineCounter=0;				//counter for good lines to calculate de average
		
		std::vector<cv::Vec2f> goodLines(lines->total,0); //vector used for saving the lines with an good inclination

		for( size_t i = 0; i < lines->total; i++ ) //loop to find one good line out of the many calculated by houghLines
		{
			float* line = (float*)cvGetSeqElem(lines,i);
		    double  theta = line[1];
			double rho = line[0];
			//condition to see if line is vertical enough
			if((theta< 0.2*CV_PI && theta>  -0.2*CV_PI) ||(theta< 1.2*CV_PI && theta>  0.8*CV_PI) || (theta<- 0.8*CV_PI && theta > - 1.2*CV_PI)){ // if line is vertical enough
				goodLines[lineCounter][0] = rho;
				goodLines[lineCounter][1] = theta;
												
				lineCounter++;
					
		}	
    }	

		if(lineCounter == 0) //no good line found
			return 0; // code for when no lines are found
		
		//in case only one line was found, return this line, to avoid errors in sorting function
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
