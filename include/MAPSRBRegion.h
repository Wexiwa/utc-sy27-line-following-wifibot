/* -*- mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
// Intempora SA - Copyright (C) 2001-2003
// V1.0 : P.C. - 14/04/2002 Creation
////////////////////////////////////////////////////////
// Purpose : RTMaps MAPSRingBufferRegion class
////////////
/*! \file
 *	\brief The RTMaps Ring-buffer region class and associated types
 */

#ifndef _MAPSRBRegion_H
#define _MAPSRBRegion_H

/////////////////////////////////////////////////////
// MAPSRingBuffer Region class, types, enumerations
#define MAPSRBREGION_NO_ERRORS(st) (!((st)&(MAPSConstRBRegionErrors)))
#define MAPSRBREGION_NO_WARNINGS(st) (!((st)&(MAPSConstRBRegionErrors|MAPSConstRBRegionWarnings)))
#define MAPSRBREGION_STATE_IS(st,stx) (((st)&(stx))&(!(MAPSConstRBRegionErrors|MAPSConstRBRegionWarnings)))

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

//! Flags for ring-buffer region state.
enum MAPSConstRBRegionState 
{
    // Normal states : region is OK
    //! When the region has been initialized, but is empty
    MAPSConstRBRegionEmpty		=0x00000000,
    //! When the region has been initialized and contains some data
    MAPSConstRBRegionReady		=0x00000001,
    // Warnings : region is valid but some warnings occured
    //! Warnings common flag
    MAPSConstRBRegionWarnings		=0x40000000,
    //! When the region has not been initialized
    MAPSConstRBRegionUninitialized	=0x40000100,
    //! When the region has been initialized, is full, and the last operation returned an overflow. This could never be the state of a region, but the state of an operation on a region.
    MAPSConstRBRegionOverflowOp		=0x40000200,
    // Errors : region is invalid or an error occured
    //! Errors common flag
    MAPSConstRBRegionErrors		=0x80000000,
    //! When the region is invalid (offset, size, fifoSize are inconsistent)
    MAPSConstRBRegionInvalid		=0x80010000,
    //! Only returned by an operation when the operation is invalid on the current region. This could never be the state of a region, but the state of an operation on a region.
    MAPSConstRBRegionInvalidOp		=0x80020000
};


//! Flags for the MAPSRBRegion::Erode method
enum MAPSConstRBRegionErosion 
{
    //! Erode from the beginning of the region	
    MAPSConstRBRegionErosionBeginning	=0x00000001,
    //! Erode from the end of the region
    MAPSConstRBRegionErosionEnd		=0x00000002
};

//! Flags for the MAPSRBRegion::Dilate method
enum MAPSConstRBRegionDilation 
{
    //! Dilate from the beginning of the region	
    MAPSConstRBRegionDilationBeginning	=0x00000001,
    //! Dilate from the end of the region
    MAPSConstRBRegionDilationEnd	=0x00000002
};

//! Flags for the MAPSRBRegion::Crop method
enum MAPSConstRBRegionCrop 
{
    //! Crop from the beginning of the region	
    MAPSConstRBRegionCropBeginning	=0x00000001,
    //! Crop from the end of the region
    MAPSConstRBRegionCropEnd		=0x00000002
};

//! Flags for the MAPSRBRegion::SubRegion method
enum MAPSConstRBRegionSubRegion 
{
    //! Extract the sub-region from the beginning of the source region	
    MAPSConstRBRegionSubRegionFromBeginning	=1, 
    //! Extract the sub-region from the end of the source region	
    MAPSConstRBRegionSubRegionFromEnd		=2,
};

//! Flags for the MAPSRBRegion::BesideRegion method
enum MAPSConstRBRegionBesideRegion 
{
    //! Take the region before the source region	
    MAPSConstRBRegionBesideRegionPrevious	=1, 
    //! Take the region after the source region
    MAPSConstRBRegionBesideRegionNext		=2
};

