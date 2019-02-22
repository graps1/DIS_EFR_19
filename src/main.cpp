#include "Arduino.h"
#include "Metro.h"
#include "can.hpp"
#include "menu.hpp"
#include "display.hpp"
#include "buttons.hpp"
#include "TLC59116.h"

#define ADDR_LEDs 96

Metro	timer_display = Metro(100), 
		timer_dis_can = Metro(5),
		timer_dis_param_can = Metro(100);

menu::Menu* currentMenu;
TLC59116 leds = TLC59116(ADDR_LEDs);
CAN::VehicleState current_vehicle_state;

#pragma region MENU_DEFINITION

menu::Menu mainMenu([] { 
	String content[4] = { "hallo", "welt", "das ist", "das hauptmenu" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::FirstChild;
});

#pragma endregion


void setup() {
	currentMenu = &mainMenu;
	buttons::initButtons();
	display::init();
	leds.init();
    leds.setStates(ON, ON, ON, 1);
	delay(1000);
    leds.setStates(OFF, OFF, OFF, 1);
}

void loop()
{
	current_vehicle_state = CAN::check();

	if (timer_display.check()) {
		// change the menu if a specific button was pressed
		currentMenu = currentMenu->update();
		// update the displays content from the selected menu
		currentMenu->display();
		// write new content to screen
		display::updateScreen();
	}

	if (timer_dis_can.check())
		CAN::sendStatus();

	if (timer_dis_param_can.check() && CAN::paramChanged())
			CAN::sendParam();
}