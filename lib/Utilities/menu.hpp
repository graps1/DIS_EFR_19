#pragma once

#include "Metro.h"
#include "Arduino.h"

#define MENU_IDLE_TIMEOUT 25000

namespace menu
{

enum SelectType
{
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
		 Menu *parent = NULL,
		 SelectType (*on_select)() = [] { return SelectType::FirstChild; });

	Menu *select();
	void addSubmenu(Menu *menu);
	void addNeighbour(Menu *menu);
	Menu *root();
	Menu *getParent();
	Menu *advance();
	void display();
	bool hasParent();
	Menu *update();
};
} // namespace menu
