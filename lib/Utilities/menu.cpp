#include "menu.hpp"
#include "buttons.hpp"

namespace menu {

Metro menu_idle_timeout_tmr = Metro(MENU_IDLE_TIMEOUT);
buttons::ButtonsToBit buttons, buttons_up, buttons_down;

Menu::Menu(void (*display)(), SelectType (*on_select)(), Menu *parent)
{
	this->_display = display;
	this->_on_select = on_select;
	this->neighbour = this;
	this->parent = parent;

	if (this->hasParent())
		this->parent->addSubmenu(this);
}

Menu* Menu::select()
{
	SelectType select_type = this->_on_select();
	
	if (select_type == SelectType::FirstChild && this->child)
		return this->child;
	else if (select_type == SelectType::Parent && this->parent)
		return this->parent;
	else if (select_type == SelectType::Root)
		return this->root();
	else if (select_type == SelectType::Default)
    	return this;

	return this;
}

// wäre viel leichter mit einem vector zu implementieren, aber 
// die c++ standardlib wird nicht von teensy unterstützt
void Menu::addSubmenu(Menu *menu)
{
	if (!child) {
		child = menu;
	} else {
		Menu* current = child;
		while (current->neighbour != child) {
			current = current->neighbour;
		}

		current->neighbour = menu;
		menu->neighbour = child;
	}

	menu->parent = this;
}

Menu* Menu::root()
{
    Menu *target = this;
	while (target->parent)
	{
		target = target->parent;
	}
	return target;
}

Menu* Menu::getParent()
{
    return this->parent;
}

Menu* Menu::update()
{
	buttons::updateButtonStates(&buttons, &buttons_up, &buttons_down);

	bool ok = buttons.LR1;
	bool next = buttons.LR2;
	bool ret = buttons.LR3;
	bool next_up = buttons_up.LR2;
	bool ok_up = buttons_up.LR1;

    if (ok || next)
        menu_idle_timeout_tmr.reset();

    if (ret)
        return getParent();

    if (menu_idle_timeout_tmr.check())
        return root();

    if (next_up)
        return advance();

    if (ok_up)
    	return select();
    
	return this;
}

Menu* Menu::advance() { return this->neighbour; }
void Menu::display() { this->_display(); }
bool Menu::hasParent() { return this->parent; }

}