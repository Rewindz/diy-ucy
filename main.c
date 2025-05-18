#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

#include "menus/appmenu.h"
#include "render.h"
#include "input.h"
#include "types.h"

void init()
{
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);
  if(has_colors()){
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_WHITE, COLOR_CYAN);
  }

  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
}


int main(int argc, char **argv)
{
  init();

  menu_screen_t **screen = create_app_menu();
  if(!screen || !*screen){
    endwin();
    return 1;
  }
  
  draw_screen(*screen);
  
  int ch;
  while((ch = getch()) != 'q'){
    switch (ch){
    case KEY_UP:
      move_screen_up(*screen);
      break;
    case KEY_DOWN:
      move_screen_down(*screen);
      break;
    case KEY_RIGHT:
      move_screen_right(*screen);
      break;
    case KEY_LEFT:
      move_screen_left(*screen);
      break;
    case '\n':
      item_on_enter(*screen, get_selected_item(*screen));
      break;
    }
    draw_screen(*screen);
  }


  free_whole_screen(*screen);
  free(screen);
  endwin();
  return 0;
}
