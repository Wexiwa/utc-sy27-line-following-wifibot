/* -*- mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
// Intempora SA - Copyright (C) 2001-2003
// P.C. - 02/09/2001
////////////////////////////////////////////////////////

#ifndef _MAPSImageProcessing1Src1Dest_H
#define _MAPSImageProcessing1Src1Dest_H

#ifndef _MAPS_H
#include "maps.hpp"
#endif

struct MAPSPoint2D 
{
	double x;
	double y;
};

/*! \file
 *	\brief The MAPSImageProcessing1Src1Dest component model
 */

#define MAPS_BEGIN_IMAGEPROCESSING_1SRC1DEST_DEFINITION(component) const MAPSImageProcessing1Src1DestDefinition component::operations[]={
#define MAPS_IMAGEPROCESSING_1SRC1DEST(id,name,nbIntParams,nbDblParams,colorModel,nbClicksUsed) {id,name,nbIntParams,nbDblParams,colorModel,false,nbClicksUsed},
#define MAPS_END_IMAGEPROCESSING_1SRC1DEST_DEFINITION NULL};

#define MAPS_IMAGEPROCESSING_1SRC1DEST_STANDARD_HEADER_CODE(component) \
	MAPS_CHILD_COMPONENT_HEADER_CODE(component,MAPSImageProcessing1Src1Dest); \
	static const MAPSImageProcessing1Src1DestDefinition operations[]; \
	bool Transform(IplImage *input, IplImage *output, const MAPSImageProcessing1Src1DestDefinition *desc, const MAPSImageProcessing1Src1DestParams *params); \
	const MAPSImageProcessing1Src1DestDefinition *GetTransformationsDescription();

#define MAPS_IMAGEPROCESSING_1SRC1DEST_COMPONENT_DEFINITION(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions,nbOfOperations) \
	MAPS_COMPONENT_DEFINITION(component,name,version,priority,kind,defaultBehaviour,nbOfInputs,nbOfOutputs,nbOfProperties,nbOfActions) \
	const MAPSImageProcessing1Src1DestDefinition	*component::GetTransformationsDescription() \
	{ \
		return(operations); \
	}

//! Definition structure for an image processing algorithm (1 Src 1 Dest operation).
struct MAPSImageProcessing1Src1DestDefinition 
{
	//! Algorithm identifier.
	int			transform;
	//! Algorithm name
	const char*	str;
	//! Required number of integer parameters for this algorithm
	int			nbIntParams;
	//! Required number of double parameters for this algorithm
	int			nbDoubleParams;
	//! Output color model for auto-allocation feature
	const char*	colorModel;
	bool		needTheROI;
	//! Specify the number of 2D points the algorithm may expect
	int			nbClicksUsed;
};

//! Parameter structure for an image processing algorithm (1 Src 1 Dest operation).
struct MAPSImageProcessing1Src1DestParams
{
	//! Integer parameters array.
	int*			intParams;
	//! Number of integer parameters in the intParams array.
	int				nbIntParams;
	//! Double parameters array.
	double*			dblParams;
	//! Number of double parameters in the dblParams array.
	int				nbDblParams;
	//! 2D points array.
	MAPSPoint2D*	clicks;
	//! Number of 2D points in the clicks array.
	int				nbClicks;
	//! Timestamp of input image
	MAPSTimestamp	ts;
	//! TimeOfIssue of input image
	MAPSTimestamp	toi;
};

//! Image Processing component base class with 1 input and 1 output images.
class MAPSImageProcessing1Src1Dest: public MAPSComponent 
{
	MAPS_PARENT_COMPONENT_HEADER_CODE_WITHOUT_CONSTRUCTOR(MAPSImageProcessing1Src1Dest,MAPSComponent);
	///////////////////////////////
private :
	static const char*	ConstStrDelimiters;

	const MAPSImageProcessing1Src1DestDefinition*	operationsDescriptions;
	MAPSImageProcessing1Src1DestParams				params;

	enum IPOpState {
		IPOpReady=0,
		IPOpWait4Operation=1,
		IPOpWait4Parameters=2,
	};

	bool			firstTime;
	bool			firstDynamic;
	int				currentOperationId;
	IPOpState		state;
	
	int*			intParamsTemp;
	double*			doubleParamsTemp;
	IplROI*			inputROI;
	IplROI*			outputROI;

	MAPSIOElt*		lastIOElt;
	MAPSEvent		reprocessLastImageEvent;
	MAPSInput*		inputs[4];
	MAPSEvent*		events[1];
	IplImage		outputModel;
	MAPSMutex		mutex;
	int				nbSkipped;

	void Process(MAPSIOElt *ioEltInput);

	bool Initialize();
	bool Close();
	bool CloseOperation();
	bool CloseParameters();
	bool CloseROI();
	bool CloseClicks();
	bool UpdateClickProperty(MAPSProperty &p,MAPSPoint2D *clicks,int nbClicks);
	bool UpdateROIProperty(MAPSProperty &p,IplROI *imageROI);

	void Set(MAPSProperty& p, bool value);
	void Set(MAPSProperty& p, MAPSInt64 value);			
	void Set(MAPSProperty& p, MAPSFloat value);			
	void Set(MAPSProperty& p,const MAPSString& value);	
	bool SetOperation(const char *operation);
	bool SetParameters(const char *parameters);
	bool SetClicks(const char *clicks);
	bool SetROI(IplROI **imageROI,const char *roi);

	///////////////////////////////
protected :
	MAPSImageProcessing1Src1Dest(const char *componentName, MAPSComponentDefinition& md):MAPSComponent(componentName, md) 
	{
		firstDynamic=true;
	};

	virtual ~MAPSImageProcessing1Src1Dest();

	virtual bool  Transform(IplImage *input, IplImage *output, 
							const MAPSImageProcessing1Src1DestDefinition *desc, 
							const MAPSImageProcessing1Src1DestParams *params)=0;
	virtual const MAPSImageProcessing1Src1DestDefinition* GetTransformationsDescription()=0;

	virtual IplImage GetOutputModel(const IplImage *input, const IplROI *outputROI,const MAPSImageProcessing1Src1DestDefinition *desc);
};

#endif
