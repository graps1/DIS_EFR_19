#include "display.hpp"

#define NO_LINES 4
#define SCREEN_LENGTH 20

namespace display
{

String _content[4]{CLEAR, CLEAR, CLEAR, CLEAR};
bool new_content;

void init()
{
    init_oled();
}

// Tries to fit two strings as best as possible into the same line
String fit(String a, String b)
{
    int al = a.length(), bl = b.length();
    if (al < SCREEN_LENGTH * 0.5 && bl < SCREEN_LENGTH * 0.5)
    {
        for (int i = al; i < SCREEN_LENGTH * 0.5; i++)
            a += " ";
        return a + b;
    }
    return a + " " + b;
}

void setContent(String _1, String _2, String _3, String _4)
{
    String lines[] = {_1, _2, _3, _4};
    for (uint8_t i = 0; i < NO_LINES; i++)
    {
        String line = lines[i] + CLEAR;
        if (_content[i] != line)
        {
            _content[i] = line;
            new_content = true;
        }
    }
}

void updateScreen()
{
    if (!new_content)
        return;

    for (int i = 0; i < NO_LINES; i++)
    {
        setCursor(i);
        writeRow(_content[i]);
    }

    new_content = false;
}

String getContent()
{
    String ret = "";
    for (int i = 0; i < NO_LINES; i++)
    {
        ret += _content[i];
    }
    return ret;
}

} // namespace display
