#include "Arduino.h"
#include "Metro.h"
#include "can.hpp"
#include "menu.hpp"
#include "display.hpp"
#include "buttons.hpp"
#include "TLC59116.h"

#define ADDR_LEDs 96

Metro timer_display = Metro(1000);
menu::Menu* currentMenu;
TLC59116 leds = TLC59116(ADDR_LEDs);

#pragma region MENU_DEFINITION

menu::Menu mainMenu([] { 
	String content[4] = { "hallo", "welt", "das ist", "das hauptmenu" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::FirstChild;
});

menu::Menu submenu1([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenu 1" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::FirstChild; 
}, &mainMenu);

menu::Menu submenu11([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenu 11" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::Root; 
}, &submenu1);

menu::Menu submenu0([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenu 0" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::FirstChild; 
}, &mainMenu);

menu::Menu submenu01([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenu 01" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::Root; 
}, &submenu0);

menu::Menu submenu02([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenu 02" };
	display::setContent(content, 4);
}, [] {
	return menu::SelectType::Root; 
}, &submenu0);

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
	if (timer_display.check()) {

		// change the menu if a specific button was pressed
		currentMenu = currentMenu->update();
		// update the displays content from the selected menu
		currentMenu->display();
		// write new content to screen
		display::updateScreen();
		
	}
}