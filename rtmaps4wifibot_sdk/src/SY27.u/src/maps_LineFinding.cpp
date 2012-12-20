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
    //img = cv::imread("C:\\Users\\Marcelo\\Documents\\SY27\\data4\\image\\image-0.png",0);
	cut = cv::Mat(Size(160 , 140),img.depth(), 3 );
	bw = cv::Mat(Size(160 , 140),img.depth(), 3);
	canny= cv::Mat(Size(160 , 140),img.depth(), 3 );
	ROI = cv::Rect( 0,90, 320,140 );
	//i=1;
	/*
	// The first time we get an image data, we have to allocate the output buffers
	// To allocate output buffer, we first define the model of images (dimensions and color)
	IplImage model;
	model=MAPS::IplImageModel(canny.size().width,canny.size().height,MAPS_COLORMODEL_RGB);
	//model=MAPS::IplImageModel(640,480,MAPS_COLORMODEL_YUV);
	// Then we call AllocOutputBufferIplImage method of the concerned output
	Output("oResultImage").AllocOutputBufferIplImage(model);
	*/
	m_Storage = cvCreateMemStorage(0);
	m_FirstTime = true;
	m_GrayImage.imageData = NULL;
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

	/*
	cvSaveImage("C:\\Users\\Maxime\\Desktop\\nand\\foo.png",&iIPLImage);
	ReportInfo("Converting IplImage to Mat...");
	cv::Mat img = cv::Mat(&iIPLImage, true); //cvarrToMat(&iIPLImage);
	//IplImage i = img;
	*/

	//cvSaveImage("C:\\Users\\Maxime\\Desktop\\nand\\foo.png",&i);
	//cv::imwrite("C:\\Users\\Maxime\\Desktop\\nand\\foo2.png", img);

	/*
	while(!img.empty())
	{
	*//*
		ReportInfo("Processing image...");
		ReportInfo("\t[Process] ROI...");
		cut = img(ROI);						//geting region of interest (cutting upper part and corners

		ReportInfo("\t[Process] Treshold...");
		cv::threshold(cut,bw,200,255, CV_THRESH_BINARY);  // converting to 2 levels image
		/*
		ReportInfo("\t[Process] Canny...");
		cv::Canny(bw,canny,100,300,3);					  // canny filter
		
		//cv::cvtColor(canny, withLines, CV_GRAY2BGR);		  // image that will have the lines draw in color for testing
		
		ReportInfo("\t[Process] Founding lines...");
		cv::Vec2f foundLine; 
		int found = findLine(canny,&foundLine);			   //applies HoughLines and chose the best line among all the found in the image
		
		if(found == 0){ // when no line is found use the last found line for calculation
			rho = oldLine[0];
			theta = oldLine[1] ;
			
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
		*/
		
		/*
		sprintf(filename,"C:\\Users\\Marcelo\\Documents\\SY27\\data4\\image\\image-%d.png",i); //calculating new filename
		img = cv::imread(filename, 0);
		i++;

		if(i>=518) break;
		char c= cv::waitKey(1);					  // frame rate defined for testing a sequence of images
		if(c==27) break;						  //stop when ESC is pressed
	}						  //stop when ESC is pressed

	*/

	double rhoRes = 1;
	double thetaRes = MAPS_PI/180;
	int threshold = 30;
	double param1=0.0, param2=0.0;
	cvClearMemStorage( m_Storage );
	
	if (m_FirstTime) {
		m_FirstTime = false;
		if (*(int*)iIPLImage.channelSeq != MAPS_CHANNELSEQ_GRAY) {
			Error("This component only accepts GRAY images on its input. (8 bpp)");
		}
		m_GrayImage = MAPS::IplImageModel(iIPLImage.width,iIPLImage.height,MAPS_CHANNELSEQ_GRAY);
		if (m_OutputEdges) {
			Output(1).AllocOutputBufferIplImage(m_GrayImage);
			m_GrayImage.imageData = NULL;
		} else {
			m_GrayImage.imageData = new char[m_GrayImage.imageSize];
		}
	}

	int edgesThreshold1 = 100;
	int edgesThreshold2 = 300;
	int edgesAperture = 3;

	IplImage* edgesImagePtr;
	MAPSIOElt* ioEltW2;
	if (m_OutputEdges) {
		ioEltW2 = StartWriting(Output("edgesImage"));
		ioEltW2->Timestamp() = iTimestamp;
		IplImage& edgesImage = ioEltW2->IplImage();
		edgesImagePtr = &edgesImage;
	} else {
		edgesImagePtr = &m_GrayImage;
	}

	//ReportInfo("Processing image...");
	//ReportInfo("\t[Process] ROI...");
	CvRect roi;
	roi.x = 0;
	roi.y = 90;
	roi.width = 320;
	roi.height = 140;
	getSubImg(&iIPLImage, &iIPLImage, roi);

	//ReportInfo("\t[Process] Canny...");	
	cvCanny(&iIPLImage,edgesImagePtr,edgesThreshold1,edgesThreshold2,edgesAperture);

	//ReportInfo("\t[Process] Hough lines...");
	CvSeq* lines = cvHoughLines2(edgesImagePtr,m_Storage,m_Method,rhoRes,thetaRes,threshold,param1,param2);


	//ReportInfo("\t[Process] Founding lines...");
	cv::Vec2f foundLine; 
	int found = findLine(lines,&foundLine);			   //applies HoughLines and chose the best line among all the found in the image

	//ReportInfo("\t[Output] Lines Objects...");
	MAPSIOElt* ioEltW = StartWriting(Output("linesObjects"));
		
	if(found == 0) 
	{
		rho = oldLine[0];
		theta = oldLine[1] ;

		ReportInfo("\t\t[Process][Lines] No line found...");
	}
	else
	{
		//if we found a line we save it in an variable in case we don't find one in the next event, and we have an good
		// estimative for the next line
		oldLine = foundLine;
		rho = foundLine[0];
		theta = foundLine[1] ;
		
		ReportInfo("\t\t[Process][Lines] OK, line found...");
	}

	MAPSDrawingObject& dobj = ioEltW->DrawingObject(1);
	dobj.kind = MAPSDrawingObject::Line;
	dobj.color = MAPS_RGB(255,0,0);
	dobj.width = 1;

	double a = cos(theta), b = sin(theta);
	if( fabs(a) < 0.001 )
	{
		dobj.line.x1 = dobj.line.x2 = cvRound(rho);
		dobj.line.y1 = 0;
		dobj.line.y2 = iIPLImage.height;
	}
	else if( fabs(b) < 0.001 )
	{
		dobj.line.y1 = dobj.line.y2 = cvRound(rho);
		dobj.line.x1 = 0;
		dobj.line.x2 = iIPLImage.width;
	}
	else
	{
		dobj.line.x1 = 0;
		dobj.line.y1 = cvRound(rho/b);
		dobj.line.x2 = cvRound(rho/a);
		dobj.line.y2 = 0;
	}

	ioEltW->VectorSize() = 1;
	ioEltW->Timestamp() = iTimestamp;

	StopWriting(ioEltW);

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
	
	/*
	// Ask for a new output data
	MAPSIOElt* oIOEltImage=StartWriting(Output("oResultImage"));	
	oIOEltImage->Timestamp()=iTimestamp;

	//Convert to IplImage
	IplImage ipl = cut;
	IplImage &oIPLImage=oIOEltImage->IplImage();

	// Get pointers to the first pixel of input and output images
	unsigned char *ib=(unsigned char*)ipl.imageData;
	unsigned char *ob=(unsigned char*)oIPLImage.imageData;
	int u,v;
	for(v=0;v<iIPLImage.height;v++)
	{
		// Get pointers to the first pixel of line number v of input and output images
		unsigned char *ibb=ib+iIPLImage.widthStep*v;
		unsigned char *obb=ob+oIPLImage.widthStep*v;
		for(u=0;u<iIPLImage.width;u++)
		{
			obb[u]=ibb[u];
		}		
	}

	StopWriting(oIOEltImage);
	*/
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
	img.release();
	cut.release();
	bw.release();
	canny.release();
    if (m_GrayImage.imageData != NULL)
		delete [] m_GrayImage.imageData;
	m_GrayImage.imageData = NULL;
}

/*
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

*/

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
			if((theta< 0.2*CV_PI && theta>  -0.2*CV_PI) ||(theta< 1.2*CV_PI && theta>  0.8*CV_PI) || (theta<- 0.8*CV_PI && theta > - 1.2*CV_PI)){ // if line is vertical enough
				goodLines[lineCounter][0] = rho;
				goodLines[lineCounter][1] = theta;
												
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

void MAPSLineFinding::getSubImg(IplImage* img, IplImage* subImg, CvRect roiRect) {
 
	cvSetImageROI(img, roiRect);
	subImg = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvCopy(img, subImg, NULL);
	cvResetImageROI(img);
 
}
