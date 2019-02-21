#include "buttons.hpp"

namespace buttons {



void updateButtonStates(
	ButtonsToBit* buttons, 
	ButtonsToBit* buttons_up, 
	ButtonsToBit* buttons_down)
{
	ButtonsToBit next_btn_state;
	next_btn_state.LR1 = !(digitalRead(Button_1));
	next_btn_state.LR2 = !(digitalRead(Button_2));
	next_btn_state.LR3 = !(digitalRead(Button_3));
	next_btn_state.LR4 = !(digitalRead(Button_4));
	next_btn_state.LR5 = !(digitalRead(Button_5));
	next_btn_state.LR6 = !(digitalRead(Button_6));
	next_btn_state.HV = !(digitalRead(Button_HV));
	next_btn_state.Reku = !(digitalRead(Button_Reku));
	next_btn_state.Start = !(digitalRead(Button_Start));

	buttons_up->buf = buttons->buf & ~next_btn_state.buf;
	buttons_down->buf = next_btn_state.buf & ~buttons->buf;
	*buttons = next_btn_state;
}

}