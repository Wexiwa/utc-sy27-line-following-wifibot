////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _maps_mfc_sample_H
#define _maps_mfc_sample_H

#ifndef _WINVER
#define WINVER 0x0500
#endif

#include <afxwin.h>
#include <mmsystem.h>
// Includes maps sdk library header
#include "maps.hpp"

#include "maps_mfc_resource.h"
#include "maps_mfc_slider_dialog.h"

// Declares a new MAPSComponent child class
class MAPSMFCSample : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSMFCSample)
private :
    // Place here your specific methods and attributes
    CDialog *mpMyDialog;
};

#endif
