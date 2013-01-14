////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter3 
 * \file
 * \brief StereoVisionIPUSample
 * This is a very simple stereo IPU component. Here, the purpose of the processing
 * of the synchronised inputs (iImage0 and iImage1) is only an alpha blending operation.
 * You can use this component sample as template for your stereo vision algorithms. 
 * The main aim of this component is to illustrate the behaviour of the SynchroStartReading
 * method.
 */

/*
 * To test this component :
 * 		- Run RTMaps Studio
 * 		- Register the package containing this component
 *		- Find a database with 2 synchronised video data flows, replay this database and process
 * 		  these flow with the StereoVisionIPUSample, place Image Viewers to display input and output
 * 		  images.
 * 		- Run the generated Studio application and see.
*/

#include "maps_StereoVisionIPUSample.h"

// Declaration of 2 inputs accepting uncompressed images, with FifoReader mode by default.
MAPS_BEGIN_INPUTS_DEFINITION(MAPSStereoVisionIPUSample)
    MAPS_INPUT("iImage0",MAPS::FilterIplImage,MAPS::FifoReader)
    MAPS_INPUT("iImage1",MAPS::FilterIplImage,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Declaration of an output of type IplImage.
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSStereoVisionIPUSample)
    MAPS_OUTPUT("oImage",MAPS::IplImage,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Declaration of the pAlphaCoeff float property, allowed to be changed during diagram execution.
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSStereoVisionIPUSample)
    MAPS_PROPERTY("pAlphaCoeff",0.5,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions (here, no actions are present)
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSStereoVisionIPUSample)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (StereoVisionIPUSample) behaviour
MAPS_COMPONENT_DEFINITION(MAPSStereoVisionIPUSample,"StereoVisionIPUSample","1.0",128,MAPS::Threaded,MAPS::Threaded,2,1,1,0)

void MAPSStereoVisionIPUSample::Birth()
{
    m_firstTime=true;
}

void MAPSStereoVisionIPUSample::Core() 
{
	// Makes a list of inputs (array of 2 pointers to the input)
	MAPSInput *inputs[2]={&Input("iImage0"),&Input("iImage1")};
	// Declares an array of 2 pointers on MAPSIOElt
	MAPSIOElt *ioElts[2];
	// Ask for a synchronised read on Input("iImage0") and Input("iImage1").
	// The SynchroStartReading returns the timestamp of the both input data, and the IO
	// Elements pointers
	MAPSTimestamp iTimestamp=SynchroStartReading(2,inputs,ioElts);
	// Check if the returned IO Elements are OK (not NULL)
	if ((NULL == ioElts[0])||(NULL == ioElts[1]))
		return;
	// Inside this I/O element, we get :
	//		- the IPL image
	IplImage &iIPLImage0=ioElts[0]->IplImage();
	IplImage &iIPLImage1=ioElts[1]->IplImage();
	//		- the time of issue of the data (availability time in RTMaps)
	//MAPSTimestamp &iTimeOfIssue0=ioElts[0]->TimeOfIssue();
	//MAPSTimestamp &iTimeOfIssue1=ioElts[1]->TimeOfIssue();

	// Check if images are of the same color model (RGB or GRAY), channel sequence (RGB, BGR, etc.)
	// width and height, and image size.
	if ((*(MAPSUInt32*)(iIPLImage0.channelSeq)!=*(MAPSUInt32*)(iIPLImage1.channelSeq))||
		(*(MAPSUInt32*)(iIPLImage0.colorModel)!=*(MAPSUInt32*)(iIPLImage1.colorModel))||
		((iIPLImage0.width)!=(iIPLImage1.width))||
		((iIPLImage0.height)!=(iIPLImage1.height))||
		((iIPLImage0.imageSize)!=(iIPLImage1.imageSize)))
		return;

	// The first time we get an image data, we have to allocate the output buffers
	// Indeed, the size of the output buffer depends on the size of input images
	// (we want here the output image to have the same dimensions than the input images)
	if (m_firstTime)
	{
		// To allocate output buffer, we first define the model of images (dimensions
		// and color)
		IplImage model;
		model=MAPS::IplImageModel(iIPLImage0.width,iIPLImage0.height,iIPLImage0.channelSeq);

		// Our algorithm only needs the input images and the output one to have the same
		// global size.
		if ((iIPLImage0.imageSize)!=(model.imageSize))
			Error("Input and output images have a different global size");

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

	// Let's do a very simple processing with the input data
	// Get pointers to the first pixel of input and output images
	unsigned char *ib0=(unsigned char*)iIPLImage0.imageData;
	unsigned char *ib1=(unsigned char*)iIPLImage1.imageData;
	unsigned char *ob=(unsigned char*)oIPLImage.imageData;
	int k;
	// Get the alpha blending coeff
	MAPSFloat pAlphaCoeff0=GetFloatProperty("pAlphaCoeff");
	pAlphaCoeff0=MIN(1,pAlphaCoeff0);
	pAlphaCoeff0=MAX(0,pAlphaCoeff0);
	MAPSFloat pAlphaCoeff1=1-pAlphaCoeff0;

	for(k=0;k<iIPLImage0.imageSize;k++)
	{
		// Just makes an alpha blending of inputs
		ob[k]=(unsigned char)(ib0[k]*pAlphaCoeff0+ib1[k]*pAlphaCoeff1);
	}

	// OK, send the output buffer (and don't use it any more !)
	StopWriting(oIOEltImage);
	// End of image processing
	///////////////////////////////////
}

void MAPSStereoVisionIPUSample::Death()
{
}
