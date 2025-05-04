#ifndef RENDER_H_
#define RENDER_H_

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <ncurses.h>

#include "types.h"

void draw_screen(menu_screen_t *screen);
void draw_input_popup(menu_item_t *item);

#endif //RENDER_H_
