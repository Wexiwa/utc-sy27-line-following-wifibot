/* -*- mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
// MAPSStream8IOComponent
// Intempora SA - Copyright (C) 2002-2003
// V1.0 : P.C. - 05/03/2002
////////////////////////////////////////////////////////

#ifndef _MAPSStream8IOComponent_H
#define _MAPSStream8IOComponent_H

#ifndef _MAPS_H
#include "maps.hpp"
#endif

#include "MAPSRBRegion.h"

/*! \file
 *  \brief The MAPSStream8IOComponent component model
 */

//! Processing component template with 1 Stream8 input and 1 Stream8 output.
class MAPSStream8IOComponent: public MAPSComponent 
{
	MAPS_PARENT_COMPONENT_HEADER_CODE(MAPSStream8IOComponent,MAPSComponent);
	///////////////////////////////
	unsigned char*	data;
	MAPSTimestamp*	timestamp;
	MAPSTimestamp*	timeOfIssue;
	MAPSRBRegion	fifo;
	int				pOutputFIFOsSize;
	bool			firstTime;

	void			Initialize(MAPSInt64 fifoSize);
	void			Close();

protected :
	//! Processing function to implement in the child class.
	virtual void NewDataCallback(MAPSRBRegion& region,const unsigned char* data, const MAPSTimestamp* timestamp, const MAPSTimestamp* timeOfIssue)=0;
	//! @name Output writing functions.
	//@{
	virtual bool SendData(const unsigned char* stream, int streamSize, MAPSTimestamp ts=0);
	virtual bool SendData(const char* stream, int streamSize, MAPSTimestamp ts=0);
	//@}
};

#endif
