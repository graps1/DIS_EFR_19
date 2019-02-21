#pragma once

#include "NHD_US2066.h"
#include "Metro.h"
#include "menu.hpp"

#define CLEAR "                     "

namespace display
{
class Display
{
  private:
    String _content[4]{CLEAR, CLEAR, CLEAR, CLEAR};
    bool new_content;
    uint8_t no_lines;
  public:
    void setContent(String content[]);
    void updateScreen();
};

} // namespace display
