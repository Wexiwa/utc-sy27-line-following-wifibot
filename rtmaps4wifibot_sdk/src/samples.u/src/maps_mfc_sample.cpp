////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module : Demonstrates how to use MFC in a RTMaps component
////////////////////////////////

#include "maps_mfc_sample.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSMFCSample)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSMFCSample)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSMFCSample)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSMFCSample)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component behaviour
MAPS_COMPONENT_DEFINITION(MAPSMFCSample,"MFCSample","1.0",128,
			  MAPS::Threaded,MAPS::Threaded, 0,0,0,0)

void MAPSMFCSample::Birth()
{
}

void MAPSMFCSample::Core() 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    mpMyDialog = new CDialog(IDD_PROPPAGE_MEDIUM);
    mpMyDialog->DoModal();
}

void MAPSMFCSample::Death()
{
}
