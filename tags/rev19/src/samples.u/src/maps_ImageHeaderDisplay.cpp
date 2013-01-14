////////////////////////////////
// RTMaps SDK Sample Component
////////////////////////////////

/*! \ingroup Chapter3 
 * \file
 * \brief ImageHeaderDisplay
 * This component illustrates how to handle the content of the IplImage structure.
*/

#include "maps_ImageHeaderDisplay.h"	// Includes the header of this component

// Use the macros to declare the inputs 
MAPS_BEGIN_INPUTS_DEFINITION(MAPSImageHeaderDisplay)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs (here, no outputs are present)
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSImageHeaderDisplay)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSImageHeaderDisplay)
    MAPS_PROPERTY("pShortDescription",true,false,true)
    MAPS_PROPERTY("pOneShotOnly",false,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions (here, no actions are present)
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSImageHeaderDisplay)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (ImageHeaderDisplay) behaviour
MAPS_COMPONENT_DEFINITION(MAPSImageHeaderDisplay,"ImageHeaderDisplay","1.0",128,MAPS::Threaded,MAPS::Threaded,
			  1, // Number of inputs
			  0, // Number of outputs
			  2, // Number of properties
			  0) // Number of actions

void MAPSImageHeaderDisplay::Birth()
{
}

void MAPSImageHeaderDisplay::Core() 
{
	// Get the I/O buffer element containing the new image data
	MAPSIOElt* iIOEltImage=StartReading(Input("iImage"));
	// Check the returned pointer
	if (NULL==iIOEltImage)
		return;
	// Get the IplImage structure encapsulated in the MAPSIOElt structure of this
	// input data. We declare a const reference because the returned IplImage 
	// structure must never be modified when the MAPSIOElt has been returned by a 
	// StartReading call !
	const IplImage &image=iIOEltImage->IplImage();

	MAPSStreamedString str;
	int i;
	str<<"Data timestamp="<<iIOEltImage->Timestamp()<<" and timeOfIssue="<<iIOEltImage->TimeOfIssue()<<'\n';
	str<<"nChannels="<<image.nChannels<<'\n';
	str<<"alphaChannel="<<image.alphaChannel<<'\n';
	str<<"depth="<<image.depth;
	switch(image.depth)
	{
	case IPL_DEPTH_1U :
		str<<"-> IPL_DEPTH_1U";
		break;
	case IPL_DEPTH_8U :
		str<<"-> IPL_DEPTH_8U";
		break;
	case IPL_DEPTH_16U :
		str<<"-> IPL_DEPTH_16U";
		break;
	case IPL_DEPTH_32F :
		str<<"-> IPL_DEPTH_32F";
		break;
	case IPL_DEPTH_8S :
		str<<"-> IPL_DEPTH_8S";
		break;
	case IPL_DEPTH_16S :
		str<<"-> IPL_DEPTH_16S";
		break;
	case IPL_DEPTH_32S :
		str<<"-> IPL_DEPTH_32S";
		break;
	default:
		str<<"-> Unknown";
		break;
	};
	str<<'\n';
	str<<"colorModel=";
	for(i=0;i<4;i++)
	{
		if (image.colorModel[i]>=32)
			str<<"'"<<(char)(image.colorModel[i])<<"' ";
		else
			str<<"0x"<<MAPSSManip::hex<<(int)image.colorModel[i]<<" ";
	}
	str<<'\n';
	str<<"channelSeq=";
	for(i=0;i<4;i++)
	{
		if (image.channelSeq[i]>=32)
			str<<"'"<<(char)(image.channelSeq[i])<<"' ";
		else
			str<<"0x"<<MAPSSManip::hex<<(int)image.channelSeq[i]<<" ";
	}
	str<<'\n';
	str<<"dataOrder="<<MAPSSManip::dec<<image.dataOrder;
	switch(image.dataOrder)
	{
	case IPL_DATA_ORDER_PIXEL :
		str<<"-> IPL_DATA_ORDER_PIXEL";
		break;
	case IPL_DATA_ORDER_PLANE :
		str<<"-> IPL_DATA_ORDER_PLANE";
		break;
	default:
		str<<"-> Unknown";
		break;
	};
	str<<'\n';
	str<<"origin="<<image.origin;
	switch(image.origin)
	{
	case IPL_ORIGIN_TL :
		str<<"-> IPL_ORIGIN_TL";
		break;
	case IPL_ORIGIN_BL :
		str<<"-> IPL_ORIGIN_BL";
		break;
	default:
		str<<"-> Unknown";
		break;
	};
	str<<'\n';
	str<<"align="<<image.align;
	switch(image.align)
	{
	case IPL_ALIGN_4BYTES :
		str<<"-> IPL_ALIGN_4BYTES";
		break;
	case IPL_ALIGN_8BYTES :
		str<<"-> IPL_ALIGN_8BYTES";
		break;
	case IPL_ALIGN_16BYTES :
		str<<"-> IPL_ALIGN_16BYTES";
		break;
	case IPL_ALIGN_32BYTES :
		str<<"-> IPL_ALIGN_32BYTES";
		break;
	default:
		str<<"-> Unknown";
		break;
	};
	str<<'\n';
	str<<"width="<<image.width<<'\n';
	str<<"height="<<image.height<<'\n';
	str<<"roi=0x"<<MAPSSManip::hex<<
#ifdef MAPS_64
		(MAPSInt64)(void*)image.roi
#else
		(MAPSInt32)(void*)image.roi
#endif
		<<MAPSSManip::dec;
	if (image.roi)
	{
		str<<". xOffset="<<image.roi->xOffset;
		str<<". yOffset="<<image.roi->yOffset;
		str<<". width="<<image.roi->width;
		str<<". height="<<image.roi->height;
		str<<". coi="<<image.roi->coi;
		if (image.roi->coi==0)
			str<<" (all the channels)";
	}
	else
		str<<" (all the image)";
	str<<'\n';
	str<<"imageSize="<<image.imageSize<<" bytes."<<'\n';
	str<<"imageData=0x"<<MAPSSManip::hex<<
#ifdef MAPS_64
		(MAPSInt64)(void*)image.imageData
#else
		(MAPSInt32)(void*)image.imageData
#endif
		<<'\n';
	str<<"widthStep="<<MAPSSManip::dec<<image.widthStep<<" bytes"<<'\n';


	if (!GetBoolProperty("pShortDescription"))
	{
		str<<"nSize=0x"<<MAPSSManip::hex<<image.nSize<<'\n';
		str<<"ID=0x"<<image.ID<<'\n';
		str<<"maskROI=0x"<<
#ifdef MAPS_64
                (MAPSInt64)(void*)image.maskROI
#else
                (MAPSInt32)(void*)image.maskROI
#endif
			<<'\n';
		str<<"imageId=0x"<<
#ifdef MAPS_64          
                (MAPSInt64)(void*)image.imageId
#else
                (MAPSInt32)(void*)image.imageId
#endif
			<<'\n';
		str<<"tileInfo=0x"<<
#ifdef MAPS_64          
                (MAPSInt64)(void*)image.tileInfo
#else
                (MAPSInt32)(void*)image.tileInfo
#endif
			<<'\n';
		str<<"imageDataOrigin=0x"<<
#ifdef MAPS_64          
                (MAPSInt64)(void*)image.imageDataOrigin
#else
                (MAPSInt32)(void*)image.imageDataOrigin
#endif
			<<'\n';
		str<<"BorderMode=";
		for(i=0;i<4;i++)
			str<<"("<<i<<"="<<image.BorderMode[i]<<") ";
		str<<'\n';
		str<<"BorderConst=";
		for(i=0;i<4;i++)
			str<<"("<<i<<"="<<image.BorderConst[i]<<") ";
		str<<'\n';
	}
	ReportInfo(str);

	if (GetBoolProperty("pOneShotOnly"))
		CommitSuicide();
}

void MAPSImageHeaderDisplay::Death()
{
}