//! MAPSRBRegion class : Ring-Buffer region manipulation class.
/*! This class allows the user to manipulate regions of ring-buffers.
 *
 *	A region is built around the following variables : 
 *		- offset   : define the start of the region in the ring-buffer.
 *		- size	   : define the size of the region. size=size0+size1
 *		- size0    : define the size of the region up to the end of the ring-buffer.
 *		- size1    : define the size of the region from the beginning of the ring-buffer.
 *		- fifoSize : define the size of the ring-buffer.
 *  \verbatim
	.                             fifoSize					.							
	.  x-------------------------------------------------------------x	.
	. |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|	.
	. |*********                 *************************************|	.
	. |+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++|	.
	.  x-------x                 x-----------------------------------x	.
	.    size1                   |                size0			.
	.                          offset					.
    \endverbatim
 */
class MAPSRBRegion
{
public :
    MAPSConstRBRegionState	state;
    //! define the size of the ring-buffer.
    int	fifoSize;
    //! define the start of the region in the ring-buffer.
    int offset;
    //! define the size of the region.
    int size;

    //! \name Constructors
    //@{
    //! Constructor of the class : just initialize all attributes to 0, and put the region to the MAPSConstRBRegionUninitialized state
    MAPSRBRegion();
    //! Copy constructor of the class. 
    MAPSRBRegion(const MAPSRBRegion &reg);
    //@}

    //! \name Initialization routines
    //@{
    //! Initialize the region with default values
    /*! \param fifoSizex The size of the ring-buffer
     *	\param offsetx The start of the region in the ring-buffer.
     *	\param sizex The region size
     *	\return Returns the state of the defined region.
     */
    MAPSConstRBRegionState Init(int fifoSizex,int offsetx,int sizex);
    //! Reset the region : offset=0, size=0
    MAPSConstRBRegionState Reset();
    //! Fill entirely the region from current offset.
    MAPSConstRBRegionState Full();
    //@}

    //! \name Attribute set methods.
    //@{
    //! Set the ring-buffer size of the current region
    MAPSConstRBRegionState SetFifoSize(int value);
    //! Set the size attribute
    MAPSConstRBRegionState SetSize(int value);
    //! Set the offset attribute
    MAPSConstRBRegionState SetOffset(int value);
    //@}
	
    //! \name Attributes or simple get methods
    //@{
    //! Returns the complementary size of the region (fifoSize-size)
    int	 InverseSize() const { return(fifoSize-size);};
    //! Returns the size of the ring-buffer
    int	 FifoSize() const {return(fifoSize);};
    //! Get size attribute
    int	 Size() const {return(size);};
    //! Get size attribute
    int	 Size0() const;
    //! Get size attribute
    int	 Size1() const;
    //! Get offset attribute
    int	 Offset() const {return(offset);};
    //! Get the last offset of the region
    int	 OffsetLastElement() const {if (size!=0) return((offset+size-1)%fifoSize); else return(0);};
    //@}	

