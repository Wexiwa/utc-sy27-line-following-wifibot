////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter3 
 * \file
 * \brief ROIThreshold
 * This component is a sample of image processing in RTMaps using the ImageProcessing1Src1Dest
 * model. This simple component just threshold image inputs according to a user-specified
 * property containing the threshold level.
*/

#include "maps_ROIThreshold.h"

// Add here more inputs to your components
MAPS_BEGIN_INPUTS_DEFINITION(MAPSROIThreshold)
MAPS_END_INPUTS_DEFINITION

// Add here more outputs to your components
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSROIThreshold)
MAPS_END_OUTPUTS_DEFINITION

// Add here more properties to your components
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSROIThreshold)
    MAPS_PROPERTY("pThreshold",128,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Add here more actions to your components
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSROIThreshold)
MAPS_END_ACTIONS_DEFINITION

// Put here the operations your component will implement
MAPS_BEGIN_IMAGEPROCESSING_1SRC1DEST_DEFINITION(MAPSROIThreshold)
    // To add operations, use the MAPS_IMAGEPROCESSING_1SRC1DEST macro
    MAPS_IMAGEPROCESSING_1SRC1DEST(1,		// The id of the operation
				   "THRESHOLD",	// The name of the operation
				   0,		// The number of integer parameters your operation needs
				   0,		// The number of double parameters your operation needs
				   NULL,	// Specify the color model of your operation output if it is different than the inputs images ones
				   0)		// Ends the operation definition with the number of click your operation needs
MAPS_END_IMAGEPROCESSING_1SRC1DEST_DEFINITION

// Define your ImageProcessing name, version, behaviour, etc. Define also its inputs, outputs, properties, and actions number.
MAPS_IMAGEPROCESSING_1SRC1DEST_COMPONENT_DEFINITION(MAPSROIThreshold,"ROIThreshold","1.0",128,MAPS::Sequential|MAPS::Threaded,
						    MAPS::Threaded,0,0,1,0,
						    -1) // Reserved. Always put -1 here

// The Birth() method
void MAPSROIThreshold::Birth()
{
    // Always call the MAPSImageProcessing1Src1Dest::Birth() method first (the parent of this component)
    MAPSImageProcessing1Src1Dest::Birth();
    // Make your own initialisations here
}

// The Core() method
void MAPSROIThreshold::Core()
{
    // Always call the MAPSImageProcessing1Src1Dest::Core() method first (the parent of this component)
    MAPSImageProcessing1Src1Dest::Core();
    // Most of the time, you don't have to put code in the Core() method
}

// The Death() method
void MAPSROIThreshold::Death()
{
    // Make your own closure here
    
    // Always call the MAPSImageProcessing1Src1Dest::Death() method at the end (the parent of this component)
    MAPSImageProcessing1Src1Dest::Death();
}

// The main function for a RTMaps Image Processing operations with 1 input and 1 output. This method
// is called each time a set of 2 input data has been read and stacked by the parent class MAPSImageProcessing1Src1Dest.
// This function is called with the following parameters :
//	- input : the image got from the iImage input
//	- output : the image to be written by your operation, image that will be sent to oImage ouput
//	- desc   : the operation description as given by the MAPS_IMAGEPROCESSING_1SRC1DEST macros above (in the operation 
//				definition location)
//	- params : the parameters (integers, doubles, clicks) of the operation
bool MAPSROIThreshold::Transform(IplImage *input, IplImage *output, const MAPSImageProcessing1Src1DestDefinition *desc, const MAPSImageProcessing1Src1DestParams *params)
{
	// Get the threshold value
	unsigned char threshold=(unsigned char)GetIntegerProperty("pThreshold");

	IplROI src,dest;
	// Get the correct input and output ROIs into src and dest structures...
	if ((input!=NULL)&&(output!=NULL)) {
		if (input->roi)
			MAPS::Memcpy(&src,input->roi,sizeof(IplROI));
		else {
			src.coi=0;
			src.xOffset=src.yOffset=0;
			src.width=input->width;
			src.height=input->height;
		}
		if (output->roi)
			MAPS::Memcpy(&dest,output->roi,sizeof(IplROI));
		else {
			dest.coi=0;
			dest.xOffset=dest.yOffset=0;
			dest.width=output->width;
			dest.height=output->height;
		}
		if (src.width<dest.width)
			dest.width=src.width;
		else
			src.width=dest.width;

		if (src.height<dest.height)
			dest.height=src.height;
		else
			src.height=dest.height;
	}

	// Change the output roi to be the correct one (
	if (output->roi!=NULL)
		MAPS::Memcpy(output->roi,&dest,sizeof(IplROI));

	// The widthStep is a very important parameter of an IplImage structure. It gives the nb of bytes
	// between one given pixel and the pixel under this one on the next line
	int				&iWidthStep=input->widthStep;
	int				&oWidthStep=output->widthStep;
	// The pixel width is the number of bytes per pixel
	int				iPixelWidth=(input->nChannels*input->depth+7)/8;
	int				oPixelWidth=(output->nChannels*output->depth+7)/8;
	// Calculates ROI width in bytes
	//int				iROIBytesWidth=src.width*iPixelWidth;
	int				oROIBytesWidth=dest.width*oPixelWidth;
	// The following variables point to the first pixel of input and output images, taking into account
	// input ROI and output ROI.
	unsigned char	*ib=(unsigned char *)(input->imageData+src.yOffset*iWidthStep+src.xOffset*iPixelWidth);
	unsigned char	*ob=(unsigned char *)(output->imageData+dest.yOffset*oWidthStep+dest.xOffset*oPixelWidth);

	// We should have here, according to this transformation description (above, see MAPS_IMAGEPROCESSING_1SRC1DEST macros)
	//		iPixelWidth==oPixelWidth
	//		iROIBytesWidth==oROIBytesWidth (because src.width==dest.width, as scaling is (1.0,1.0)
	// One could assert these conditions !

	// Use a counter with the number of lines
	for(int j=0;j<dest.height;j++)
	{
		for(int i=0;i<oROIBytesWidth;i++)
		{
			if (ib[i]>threshold)
				ob[i]=255;
			else
				ob[i]=0;
		}
		// Go to the next input and output lines
		ib+=iWidthStep;
		ob+=oWidthStep;
	}

	return(true);
}
