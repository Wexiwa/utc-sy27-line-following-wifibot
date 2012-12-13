////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_ComponentTemplate.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSComponentTemplate)
    //MAPS_INPUT("iName",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSComponentTemplate)
    //MAPS_OUTPUT("oName",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSComponentTemplate)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSComponentTemplate)
    //MAPS_ACTION("aName",MAPSComponentTemplate::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (ComponentTemplate) behaviour
MAPS_COMPONENT_DEFINITION(MAPSComponentTemplate,"ComponentTemplate","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  0, // Nb of inputs
			  0, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSComponentTemplate::Birth()
{
    // Reports this information to the RTMaps console
    ReportInfo("ComponentTemplate: Passing through Birth() method");
}

void MAPSComponentTemplate::Core() 
{
    // Reports this information to the RTMaps console
    ReportInfo("ComponentTemplate: Passing through Core() method");
    // Sleeps during 500 milliseconds (500000 microseconds)
    Rest(500000);
}

void MAPSComponentTemplate::Death()
{
    // Reports this information to the RTMaps console
    ReportInfo("ComponentTemplate: Passing through Death() method");
}
