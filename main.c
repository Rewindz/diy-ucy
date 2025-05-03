#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

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
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
  }

  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
}

void draw_screen(menu_screen_t *screen)
{
  int start_y = 2;
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  int section_width = max_x / screen->count;
  
  for(int sec = 0; sec < screen->count; ++sec){
    int start_x = sec * section_width;

    attron(A_BOLD | COLOR_PAIR(2));
    mvprintw(start_y, start_x + 2, "%s", screen->sections[sec]->title);
    attroff(A_BOLD | COLOR_PAIR(2));

    for(int item = 0; item < screen->sections[sec]->menu->item_count; ++item){
      int attrs;
      if(sec == screen->selected_section && item == screen->sections[sec]->menu->selected_item)
	attrs = (A_REVERSE | COLOR_PAIR(1));
      else
	attrs = (COLOR_PAIR(1));
      attron(attrs);

      mvprintw(start_y + item + 2, start_x + 2, "%s",
	       screen->sections[sec]->menu->items[item]->label); 
      
      attroff(attrs);
    }

    if (sec < screen->count - 1){
      for(int y = start_y; y < max_y; ++y)
	mvaddch(y, start_x + section_width - 1, ACS_VLINE);
    }
  }
  refresh();
}


int main(int argc, char **argv)
{
  init();

  menu_screen_t *screen;
  
  { //Create sections
    section_t *section_a;
    section_t *section_b;
    
    { //Section a
      menu_item_t *labelA
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Hello world!");
      menu_item_t *labelB
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Second label!");
      menu_t *menu
	= create_new_menu(2, labelA, labelB);
      section_a
	= create_new_section("Section A", menu);
    }
    { //Section b
      menu_item_t *labelA
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Goodbye world!");
      menu_item_t *labelB
	= create_new_menu_item(ITEM_TYPE_LABEL,
			       "Forth label!");
      menu_t *menu
	= create_new_menu(2, labelA, labelB);
      section_b
	= create_new_section("Section B", menu);
    }
    screen
      = create_new_menu_screen(2, section_a, section_b);
  }

  draw_screen(screen);
  
  int ch;
  while((ch = getch()) != 'q'){
    switch (ch){
    case KEY_UP:
      move_screen_up(screen);
      break;
    case KEY_DOWN:
      move_screen_down(screen);
      break;
    case KEY_RIGHT:
      move_screen_right(screen);
      break;
    case KEY_LEFT:
      move_screen_left(screen);
      break;
    }
    draw_screen(screen);
  }


  free_whole_screen(screen);
  endwin();
  return 0;
}
