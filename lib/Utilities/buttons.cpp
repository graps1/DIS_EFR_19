#include "buttons.hpp"

namespace buttons {

ButtonsToBitStamped 	_buttons, 
						_buttons_up, 
						_buttons_down;

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

void updateButtonStates() {
	ButtonsToBit next_btn_state = getCurrentState();

	if (_buttons_up.btns.buf != (_buttons.btns.buf & ~next_btn_state.buf)) {
		_buttons_up.btns.buf = _buttons.btns.buf & ~next_btn_state.buf;
		_buttons_up.stamp_millis = millis();
	}
	
	if (_buttons_down.btns.buf != (next_btn_state.buf & ~_buttons.btns.buf)) {
		_buttons_down.btns.buf = next_btn_state.buf & ~_buttons.btns.buf;
		_buttons_down.stamp_millis = millis();
	}

	if (_buttons.btns.buf != next_btn_state.buf) {
		_buttons.btns.buf = next_btn_state.buf;
		_buttons.stamp_millis = millis();
	}
}


void getButtonStates(
	ButtonsToBitStamped* buttons, 
	ButtonsToBitStamped* buttons_up, 
	ButtonsToBitStamped* buttons_down)
{
	if (buttons) *buttons = _buttons;
	if (buttons_up) *buttons_up = _buttons_up;
	if (buttons_down) *buttons_down = _buttons_down;
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