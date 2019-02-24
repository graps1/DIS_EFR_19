#pragma once

#include "NHD_US2066.h"
#include "Metro.h"
#include "menu.hpp"

#define CLEAR "                     "


namespace display
{
    void setContent(
        String _1 = CLEAR, 
        String _2 = CLEAR, 
        String _3 = CLEAR, 
        String _4 = CLEAR
    );
    void updateScreen();
    String getContent();
    void init();
    String fit(String a, String b);
} // namespace display
