#pragma once

#include "Arduino.h"

#define Button_Start 13
#define Button_HV 14
#define Button_Reku 15		
#define Button_1 1
#define Button_2 2
#define Button_3 0
#define Button_4 10
#define Button_5 5 
#define Button_6 6
#define Poti_1 16
#define Poti_2 17


namespace buttons {

union ButtonsToBit {
	struct
	{
		unsigned char LR6 : 1;	// failure menu
		unsigned char LR5 : 1;	// ++
		unsigned char LR4 : 1;	// --
		unsigned char LR3 : 1;	// ret
		unsigned char LR2 : 1;	// next
		unsigned char LR1 : 1;	// ok
		unsigned char Reku : 1;
		unsigned char HV : 1;
		unsigned char Start : 1;
	};
	uint16_t buf;
};

struct ButtonsToBitStamped {
	ButtonsToBit btns;
	int stamp_millis;
};

void initButtons();

uint16_t getPoti(uint8_t poti);

void updateButtonStates();
void getButtonStates(
	ButtonsToBitStamped* buttons = NULL, 
	ButtonsToBitStamped* buttons_up = NULL, 
	ButtonsToBitStamped* buttons_down = NULL);

int getNegBtn(ButtonsToBit buttons);
int getPosBtn(ButtonsToBit buttons);
int getNextBtn(ButtonsToBit buttons);
int getOKBtn(ButtonsToBit buttons);
int getFailBtn(ButtonsToBit buttons);
int getRetBtn(ButtonsToBit buttons);

}


