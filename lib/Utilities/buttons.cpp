#include "buttons.hpp"

namespace buttons {

void initButtons() {
	pinMode(Button_1, INPUT);
	pinMode(Button_2, INPUT);
	pinMode(Button_3, INPUT);
	pinMode(Button_4, INPUT);
	pinMode(Button_5, INPUT);
	pinMode(Button_6, INPUT);
	pinMode(Button_HV, INPUT_PULLUP);
	pinMode(Button_Reku, INPUT);
	pinMode(Button_Start, INPUT_PULLUP);
}

ButtonsToBit getCurrentState() {
	ButtonsToBit current_btn_state;
	current_btn_state.LR1 = !(digitalRead(Button_1));
	current_btn_state.LR2 = !(digitalRead(Button_2));
	current_btn_state.LR3 = !(digitalRead(Button_3));
	current_btn_state.LR4 = !(digitalRead(Button_4));
	current_btn_state.LR5 = !(digitalRead(Button_5));
	current_btn_state.LR6 = !(digitalRead(Button_6));
	current_btn_state.HV = !(digitalRead(Button_HV));
	current_btn_state.Reku = !(digitalRead(Button_Reku));
	current_btn_state.Start = !(digitalRead(Button_Start));
	return current_btn_state;
}

void updateButtonStates(
	ButtonsToBit* buttons, 
	ButtonsToBit* buttons_up, 
	ButtonsToBit* buttons_down)
{
	ButtonsToBit next_btn_state = getCurrentState();
	buttons_up->buf = buttons->buf & ~next_btn_state.buf;
	buttons_down->buf = next_btn_state.buf & ~buttons->buf;
	*buttons = next_btn_state;
}

uint16_t getPoti(uint8_t poti) {
	uint8_t pin = 0;
	switch(poti) {
		case 1:
			pin = Poti_1;
			break;
		case 2:
			pin = Poti_2;
			break;
		default:
			return 0;
			break;
	}

	return analogRead(pin) / 4;
}

}