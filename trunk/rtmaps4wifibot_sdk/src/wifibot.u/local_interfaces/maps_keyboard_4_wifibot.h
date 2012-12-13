////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_keyboard_4_wifibot_H
#define _Maps_keyboard_4_wifibot_H

// Includes maps sdk library header
#include "maps.hpp"

typedef struct CodeConfig
{
	int code;
	int left;
	int right;
} CodeConfig;

// Declares a new MAPSComponent child class
class MAPSkeyboard_4_wifibot : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSkeyboard_4_wifibot)
	void Dynamic();
private :
	// Place here your specific methods and attributes
	int _nb_confs;
	int _output_mode;
	int _default_action;

	CodeConfig _config1, _config2, _config3, _config4, _config5;
	CodeConfig* _additional_configs;
};

#endif
