#include "types.h"

menu_screen_t *create_new_menu_screen(int count, ...)
{
  if(count <= 0)
    return NULL;

  menu_screen_t *screen = calloc(1, sizeof(menu_screen_t));
  if(!screen)
    return NULL;
  
  va_list args;
  va_start(args, count);
  screen->count = count;
  screen->sections = calloc(count, sizeof(section_t *));
  if(!screen->sections)
    return NULL;
  
  for(int i = 0; i < count; ++i){
    screen->sections[i] = va_arg(args, section_t *);
  }
  va_end(args);
  return screen;
}

section_t *create_new_section(const char *title, menu_t *menu)
{
  section_t *section = calloc(1, sizeof(section_t));
  if(!section)
    return NULL;

  section->title = strdup(title);
  section->menu = menu;
  return section;
}

menu_t *create_new_menu(int count, ...)
{
  menu_t *menu = calloc(1, sizeof(menu_t));
  if(!menu)
    return NULL;
  menu->item_count = count;
  menu->items = calloc(count, sizeof(menu_item_t *));
  if(!menu->items){
    free(menu);
    return NULL;
  }

  va_list args;
  va_start(args, count);
  for(int i = 0; i < count; ++i)
    menu->items[i] = va_arg(args, menu_item_t *);
  
  va_end(args);

  return menu;
}

menu_item_t *create_new_menu_item(item_type_t type, const char *label)
{
  menu_item_t *item = calloc(1, sizeof(menu_item_t));
  if(!item)
    return NULL;

  item->type = type;
  item->value = calloc(512, sizeof(char));
  item->label = strdup(label);
  
  return item;
}

void replace_menu(menu_screen_t *screen, menu_t *old, menu_t *new)
{
  if(!screen || !old || !new)
    return;

  section_t *section = NULL;
  for (int i = 0; i < screen->count; ++i){
    if(screen->sections[i]->menu == old)
      section = screen->sections[i];
  }

  free_whole_menu(old);
  section->menu = new;
  
}

void free_whole_screen(menu_screen_t *screen)
{
  for(int i = 0; i < screen->count; ++i){
    free_whole_section(screen->sections[i]);
  }
  free(screen);
}

void free_whole_section(section_t *section)
{
  free_whole_menu(section->menu);
  free(section->title);
  free(section);
}

void free_whole_menu(menu_t *menu)
{
  free_menu_item_arr(menu->item_count, menu->items);
  free(menu);
}


void free_menu_item_arr(int count, menu_item_t **items)
{
  for(int i = 0; i < count; ++i){
    free(items[i]->label);
    free(items[i]->value);
    free(items[i]);
  }
  free(items);
}

void move_screen_up(menu_screen_t *screen)
{
  menu_t *menu
    = screen->sections[screen->selected_section]->menu;
  if(menu->selected_item - 1 < 0)
    return;

  menu->selected_item--;
}

void move_screen_down(menu_screen_t *screen)
{
  menu_t *menu
    = screen->sections[screen->selected_section]->menu;
  if(menu->selected_item + 1 > menu->item_count - 1)
    return;

  menu->selected_item++;
}

void move_screen_left(menu_screen_t *screen)
{
  if(screen->selected_section - 1 < 0)
    return;

  screen->selected_section--;
}

void move_screen_right(menu_screen_t *screen)
{
  if(screen->selected_section + 1 > screen->count - 1)
    return;

  screen->selected_section++;
}

menu_item_t *get_selected_item(menu_screen_t *screen)
{
  section_t *section = screen->sections[screen->selected_section];
  menu_item_t *item = section->menu->items[section->menu->selected_item];
  return item; 
}

void add_item_callback(menu_item_t *item, menu_item_callback_t cb)
{
  item->cb = cb;
}

void remove_item_callback(menu_item_t* item)
{
  item->cb = NULL;
}
