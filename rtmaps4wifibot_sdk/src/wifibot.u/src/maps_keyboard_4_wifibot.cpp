////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_keyboard_4_wifibot.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSkeyboard_4_wifibot)
    MAPS_INPUT("input_code",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSkeyboard_4_wifibot)
    MAPS_OUTPUT("left_and_right_motor_speeds",MAPS::Integer,NULL,NULL,2)
	MAPS_OUTPUT("left_motor_speed",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("right_motor_speed",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSkeyboard_4_wifibot)
    MAPS_PROPERTY_ENUM("output_mode","Vectorized|Separated left/right speeds",0,false,false)
	MAPS_PROPERTY("input_code_1",200,false,false)
	MAPS_PROPERTY("input_code_1_left_speed",40,false,false)
	MAPS_PROPERTY("input_code_1_right_speed",40,false,false)
	MAPS_PROPERTY("input_code_2",208,false,false)
	MAPS_PROPERTY("input_code_2_left_speed",-40,false,false)
	MAPS_PROPERTY("input_code_2_right_speed",-40,false,false)
	MAPS_PROPERTY("input_code_3",203,false,false)
	MAPS_PROPERTY("input_code_3_left_speed",-30,false,false)
	MAPS_PROPERTY("input_code_3_right_speed",30,false,false)
	MAPS_PROPERTY("input_code_4",205,false,false)
	MAPS_PROPERTY("input_code_4_left_speed",30,false,false)
	MAPS_PROPERTY("input_code_4_right_speed",-30,false,false)
	MAPS_PROPERTY("input_code_5",32,false,false)
	MAPS_PROPERTY("input_code_5_left_speed",0,false,false)
	MAPS_PROPERTY("input_code_5_right_speed",0,false,false)
	MAPS_PROPERTY_ENUM("any_other_code","Ignore|Stop the robot",0,false,false)
	MAPS_PROPERTY("nb_additional_configs",0,false,false)
MAPS_END_PROPERTIES_DEFINITION

#define FIRST_DYN_PROP 18

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSkeyboard_4_wifibot)
    //MAPS_ACTION("aName",MAPSkeyboard_4_wifibot::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (keyboard_4_wifibot) behaviour
MAPS_COMPONENT_DEFINITION(MAPSkeyboard_4_wifibot,"keyboard_4_wifibot","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  0, // Nb of outputs
			  FIRST_DYN_PROP, // Nb of properties
			  0) // Nb of actions

void MAPSkeyboard_4_wifibot::Dynamic()
{
	_output_mode = (int)GetIntegerProperty("output_mode");
	_nb_confs = (int)GetIntegerProperty("nb_additional_configs");

	switch(_output_mode) {
		case 0:
			NewOutput(0);
			break;
		case 1:
			NewOutput(1);
			NewOutput(2);
			break;
	}

	for (int i=0; i<_nb_confs; i++) {
		MAPSStreamedString name1("input_code_");
		name1 << 6+i;
		MAPSStreamedString name2("input_code_");
		name2 << 6+i << "_left_speed";
		MAPSStreamedString name3("input_code_");
		name3 << 6+i << "_right_speed";
		NewProperty("input_code_1",name1);
		NewProperty("input_code_1_left_speed",name2);
		NewProperty("input_code_1_right_speed",name3);
	}
}

void MAPSkeyboard_4_wifibot::Birth()
{
	_default_action = (int)GetIntegerProperty("any_other_code");
	_config1.code = (int)GetIntegerProperty("input_code_1");
	_config1.left = (int)GetIntegerProperty("input_code_1_left_speed");
	_config1.right = (int)GetIntegerProperty("input_code_1_right_speed");
	_config2.code = (int)GetIntegerProperty("input_code_2");
	_config2.left = (int)GetIntegerProperty("input_code_2_left_speed");
	_config2.right = (int)GetIntegerProperty("input_code_2_right_speed");
	_config3.code = (int)GetIntegerProperty("input_code_3");
	_config3.left = (int)GetIntegerProperty("input_code_3_left_speed");
	_config3.right = (int)GetIntegerProperty("input_code_3_right_speed");
	_config4.code = (int)GetIntegerProperty("input_code_4");
	_config4.left = (int)GetIntegerProperty("input_code_4_left_speed");
	_config4.right = (int)GetIntegerProperty("input_code_4_right_speed");
	_config5.code = (int)GetIntegerProperty("input_code_5");
	_config5.left = (int)GetIntegerProperty("input_code_5_left_speed");
	_config5.right = (int)GetIntegerProperty("input_code_5_right_speed");

	_additional_configs = NULL;
	if (_nb_confs > 0) {
		_additional_configs = new CodeConfig[_nb_confs];
		for (int i=0; i<_nb_confs; i++) {
			_additional_configs[i].code = (int)GetIntegerProperty(FIRST_DYN_PROP + 3*i);
			_additional_configs[i].left = (int)GetIntegerProperty(FIRST_DYN_PROP + 3*i + 1);
			_additional_configs[i].right = (int)GetIntegerProperty(FIRST_DYN_PROP + 3*i + 2);
		}
	}


}

void MAPSkeyboard_4_wifibot::Core() 
{
	MAPSIOElt* ioeltin = StartReading(Input(0));
	if (ioeltin == NULL)
		return;

	int code = ioeltin->Integer();

	int left = 0;
	int right = 0;
	bool discard = false;
	if (code == _config1.code) {
		left = _config1.left;
		right = _config1.right;
	} else if (code == _config2.code) {
		left = _config2.left;
		right = _config2.right;
	} else if (code == _config3.code) {
		left = _config3.left;
		right = _config3.right;
	} else if (code == _config4.code) {
		left = _config4.left;
		right = _config4.right;
	} else if (code == _config5.code) {
		left = _config5.left;
		right = _config5.right;
	} else {
		bool found = false;
		for (int i=0; i< _nb_confs; i++) {
			if (code == _additional_configs[i].code) {
				found = true;
				left = _additional_configs[i].left;
				right = _additional_configs[i].right;
				break;
			}
		}
		if (false == found) {
			if (_default_action == 0) { //ignore
				discard = true;
			}
		}
	}

	if (false == discard) {
		if (_output_mode == 0) { //vectorized
			MAPSIOElt* ioeltout = StartWriting(Output(0));
			ioeltout->Integer() = left;
			ioeltout->Integer(1) = right;
			ioeltout->Timestamp() = ioeltin->Timestamp();
			StopWriting(ioeltout);
		} else { //splitted outputs
			MAPSIOElt* ioeltout = StartWriting(Output(0));
			MAPSIOElt* ioeltout2 = StartWriting(Output(1));
			ioeltout->Integer() = left;
			ioeltout2->Integer() = right;
			ioeltout->Timestamp() = ioeltin->Timestamp();
			ioeltout2->Timestamp() = ioeltin->Timestamp();
			StopWriting(ioeltout2);
			StopWriting(ioeltout);
		}
	}
}

void MAPSkeyboard_4_wifibot::Death()
{
	if (_additional_configs) {
		delete [] _additional_configs;
		_additional_configs = NULL;
	}
}