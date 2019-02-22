#include "display.hpp"

namespace display
{

String _content[4]{CLEAR, CLEAR, CLEAR, CLEAR};
bool new_content;

void init() {
    init_oled();
}

void setContent(String content[], int arrlen)
{
    for (uint8_t i = 0; i < NO_LINES; i++)
    {
        String line = i < arrlen ? content[i] + CLEAR: CLEAR;
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

String getContent() {
    String ret = "";
    for (int i=0; i<NO_LINES; i++) {
        ret += _content[i];
    }
    return ret;
}



} // namespace display