    //! \name Operation on the region itself
    //@{
    //! Erosion of a region
    /*! \param type Define if the region must be eroded from its beginning and/or its end
     * \param sizex Size of the erosion element.
     * \return The return value is the state of the region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>
     */
    MAPSConstRBRegionState Erode(MAPSConstRBRegionErosion type, int sizex);
    //! Dilation of a region
    /*! \param type Define if the region must be dilated from its beginning and/or its end
     * \param sizex Size of the dilation element.
     * \return The return value is the state of the region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>
     */
    MAPSConstRBRegionState Dilate(MAPSConstRBRegionDilation type,int sizex);
    //! Crop a region.
    /*! \param type Define if the region must be croped from its beginning or its end
     *  \param offsetX Define an offset from the starting point (beginning or end).
     * \param sizex Size of the new region (cropped).
     * \return The return value is the state of the new region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>
     */
    MAPSConstRBRegionState Crop(MAPSConstRBRegionCrop type,int offsetX,int sizex);
    //@}
    //! \name Transformation of the region into a new one
    //@{
    //! Extract a sub-region from current region.
    /*! \param dest The destination region.
     *	\param type Define if the region must be extracted from the beginning or the end of src region.
     *  \param offsetX Define an offset from the starting point (beginning or end).
     *  \param sizex Size of the new region.
     *  \return The return value is the state of the new region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>
     */
    MAPSConstRBRegionState SubRegion(MAPSRBRegion &dest, MAPSConstRBRegionSubRegion type,int offsetX,int sizex) const;
    //! Define a new region next to current region.
    /*! \param dest The destination region.
     *	\param type Define if the region must be taken from before or after the src region.
     *  \param offsetX Define an offset from the starting point (beginning or end).
     *  \param sizex Size of the new region.
     *  \return The return value is the state of the new region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>
     */
    MAPSConstRBRegionState BesideRegion(MAPSRBRegion &dest, MAPSConstRBRegionBesideRegion type,int offsetX,int sizex) const;
    //! Define the complementary region of the current region (inversion).
    /*! \param dest The destination region.
     *  \return The return value is the state of the new region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>
     */
    MAPSConstRBRegionState InverseRegion(MAPSRBRegion &dest) const;
    //@}
    //! \name Direct-access operations : Push, Pop, etc...
    //@{
    //! Push some data in the ring-buffer (FIFO like operation)
    /*! \param nbData The number of element your want to push in the FIFO. nbData will contain the real number of pushed data in the region.
     *  \param destSubregion Will contain the sub-region that must receive data to push.
     *  \param enableOverwrite If set to true, a push with nbData too big will overwrite FIFO data. Else, nbData is adjusted to the maximum value without overwrite.
     *  \return The return value is the state of the new region, if the operation succeeded. An overflow will return <CODE>MAPSConstRBRegionOverflowOp</CODE>. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>.
     !*/
    MAPSConstRBRegionState  Push(int nbData,MAPSRBRegion &destSubregion,bool enableOverwrite=true);
    //! Pop some data from the ring-buffer (FIFO like operation)
    /*! \param nbData The number of element your want to pop from the FIFO
     *  \param srcSubregion Will contain the sub-region that contains data to pop.
     *  \return The return value is the state of the new region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>.
     */
    MAPSConstRBRegionState  Pop(int nbData,MAPSRBRegion *srcSubregion=NULL);
    //! Dequeues some data from the ring-buffer (STACK like operation). The data is removed from the last
    /*! \param nbData The number of element your want to popback from the stack
     *  \param srcSubregion Will contain the sub-region that contains data to popback.
     *  \return The return value is the state of the new region, if the operation succeeded. Else it returns <CODE>MAPSConstRBRegionInvalidOp</CODE>.
     */
    MAPSConstRBRegionState   Popback(int nbData, MAPSRBRegion *srcSubregion=NULL);
    //@}
    //! Check that the region is consistent and returns its state.
    MAPSConstRBRegionState		Check() const;

