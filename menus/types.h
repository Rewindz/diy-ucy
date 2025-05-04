#ifndef MENU_TYPES_H_
#define MENU_TYPES_H_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


typedef void (* menu_item_callback_t)(void *); // menu_screen_t ptr idk how to forward declare this lol

typedef enum
{
  ITEM_TYPE_INPUT,
  ITEM_TYPE_BUTTON,
  ITEM_TYPE_LABEL
}item_type_t;

typedef struct
{
  item_type_t type;
  char *label;
  char *value;
  menu_item_callback_t cb;
}menu_item_t;

typedef struct
{
  int item_count;
  int selected_item;
  menu_item_t **items;
}menu_t;

typedef struct
{
  char *title;
  menu_t *menu;
}section_t;

typedef struct
{
  int count;
  int selected_section;
  section_t **sections;
}menu_screen_t;

void free_menu_item_arr(int count, menu_item_t **items);
void free_whole_menu(menu_t *menu);
void free_whole_section(section_t *section);
void free_whole_screen(menu_screen_t *screen);

menu_item_t *create_new_menu_item(item_type_t type, const char* label);
menu_t *create_new_menu(int count, ...);
section_t *create_new_section(const char *title, menu_t *menu);
menu_screen_t *create_new_menu_screen(int count, ...);
menu_item_t *get_selected_item(menu_screen_t *screen);
void replace_menu(menu_screen_t *screen, menu_t *old, menu_t *new);


void add_item_callback(menu_item_t *item, menu_item_callback_t cb);
void remove_item_callback(menu_item_t* item);

void move_screen_down(menu_screen_t *screen);
void move_screen_up(menu_screen_t *screen);
void move_screen_left(menu_screen_t *screen);
void move_screen_right(menu_screen_t *screen);


#endif //MENU_TYPES_H_
