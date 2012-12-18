////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter3 
 * \file
 * \brief MonoVisionIPUSample
 * Your first RTMaps component : the MonoVisionIPUSample component
 * This is a sample of image processing component (IPU means here Image Processing Unit). 
 * It inherits directly from MAPSComponent. Thus this component implements all RTMaps I/O 
 * management. This sample just make a very simple image processing and draw 
 * some 2D objects.
 * To test this component:
 * - use a RandomImage component, a Webcam component or a Player for example to input the images.
 * - use an Overlay drawing component (in section Viewers) to overlay the 2D objects onto the images.
 * - use an Image Viewer to display the results.
*/

////////////////////////////////////
// BEGINNING OF COMPONENT DEFINITION
#define IPU_MAX_DRAWINGOBJECTS 20

#include "maps_MonoVisionIPUSample.h"

// Inputs definition
// We use here a FifoReader. This is the better way not to loose input images
// This is not always a good choice for image processing components
MAPS_BEGIN_INPUTS_DEFINITION(MAPSMonoVisionIPUSample)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Outputs definition
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSMonoVisionIPUSample)
    MAPS_OUTPUT("oImage",MAPS::IplImage,NULL,NULL,1)
    MAPS_OUTPUT("o2DObjects",MAPS::DrawingObject,NULL,NULL,IPU_MAX_DRAWINGOBJECTS)
MAPS_END_OUTPUTS_DEFINITION

// Properties definition. Just for fun..
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSMonoVisionIPUSample)
    MAPS_PROPERTY("radius",50,false,true)
    MAPS_PROPERTY("cx",100,false,true)
    MAPS_PROPERTY("cy",100,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSMonoVisionIPUSample)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSMonoVisionIPUSample,"MonoVisionIPUSample","1.0",128,
			  MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,1,2,3,0)
// END OF COMPONENT DEFINITION
////////////////////////////////////

////////////////////////////////////
// BEGINNING OF COMPONENT IMPLEMENTATION
void MAPSMonoVisionIPUSample::Birth() 
{	
    m_firstTime=true;
}

void MAPSMonoVisionIPUSample::Death() 
{	
}

