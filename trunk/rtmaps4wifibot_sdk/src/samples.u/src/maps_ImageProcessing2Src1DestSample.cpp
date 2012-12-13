////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter3 
 * \file
 * \brief ImageProcessing2Src1DestSample
 * This component is a template for RTMaps Image Processing operations with 2 inputs and 1 output 
 * This component inherits from the class MAPSImageProcessing2Src1Dest. It means that MAPSImageProcessing2Src1Dest 
 * provides to this component its basic behaviour and a set of standard inputs, outputs, properties and actions.
*/
#include "maps_ImageProcessing2Src1DestSample.h"

#ifdef MIN3
#undef MIN3
#endif
#define MIN3(x,y,z) ((x)<(y)?((x)<(z)?(x):(z)):((y)<(z)?(y):(z)))

// Add here more inputs to your components
MAPS_BEGIN_INPUTS_DEFINITION(MAPSImageProcessing2Src1DestSample)
MAPS_END_INPUTS_DEFINITION

// Add here more outputs to your components
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSImageProcessing2Src1DestSample)
MAPS_END_OUTPUTS_DEFINITION

// Add here more properties to your components
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSImageProcessing2Src1DestSample)
MAPS_END_PROPERTIES_DEFINITION

// Add here more actions to your components
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSImageProcessing2Src1DestSample)
MAPS_END_ACTIONS_DEFINITION

// Put here the operations your component will implement
MAPS_BEGIN_IMAGEPROCESSING_2SRC1DEST_DEFINITION(MAPSImageProcessing2Src1DestSample)
    // To add operations, use the MAPS_IMAGEPROCESSING_2SRC1DEST macro
    MAPS_IMAGEPROCESSING_2SRC1DEST(1,		// The id of the operation
				   "ADD",	// The name of the operation
				   1,		// The number of integer parameters your operation needs
				   1,		// The number of double parameters your operation needs
				   NULL,	// Specify the color model of your operation output if it is different than the inputs images ones
				   0)		// Ends the operation definition with the number of click your operation needs
    MAPS_IMAGEPROCESSING_2SRC1DEST(2,"SUB",1,1,NULL,0)
    MAPS_IMAGEPROCESSING_2SRC1DEST(3,"SUB2",0,0,NULL,0)
MAPS_END_IMAGEPROCESSING_2SRC1DEST_DEFINITION


// Define your ImageProcessing name, version, behaviour, etc. Define also its inputs, outputs, properties, and actions number.
MAPS_IMAGEPROCESSING_2SRC1DEST_COMPONENT_DEFINITION(MAPSImageProcessing2Src1DestSample,"ImageProcessing2Src1DestSample","1.0",128,MAPS::Sequential|MAPS::Threaded,
						    MAPS::Threaded,0,0,0,0,
						    -1) // Reserved. Always put -1 here

// The Birth() method
void MAPSImageProcessing2Src1DestSample::Birth()
{
    // Always call the MAPSImageProcessing2Src1Dest::Birth() method first (the parent of this component)
    MAPSImageProcessing2Src1Dest::Birth();
    // Make your own initialisations here
}

// The Core() method
void MAPSImageProcessing2Src1DestSample::Core()
{
    // Always call the MAPSImageProcessing2Src1Dest::Core() method first (the parent of this component)
    MAPSImageProcessing2Src1Dest::Core();
    // Most of the time, you don't have to put code in the Core() method
}

// The Death() method
void MAPSImageProcessing2Src1DestSample::Death()
{
    // Make your own closure here
    
    // Always call the MAPSImageProcessing2Src1Dest::Death() method at the end (the parent of this component)
    MAPSImageProcessing2Src1Dest::Death();
}

