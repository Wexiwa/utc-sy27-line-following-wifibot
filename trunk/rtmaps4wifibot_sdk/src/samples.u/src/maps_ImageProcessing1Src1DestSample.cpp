////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter3 
 * \file
 * \brief ImageProcessing1Src1DestSample
 * This component is a sample of Image Processing component with 1 input image and 1 output image
 * This component inherits from the class MAPSImageProcessing1Src1Dest. It means that MAPSImageProcessing1Src1Dest 
 * provides to this component its basic behaviour and a set of standard inputs, outputs, properties and actions.
*/

#include "maps_ImageProcessing1Src1DestSample.h"

// Add here more inputs to your components
MAPS_BEGIN_INPUTS_DEFINITION(MAPSImageProcessing1Src1DestSample)
MAPS_END_INPUTS_DEFINITION

// Add here more outputs to your components
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSImageProcessing1Src1DestSample)
MAPS_END_OUTPUTS_DEFINITION

// Add here more properties to your components
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSImageProcessing1Src1DestSample)
MAPS_END_PROPERTIES_DEFINITION

// Add here more actions to your components
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSImageProcessing1Src1DestSample)
MAPS_END_ACTIONS_DEFINITION

// Put here the operations your component will implement
MAPS_BEGIN_IMAGEPROCESSING_1SRC1DEST_DEFINITION(MAPSImageProcessing1Src1DestSample)
    // To add operations, use the MAPS_IMAGEPROCESSING_1SRC1DEST macro
    MAPS_IMAGEPROCESSING_1SRC1DEST(1,		// The id of the operation
				   "THRESHOLD",	// The name of the operation
				   1,		// The number of integer parameters your operation needs
				   0,		// The number of double parameters your operation needs
				   NULL,	// Specify the color model of your operation output if it is different than the inputs images ones
				   0)		// Ends the operation definition with the number of click your operation needs
    MAPS_IMAGEPROCESSING_1SRC1DEST(2,"COPY",0,0,NULL,0)
    MAPS_IMAGEPROCESSING_1SRC1DEST(3,"INVERT",0,0,NULL,0)
    MAPS_IMAGEPROCESSING_1SRC1DEST(4,"SCALE",1,1,NULL,0)
MAPS_END_IMAGEPROCESSING_1SRC1DEST_DEFINITION

// Define your ImageProcessing name, version, behaviour, etc. Define also its inputs, outputs, properties, and actions number.
MAPS_IMAGEPROCESSING_1SRC1DEST_COMPONENT_DEFINITION(MAPSImageProcessing1Src1DestSample,"ImageProcessing1Src1DestSample","1.0",128,MAPS::Sequential|MAPS::Threaded,
						    MAPS::Threaded,0,0,0,0,
						    -1) // Reserved. Always put -1 here

// The Birth() method
void MAPSImageProcessing1Src1DestSample::Birth()
{
    // Always call the MAPSImageProcessing1Src1Dest::Birth() method first (the parent of this component)
    MAPSImageProcessing1Src1Dest::Birth();
    // Make your own initialisations here
}

// The Core() method
void MAPSImageProcessing1Src1DestSample::Core()
{
    // Always call the MAPSImageProcessing1Src1Dest::Core() method first (the parent of this component)
    MAPSImageProcessing1Src1Dest::Core();
    // Most of the time, you don't have to put code in the Core() method
}

// The Death() method
void MAPSImageProcessing1Src1DestSample::Death()
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
bool MAPSImageProcessing1Src1DestSample::Transform(IplImage *input, IplImage *output, const MAPSImageProcessing1Src1DestDefinition *desc, const MAPSImageProcessing1Src1DestParams *params)
{
	int		*intParams=params->intParams;
	double	*dblParams=params->dblParams;

	if (desc->transform==1) 
	{
		///////////////////////////////////////////////////////////////////
		// Show 1 very simple algorithm processing images 
		// A very simple threshold function that do not use the regions of interest of images (roi field of IplImage structure)...
		unsigned char *oData=(unsigned char *)output->imageData;
		const unsigned char *iData=(const unsigned char *)input->imageData;
		// Use the integer parameter as the reference for the threshold
		for(int i=0;i<output->imageSize;i++)
		{
			if (iData[i]>intParams[0])
				oData[i]=255;
			else
				oData[i]=0;
		}
	}
	else
	{	
		///////////////////////////////////////////////////////////////////
		// Show 3 more algorithms processing images taking ROI into account

		/////////////////
		// STANDARD CODE
		IplROI src,dest;

		// The following code inits the src an dest ROIs.
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

		// The following code allows to set the output ROI to the zone of image we are going to write to.
		// This is particularily useful when some input ROI has been specified and output ROI is not. With 
		// the ScaledROIsCheck method call, you get the region of interest of the output image you are supposed
		// to write to. Thus you can modify the output image roi consequently.
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

		/////////////////
		// SPECIFIC CODE : put your own processing here
		switch(desc->transform)
		{
		case 2 :
			{
				// A very simple copy function using ROIs
				unsigned char	*ibb=ib;
				unsigned char	*obb=ob;

				// We should have here, according to this transformation description (above, see MAPS_IMAGEPROCESSING_1SRC1DEST macros)
				//		iPixelWidth==oPixelWidth
				//		iROIBytesWidth==oROIBytesWidth (because src.width==dest.width, as scaling is (1.0,1.0)
				// One could assert these conditions !

				// Use a counter with the number of lines
				for(int j=0;j<dest.height;j++)
				{
					// Copy input pixels to output pixels
					MAPS::Memcpy(obb,ibb,oROIBytesWidth);
					// Go to the next input and output lines
					ibb+=iWidthStep;
					obb+=oWidthStep;
				}
			}
			return(true);
		case 3 :
			{
				// A very simple copy function using ROIs
				unsigned char	*ibb=ib;
				unsigned char	*obb=ob;

				// We should have here, according to this transformation description (above, see MAPS_IMAGEPROCESSING_1SRC1DEST macros)
				//		iPixelWidth==oPixelWidth
				//		iROIBytesWidth==oROIBytesWidth (because src.width==dest.width, as scaling is (1.0,1.0)
				// One could assert these conditions !

				// Use a counter with the number of lines
				for(int j=0;j<dest.height;j++)
				{
					for(int i=0;i<oROIBytesWidth;i++)
						obb[i]=255-ibb[i];
					// Go to the next input and output lines
					ibb+=iWidthStep;
					obb+=oWidthStep;
				}
			}
			return(true);
		case 4 :
			{
				// A very simple scaling function using ROIs
				unsigned char	*ibb=ib;
				unsigned char	*obb=ob;

				// We should have here, according to this transformation description (above, see MAPS_IMAGEPROCESSING_1SRC1DEST macros)
				//		iPixelWidth==oPixelWidth
				//		iROIBytesWidth==oROIBytesWidth (because src.width==dest.width, as scaling is (1.0,1.0)
				// One could assert these conditions !

				// Use a counter with the number of lines
				for(int j=0;j<dest.height;j++)
				{
					for(int i=0;i<oROIBytesWidth;i++)
						obb[i]=(unsigned char)(intParams[0]+dblParams[0]*ibb[i]+0.5);
					// Go to the next input and output lines
					ibb+=iWidthStep;
					obb+=oWidthStep;
				}
			}
			return(true);
		default :
			return(false);
		};
	}
	return(true);
}
