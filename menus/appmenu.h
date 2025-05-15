#ifndef APP_MENU_H_
#define APP_MENU_H_

#include <stdio.h>

#include "render.h"
#include "types.h"
#include "calcs.h"
#include "constants.h"
#include "callbacks.h"
#include "save.h"

typedef struct
{
  menu_screen_t *screen;
  section_t *section_batch;
  section_t *section_nic_base;
  section_t *section_target;
  section_t *section_flavours;
  section_t *section_recipe;
  section_t *section_save;
}AppMenu_t;


menu_screen_t **create_app_menu();



#endif //APP_MENU_H_