// The main function for a RTMaps Image Processing operations with 2 inputs and 1 output. This method
// is called each time a set of 2 input data has been read and stacked by the parent class MAPSImageProcessing2Src1Dest.
// This function is called with the following parameters :
//	- input0 : the image got from the iImage0 input
//	- input1 : the image got from the iImage1 input
//	- output : the image to be written by your operation, image that will be sent to oImage ouput
//	- desc   : the operation description as given by the MAPS_IMAGEPROCESSING_2SRC1DEST macros above (in the operation 
//				definition location)
//	- params : the parameters (integers, doubles, clicks) of the operation
bool MAPSImageProcessing2Src1DestSample::Transform(IplImage *input0, IplImage *input1, IplImage *output, const MAPSImageProcessing2Src1DestDefinition *desc, const MAPSImageProcessing2Src1DestParams *params)
{
	int		*intParams=params->intParams;
	double	*dblParams=params->dblParams;

	if (desc->transform==1)
	{
		///////////////////////////////////////////////////////////////////
		// Show 1 very simple algorithm processing images 
		// A very simple addition function : do not use the regions of interest of images (roi field of IplImage structure)...

		// For this reason, we just test if images have the same size
		if (input0->imageSize!=input1->imageSize)
		{
			ReportWarning("This operation can only be done on images of the same dimensions and types");
			return(false);
		}
		if (output->imageSize!=input1->imageSize)
		{
			ReportWarning("This operation can only be done on images of the same dimensions and types");
			return(false);
		}
		// Get the image pointers
		unsigned char *oData=(unsigned char *)output->imageData;
		const unsigned char *iData0=(const unsigned char *)input0->imageData;
		const unsigned char *iData1=(const unsigned char *)input1->imageData;
		// Use the integer parameter as a constant offset.
		// Use the double parameter as a scale factor.
		for(int i=0;i<output->imageSize;i++)
			oData[i]=(unsigned char)(intParams[0]+(iData0[i]+iData1[i])*dblParams[0]+0.5);
	}
	else
	{
		IplROI src0,src1,dest;
		// The following code inits the src1, src2 and dest ROIs.
		if ((input0!=NULL)&&(input1!=NULL)&&(output!=NULL)) {		
			if (input0->roi)
				MAPS::Memcpy(&src0,input0->roi,sizeof(IplROI));
			else {
				src0.coi=0;
				src0.xOffset=src0.yOffset=0;
				src0.width=input0->width;
				src0.height=input0->height;
			}
			if (input1->roi)
				MAPS::Memcpy(&src1,input1->roi,sizeof(IplROI));
			else {
				src1.coi=0;
				src1.xOffset=src1.yOffset=0;
				src1.width=input1->width;
				src1.height=input1->height;
			}
			if (output->roi)
				MAPS::Memcpy(&dest,output->roi,sizeof(IplROI));
			else {
				dest.coi=0;
				dest.xOffset=dest.yOffset=0;
				dest.width=output->width;
				dest.height=output->height;
			}
			int dw=MIN3(src0.width,src1.width,dest.width);
			int dh=MIN3(src0.height,src1.height,dest.height);
			src0.width=src1.width=dest.width=dw;
			src0.height=src1.height=dest.height=dh;
		}

		// The following code allows to set the output ROI to the zone of image we are going to write to.
		// This is particularily useful when some inputs ROI have been specified and output ROI is not.
		if (output->roi)
			MAPS::Memcpy(output->roi,&dest,sizeof(IplROI));
		// The widthStep is a very important parameter of an IplImage structure. It gives the nb of bytes
		// between one given pixel and the pixel under this one on the next line
		int				&i0WidthStep=input0->widthStep;
		int				&i1WidthStep=input1->widthStep;
		int				&oWidthStep=output->widthStep;
		// The pixel width is the number of bytes per pixel
		int				i0PixelWidth=(input0->nChannels*input0->depth+7)/8;
		int				i1PixelWidth=(input1->nChannels*input1->depth+7)/8;
		int				oPixelWidth=(output->nChannels*output->depth+7)/8;
		// Calculates ROI width in bytes
		//int				i0ROIBytesWidth=src0.width*i0PixelWidth;
		//int				i1ROIBytesWidth=src1.width*i1PixelWidth;
		int				oROIBytesWidth=dest.width*oPixelWidth;
		// The following variables point to the first pixel of input and output images, taking into account
		// inputs ROI and output ROI.
		unsigned char	*i0b=(unsigned char *)(input0->imageData+src0.yOffset*i0WidthStep+src0.xOffset*i0PixelWidth);
		unsigned char	*i1b=(unsigned char *)(input1->imageData+src1.yOffset*i1WidthStep+src1.xOffset*i1PixelWidth);
		unsigned char	*ob=(unsigned char *)(output->imageData+dest.yOffset*oWidthStep+dest.xOffset*oPixelWidth);

		// We check some pixels requirements on images:
		if ((i0PixelWidth!=i1PixelWidth)||(i0PixelWidth!=oPixelWidth))
			Error("Inputs and output images must have the same pixel width to be processed");

		switch(	desc->transform)
		{
		case 2 :
			{
				// Use a counter with the number of lines
				for(int j=0;j<dest.height;j++)
				{
					for(int i=0;i<oROIBytesWidth;i++)
						ob[i]=(unsigned char)(intParams[0]+(i0b[i]-i1b[i])*dblParams[0]+0.5);
					// Go to the next input and output lines
					i0b+=i0WidthStep;
					i1b+=i1WidthStep;
					ob+=oWidthStep;
				}
			}
			break;
		case 3 :
			{
				// Use a counter with the number of lines
				for(int j=0;j<dest.height;j++)
				{
					for(int i=0;i<oROIBytesWidth;i++)
						ob[i]=(unsigned char)((int)i0b[i]-i1b[i]+0.5);
					// Go to the next input and output lines
					i0b+=i0WidthStep;
					i1b+=i1WidthStep;
					ob+=oWidthStep;
				}
			}
			break;
		default :
			return(false);
		}
	}
	return(true);
}
