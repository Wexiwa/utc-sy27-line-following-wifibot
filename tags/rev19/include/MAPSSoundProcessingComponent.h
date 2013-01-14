/* -*- mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
// MAPSSoundProcessingComponent
// Intempora SA - Copyright (C) 2002-2007
// V1.0 : O.M. - 08/02/2006
////////////////////////////////////////////////////////

#ifndef _Maps_SoundProcessingComponent_H
#define _Maps_SoundProcessingComponent_H

#ifndef _MAPS_H
#include "maps.hpp"
#endif

/*! \file
*	\brief The MAPSSoundProcessingComponent component model
* Format of IO element: 
*  - Frequency() holds frequency info (!!! in mHz !!!!)
*  - Misc1() holds the number of channels
*  - Misc2() holds the bits info
*/

typedef struct MAPSAudioStreamDef
{
	MAPSUInt8	mBits;
	MAPSUInt8	mChan;
	MAPSInt64	mFrequency;
} MAPSAudioStreamDef;

class MAPSSoundProcessingComponent : public MAPSComponent 
{
	MAPS_PARENT_COMPONENT_HEADER_CODE(MAPSSoundProcessingComponent,MAPSComponent)
	///////////////////////////////
	MAPSFloat* 			m_data;
	bool 				m_firstTime;
	bool 				m_bConversionNeeded;
	MAPSAudioStreamDef 	m_inputFormat;

protected:
	MAPSAudioStreamDef&
	GetFormat() { return m_inputFormat; }
	//! Processing function to implement in the child class. A frame is made of nbChannels samples.
	virtual void
	Process(MAPSFloat const *const	inData,
			MAPSTimestamp			inTS,
			MAPSFloat *const		outData,
			MAPSTimestamp*			pOutTS,
			int const				sampleFrames,
			short const				nbChannels,
			bool*					pCancel)=0;
};

#endif