    //! \name Templates
    //@{
    //! A template function designed to copy the content of a circular buffer to a temporary linear buffer.
    template<class T> int	CopyFromRB(const T *fifoData,T *dest,int destMaxSize) const
    {
	//	if (Check())
	{
	    int cpSize=MIN(destMaxSize,size);
	    if (cpSize>0)
	    {
		int cpSize0;
		cpSize0=MIN(cpSize,fifoSize-offset);
		MAPS::Memcpy(dest,fifoData+offset,sizeof(T)*cpSize0);
		if (cpSize!=cpSize0)
		{
		    int cpSize1=cpSize-cpSize0;
		    MAPS::Memcpy(dest+cpSize0,fifoData,sizeof(T)*cpSize1);
		}
	    }
	    return(cpSize);
	}
	return(0);
    };
    //! A template function designed to copy the content of a temporary linear buffer to a circular buffer region
    template<class T> int	CopyToRB(T *fifoData,const T *src,int srcMaxSize)
    {
	//	if (Check())
	{
	    int cpSize=MIN(srcMaxSize,size);
	    if (cpSize>0)
	    {
		int cpSize0=MIN(cpSize,fifoSize-offset);
		MAPS::Memcpy(fifoData+offset,src,sizeof(T)*cpSize0);
		if (cpSize!=cpSize0)
		{
		    int cpSize1=cpSize-cpSize0;
		    MAPS::Memcpy(fifoData,src+cpSize0,sizeof(T)*cpSize1);
		}
	    }
	    return(cpSize);
	}
	return(0);
    };
    //! A template function designed to duplicate an element t of T into the whole region of dest
    template<class T> int	DuplicateToRB(T *fifoData,T data)
    {
	//	if (Check())
	if (size>0)
	{
	    int offsetMax=MIN(offset+size,fifoSize);
	    int i;
	    for(i=offset;i<offsetMax;i++)
		fifoData[i]=data;
	    if (offsetMax-offset<size)
	    {
		int size1=size-(offsetMax-offset);
		for(i=0;i<size1;i++)
		    fifoData[i]=data;
	    }
	    return(size);
	}
	return(0);
    };
    //! A template function designed to duplicate an element t of T into the whole region of dest with regular increments.
	/*!
	 * Usually used for timestamp data with frequency information (so we duplicate first the original timestamp, then increment it each step).
	 */
    template<class T> int	DuplicateToRB2(T *fifoData,T data,T increment)
    {
	//	if (Check())
	if (size>0)
	{
	    int offsetMax=MIN(offset+size,fifoSize);
	    int i;
	    T data2=data;
	    for(i=offset;i<offsetMax;i++)
	    {
		fifoData[i]=data2;
		data2+=increment;
	    }
	    if (offsetMax-offset<size)
	    {
		int size1=size-(offsetMax-offset);
		for(i=0;i<size1;i++)
		{
		    fifoData[i]=data2;
		data2+=increment;
		}
	    }
	    return(size);
	}
	return(0);
    };
    //! A template function designed to duplicate an element t of T into the whole region of dest with regular increments but with a minimum value.
	/*!
	 * Usually used for timestamp data with frequency information and monotonous constraints 
	 * The min_data_value parameter should then be the last timestamp duplicated to the RB at the previous call to DuplicateToRB3.
	 * So we duplicate first the original timestamp, then increment it each step, provided it is not lower that min_data_value. 
	 * Otherwise, the current value is bounded to min_data_value.
	 * WARNING: when using with timestamp, this can be nice for real-time operation, but problematic in replay mode when jumping backwards in time.
	 */
	template<class T> int	DuplicateToRB3(T *fifoData,T data,T increment, T min_data_value)
	{
		//	if (Check())
		if (size>0)
		{
			int offsetMax=MIN(offset+size,fifoSize);
			int i;
			T data2, data3=data;
			for(i=offset;i<offsetMax;i++)
			{
				data2 = data3;
				if( data2 > min_data_value)
					data2 = min_data_value;
				fifoData[i]=data2;
				data3+=increment;
			}
			if (offsetMax-offset<size)
			{
				int size1=size-(offsetMax-offset);
				for(i=0;i<size1;i++)
				{
					data2 = data3;
					if (data2 > min_data_value)
						data2 = min_data_value;
					fifoData[i]=data2;
					data3+=increment;
				}
			}
			return(size);
		}
		return(0);
	};

    //@}
    //! A very important operator when manipulating regions
    /*! This operator returns the offset  <CODE>i</CODE> of a given region. If  <CODE>i</CODE>
     *  is an incorrect offset and in debug mode, this method generate an <CODE>assert</CODE>.
     *	Otherwise, it returns the direct offset in the ring-buffer of the  <CODE>i</CODE> element
     *   of the current region.
     */
    int operator[](int i) const ;
    
    //! A very useful function to convert state into strings
    static const MAPSString StateToString(const MAPSConstRBRegionState &st);
    
};

#endif
