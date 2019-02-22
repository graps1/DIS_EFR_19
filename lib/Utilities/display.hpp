#pragma once

#include "NHD_US2066.h"
#include "Metro.h"
#include "menu.hpp"

#define CLEAR "                     "
#define NO_LINES 4


namespace display
{
    void setContent(String content[], int arrlen);
    void updateScreen();
    String getContent();
    void init();
} // namespace display
