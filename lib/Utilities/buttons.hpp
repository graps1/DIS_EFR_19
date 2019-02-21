#pragma once

#include "Arduino.h"
#define Button_HV 10
#define Button_Start 11
#define Button_Reku 12
#define Button_1 2
#define Button_2 1
#define Button_3 0
#define Button_4 5
#define Button_5 6
#define Button_6 13

namespace buttons {

union ButtonsToBit {
	uint16_t buf;
	struct
	{
		unsigned char LR6 : 1;
		unsigned char LR5 : 1;
		unsigned char LR4 : 1;
		unsigned char LR3 : 1;
		unsigned char LR2 : 1;
		unsigned char LR1 : 1;
		unsigned char Reku : 1;
		unsigned char HV : 1;
		unsigned char Start : 1;
	};
};

void updateButtonStates(
	ButtonsToBit* buttons, 
	ButtonsToBit* buttons_up, 
	ButtonsToBit* buttons_down);

}