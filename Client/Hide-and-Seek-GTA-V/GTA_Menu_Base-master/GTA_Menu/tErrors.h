#ifndef ERRORS_H
#define ERRORS_H

#include "Menu.h"

#if MAX_PP % 2 == 0
#error Menu.h: MAX_PP MUST BE AN ODD NUMBER
#endif

#if MENU_COUNT > MAX_MENUS
#error Menu.h: TOO MANY MENUS INCREASE MAX_MENUS BUFFER SIZE TO FIX THIS ERROR
#endif

#endif