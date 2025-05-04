#include "input.h"



void item_on_enter(menu_screen_t *screen, menu_item_t *item)
{
  switch(item->type){
  case ITEM_TYPE_INPUT:
    draw_input_popup(item);
    if(item->cb)
      item->cb(screen);
    break;
  case ITEM_TYPE_BUTTON:
    if(item->cb)
      item->cb(screen);
    break;
  default:
    return;
  }
}
