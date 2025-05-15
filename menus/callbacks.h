#ifndef MENU_CALLBACKS_H
#define MENU_CALLBACKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menus/appmenu.h"
#include "types.h"

extern menu_screen_t **app_screen;
extern menu_t *menu_base;
extern menu_t *menu_target;
extern menu_t *menu_batch;
extern menu_t *menu_flavours;
extern menu_t *menu_submit;

extern MixInputs mix_inputs;
extern MixData mix_data;

extern int nic_by_weight;


void load_btn_cb(void *screen);
void save_btn_cb(void *screen);
void base_pg_cb(void *screen);
void base_vg_cb(void *screen);
void target_pg_cb(void *screen);
void target_vg_cb(void *screen);
void by_vol_cb(void *screen);
void by_weight_cb(void *screen);
void flavour_input_cb(void *screen);
void add_flavour_cb(void *screen);
void remove_flavour_cb(void *screen);
void submit_recipe_cb(void *screen);

#endif //MENU_CALLBACKS_H
