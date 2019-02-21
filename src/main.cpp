#include "Arduino.h"
#include "Metro.h"
#include "can.hpp"
#include "menu.hpp"
#include "display.hpp"
#include "buttons.hpp"

Metro timer_display = Metro(100);
display::Display dis;
menu::Menu* currentMenu;

menu::Menu mainMenu([] { 
	String content[4] = { "hallo", "welt", "das ist", "das hauptmenü" };
	dis.setContent(content);
}, [] {
	return menu::SelectType::FirstChild;
});

menu::Menu submenu1([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenü 1" };
	dis.setContent(content);
}, [] {
	return menu::SelectType::FirstChild; 
}, &mainMenu);

menu::Menu submenu11([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenü 11" };
	dis.setContent(content);
}, [] {
	return menu::SelectType::Root; 
}, &submenu1);

menu::Menu submenu0([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenü 0" };
	dis.setContent(content);
}, [] {
	return menu::SelectType::FirstChild; 
}, &mainMenu);

menu::Menu submenu01([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenü 01" };
	dis.setContent(content);
}, [] {
	return menu::SelectType::Root; 
}, &submenu0);

menu::Menu submenu02([] { 
	String content[4] = { "hallo", "welt", "das ist", "das untermenü 02" };
	dis.setContent(content);
}, [] {
	return menu::SelectType::Root; 
}, &submenu0);


void setup() {
	currentMenu = &mainMenu;
}

void loop()
{
	if (timer_display.check()) {
		// change the menu if a specific button was pressed
		currentMenu = currentMenu->update();
		// update the displays content from the selected menu
		currentMenu->display();
		// write new content to screen
		dis.updateScreen();
	}
}