void MAPSMonoVisionIPUSample::Core() 
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


	// The first time we get an image data, we have to allocate the output buffers
	// Indeed, the size of the output buffer depends on the size of input image
	// (we want here the output image to have the same dimensions than the input image, and to be grayscale	)
	if (m_firstTime)
	{
		// To allocate output buffer, we first define the model of images (dimensions
		// and color)
		IplImage model;
		model=MAPS::IplImageModel(iIPLImage.width,iIPLImage.height,MAPS_COLORMODEL_GRAY);
		// Then we call AllocOutputBufferIplImage method of the concerned output
		Output(0).AllocOutputBufferIplImage(model);
		// And there is only one first time...
		m_firstTime=false;
	}

	///////////////////////////////////
	// Beginning of image processing
	// Ask for a new output data
	MAPSIOElt* oIOEltImage=StartWriting(Output("oImage"));	
	// Inside this I/O element, we get :
	//		- the IPL image
	IplImage &oIPLImage=oIOEltImage->IplImage();
	//		- the timestamp of the data (time of data origin)
	MAPSTimestamp &oTimestamp=oIOEltImage->Timestamp();

	// We can define the same timestamp (time of data origin) for the input and output
	// images... Thus it allows to synchronize raw data and processed ones for fusion
	oTimestamp=iTimestamp;

	// The IPL structure allow a very powerful image description
	// Here we check image format (color model) : please read Intel IPL documentation
	// to get more information
	if ((*(MAPSUInt32*)iIPLImage.colorModel)==MAPS_CHANNELSEQ_RGB)
	{
		// Here we know that input image has a RGB channel sequence
	}
	else if ((*(MAPSUInt32*)iIPLImage.colorModel)==MAPS_CHANNELSEQ_BGR)
	{
		// Here we know that input image has a BGR channel sequence
	}
	else
		Error("iImage input only accept color IPL images (RGB or BGR)");

	// Let's do a very simple processing with the input data
	// Get pointers to the first pixel of input and output images
	unsigned char *ib=(unsigned char*)iIPLImage.imageData;
	unsigned char *ob=(unsigned char*)oIPLImage.imageData;
	int u,v;
	for(v=0;v<iIPLImage.height;v++)
	{
		// Get pointers to the first pixel of line number v of input and output images
		unsigned char *ibb=ib+iIPLImage.widthStep*v;
		unsigned char *obb=ob+oIPLImage.widthStep*v;
		for(u=0;u<iIPLImage.width;u++)
		{
			// And for example, we output a "gray" image such as for each pixel :
			// Gray value = (R Value + G Value + B Value)/3
			obb[u]=(ibb[3*u]+ibb[3*u+1]+ibb[3*u+2])/3;
		}		
	}
	// We could have used the iplColorToGray function of Intel IPL Libray instead...
	//	iplColorToGray(&iIPLImage,&oIPLImage);
	// OK, send the output buffer (and don't write into it any more !)
	StopWriting(oIOEltImage);
	// End of image processing
	///////////////////////////////////

	///////////////////////////////////
	// Beginning of 2D object output
	// Now, write 2D Objects data to the 2nd output
	MAPSIOElt *oIOElt2DObjects=StartWriting(Output("o2DObjects"));
	// Inside this I/O element, we get :
	//		- the 2D Objects data
	MAPSDrawingObject *oDrawingObjects=&(oIOElt2DObjects->DrawingObject(0));
	//		- the timestamp of the data (time of data origin)
	MAPSTimestamp &oTimestamp2=oIOEltImage->Timestamp();
	// We can define the same timestamp (time of data origin) for the input and output
	// images... Thus it allows to synchronize raw data and processed ones for fusion
	oTimestamp2=iTimestamp;
	// Draw something with no interest but being a sample... 
	int oNbObjects=0;
	//		- A blue circle...
	oDrawingObjects[oNbObjects].kind=MAPSDrawingObject::Circle;
	oDrawingObjects[oNbObjects].width=1;
	oDrawingObjects[oNbObjects].color=MAPS_RGB(0,0,255);
	// (be careful, 64 bits integer to 32 bits integer conversion here !)
	oDrawingObjects[oNbObjects].circle.x=(int)GetIntegerProperty("cx");
	oDrawingObjects[oNbObjects].circle.y=(int)GetIntegerProperty("cy");
	oDrawingObjects[oNbObjects].circle.radius=(int)GetIntegerProperty("radius");
	// (first object)
	oNbObjects++;
	//		- A red rectangle
	oDrawingObjects[oNbObjects].kind=MAPSDrawingObject::Rectangle;
	oDrawingObjects[oNbObjects].width=1;
	oDrawingObjects[oNbObjects].color=MAPS_RGB(255,0,0);
	oDrawingObjects[oNbObjects].rectangle.x1=100;
	oDrawingObjects[oNbObjects].rectangle.y1=100;
	oDrawingObjects[oNbObjects].rectangle.x2=200;
	oDrawingObjects[oNbObjects].rectangle.y2=200;
	// (2nd object)
	oNbObjects++;
	//		- A white string "Hello World"
	oDrawingObjects[oNbObjects].kind=MAPSDrawingObject::Text;
	oDrawingObjects[oNbObjects].width=1;
	oDrawingObjects[oNbObjects].color=MAPS_RGB(255,255,255);
	MAPS::Strcpy(oDrawingObjects[oNbObjects].text.text,"Hello world");
	oDrawingObjects[oNbObjects].text.x=50;
	oDrawingObjects[oNbObjects].text.y=50;
	oDrawingObjects[oNbObjects].text.cwidth=6;
	oDrawingObjects[oNbObjects].text.cheight=10;
	// (3rd object)
	oNbObjects++;
	// Define in the IO buffer element how many objects we have in the vector
	oIOElt2DObjects->VectorSize()=oNbObjects;
	// OK, send the output buffer (and don't write into it any more !)
	StopWriting(oIOElt2DObjects);
	// End of 2D object output
	///////////////////////////////////
}
// END OF COMPONENT IMPLEMENTATION
////////////////////////////////////
