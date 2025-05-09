#include "render.h"


void draw_item(int sec_w, int x, int y, menu_item_t *item)
{
  mvprintw(y, x, "%-*s", sec_w - 2, " ");
  
  switch(item->type){
  case ITEM_TYPE_INPUT:
  case ITEM_TYPE_LABEL_VALUED:
    mvprintw(y, x, "%s: %s", item->label, ( (item->value) ? item->value : "\0" ));
    break;
  default:
    mvprintw(y, x, "%s", item->label);
  }

}

void draw_screen(menu_screen_t *screen)
{
  int start_y = 2;
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  int section_width = max_x / screen->count;
  
  for(int sec = 0; sec < screen->count; ++sec){

    section_t *section = screen->sections[sec];
    
    int start_x = sec * section_width;

    int attrs = (sec == screen->selected_section) ? (A_REVERSE | A_BOLD | COLOR_PAIR(2)) : (A_BOLD | COLOR_PAIR(2));
    attron(attrs);
    mvprintw(start_y, start_x + 2, "%s", section->title);
    attroff(attrs);

    for(int item = 0; item < section->menu->item_count; ++item){

      menu_item_t *m_item = section->menu->items[item];
      
      attrs = (item == section->menu->selected_item) ?
	  (A_REVERSE | COLOR_PAIR(1)) : (COLOR_PAIR(1));
      attron(attrs);

      draw_item(section_width, start_x + 2, start_y + item + 2, m_item);
      
      attroff(attrs);
    }

    if (sec < screen->count - 1){
      for(int y = start_y; y < max_y; ++y)
	mvaddch(y, start_x + section_width - 1, ACS_VLINE);
    }
  }
  refresh();
}


void draw_input_popup(menu_item_t *item)
{
  const int popup_height = 7;
  const int popup_width = 40;
  int start_y = (LINES - popup_height) / 2;
  int start_x = (COLS - popup_width) / 2;

  WINDOW *popup = newwin(popup_height, popup_width, start_y, start_x);
  box(popup, 0, 0);
  mvwprintw(popup, 1, 2, "Enter value for: %s", item->label);
  mvwprintw(popup, 3, 2, "> ");

  int max_len = popup_width - 6;
  char buffer[128] = {0};
  int pos = 0;

  if (item->value)
    strncpy(buffer, item->value, sizeof(buffer) - 1);

  echo();
  curs_set(1);
  wmove(popup, 3, 4 + strlen(buffer));
  wrefresh(popup);

  int ch;
  pos = strlen(buffer);
  mvwprintw(popup, 3, 4, "%-*s", max_len, buffer);
  wmove(popup, 3, 4 + pos);
  while(1){
    ch = wgetch(popup);
    if (ch == '\n') break;
    else if (ch == 27){ //ESC
      buffer[0] = '\0';
      break;
    }else if( (ch == KEY_BACKSPACE || ch == 127) && pos > 0){
      buffer[--pos] = '\0';
      mvwprintw(popup, 3, 4, "%-*s", max_len, buffer);
      wmove(popup, 3, 4 + pos);
    }else if(isprint(ch) && pos < max_len - 1) {
      buffer[pos++] = ch;
      buffer[pos] = '\0';
      mvwprintw(popup, 3, 4, "%-*s", max_len, buffer);
      wmove(popup, 3, 4 + pos);
    }

    wrefresh(popup);
    
  }

  if(item->value)
    free(item->value);
  item->value = strdup(buffer);

  curs_set(0);
  noecho();
  werase(popup);
  wrefresh(popup);
  delwin(popup);


}

void clear_and_draw(menu_screen_t *screen) 
{
  werase(stdscr);
  draw_screen(screen); 
}
