#include "display.hpp"

namespace display
{

void Display::setContent(String content[])
{
    int len = sizeof(content)/sizeof(*content);
    for (uint8_t i = 0; i < 4; i++)
    {
        String line = i < len ? content[i] : CLEAR;
        if (_content[i] != line)
        {
            _content[i] = line;
            new_content = true;
        }
    }
}

void Display::updateScreen()
{
    if (!new_content)
        return;

    for (int i = 0; i < no_lines; i++)
    {
        setCursor(i);
        writeRow(_content[i]);
    }

    new_content = false;
}



} // namespace display
