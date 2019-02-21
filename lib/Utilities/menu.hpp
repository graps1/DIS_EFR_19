#pragma once

#include "Metro.h"
#include "Arduino.h"

#define MENU_IDLE_TIMEOUT 5000

namespace menu
{


enum SelectType {
	FirstChild,
	Parent,
	Root,
	Default
};

class Menu
{
  private:
	Menu *child;
	Menu *parent;
	Menu *neighbour;
	void (*_display)();
	SelectType (*_on_select)();

  public:
	Menu(void (*display)(),
		 SelectType (*on_select)() = [] { return SelectType::Default; },
		 Menu *parent = NULL);

	Menu *select();
	void addSubmenu(Menu *menu);
	Menu *root();
	Menu *getParent();
	Menu *advance();
	void display();
	bool hasParent();
	Menu *update();
};
} // namespace menu
